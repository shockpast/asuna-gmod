#pragma once

#include "../../deps/memory.h"

#include "../lua_shared/CLuaInterface.h"

#define IMPORTFUNC(ret, name, args, ...)	\
	static std::uintptr_t find_##name() {	\
		static std::uintptr_t funcAddr = reinterpret_cast<std::uintptr_t>(GetProcAddress(GetModuleHandleA("lua_shared.dll"), #name));	\
		return funcAddr;					\
	}										\
	static ret call_##name args {			\
		return reinterpret_cast<ret(__cdecl*)args>(find_##name())(__VA_ARGS__);	\
	}

namespace luajit
{
	IMPORTFUNC(int, lua_setfenv, (lua_State* state, int pos), state, pos);
	IMPORTFUNC(int, luaL_loadbufferx, (lua_State* state, const char* buf, std::size_t size, const char* name, const char* mode), state, buf, size, name, mode);
}