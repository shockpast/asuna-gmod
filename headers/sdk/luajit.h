#pragma once

#include "../lua_shared/CLuaInterface.h"

#define LUA_IDSIZE 60

struct lj_Debug {
	/* Common fields. Must be in the same order as in lua.h. */
	int event;
	const char* name;
	const char* namewhat;
	const char* what;
	const char* source;
	int currentline;
	int nups;
	int linedefined;
	int lastlinedefined;
	char short_src[LUA_IDSIZE];
	int i_ci;
	/* Extended fields. Only valid if lj_debug_getinfo() is called with ext = 1.*/
	int nparams;
	int isvararg;
};

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
	IMPORTFUNC(int, lua_getinfo, (lua_State* state, const char* what, lj_Debug* ar), state, what, ar);
	IMPORTFUNC(char*, lua_tolstring, (lua_State* state, int idx, size_t* len), state, idx, len);
	IMPORTFUNC(int, luaL_loadstring, (lua_State* state, const char* s), state, s);
}