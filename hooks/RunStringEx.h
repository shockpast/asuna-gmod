#pragma once

#include "../globals.h"

#include "../hooks/SendNetMsg.h"

#include "../features/lua.h"
#include "../features/gui.h"

#include "../ext/memory.h"

#include <string>

typedef bool(__thiscall* _RunStringEx)(CLuaInterface*, const char*, const char*, const char*, bool, bool, bool, bool);
_RunStringEx oRunStringEx = nullptr;

typedef CLuaInterface* (__thiscall* _CreateLuaInterfaceFn)(CLuaShared*, LuaInterfaceType, bool);
_CreateLuaInterfaceFn oCreateLuaInterfaceFn;

typedef int(__thiscall* _CloseLuaInterfaceFn)(CLuaShared*, CLuaInterface*);
_CloseLuaInterfaceFn oCloseLuaInterfaceFn;

bool __fastcall hkRunStringEx(CLuaInterface* _this, const char* filename, const char* path, const char* stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns)
{
	if (settings::lua::scripthook::dumpServer)
        SaveScript(filename, stringToRun);

	return oRunStringEx(_this, filename, path, stringToRun, run, printErrors, dontPushErrors, noReturns);
}

int __fastcall hkCloseInterfaceLuaFn(CLuaShared* _this, CLuaInterface* luaInterface)
{
    if (luaInterface == Lua) Lua = nullptr;
    return oCloseLuaInterfaceFn(_this, luaInterface);
}

CLuaInterface* __fastcall hkCreateLuaInterfaceFn(CLuaShared* _this, LuaInterfaceType luaState, bool renew)
{
    auto luaInterface = oCreateLuaInterfaceFn(_this, luaState, renew);
    
    if (luaState != LuaInterfaceType::LUA_CLIENT) 
        return luaInterface;

    Lua = luaInterface;

    if (!oRunStringEx) 
        oRunStringEx = VMTHook<_RunStringEx>((PVOID**)Lua, (PVOID)hkRunStringEx, 111, true);
    if (!oSendNetMsg) 
        oSendNetMsg = VMTHook<_SendNetMsg>((PVOID**)EngineClient->GetNetChannelInfo(), (PVOID)hkSendNetMsg, 40, true);

    return Lua;
}