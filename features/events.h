#pragma once

#include "../globals.h"

#include "gui.h"

const char* events[5] = {
	"player_hurt", "player_death", "player_spawn", "player_team",
	"entity_killed"
};

class IEvents : public IGameEventListener2
{
public:
	static inline IGameEvent* cEvent;
	IEvents(void) {
		for (const char* event : events)
		{
			EventManager->AddListener(this, event, false);
		}
	}
	~IEvents(void) { EventManager->RemoveListener(this); }
	void FireGameEvent(IGameEvent* event) override;
};

// fuck LUA_FUNCTION, we go raw
int get_name(lua_State* state) { Lua->PushString(IEvents::cEvent->GetName()); return 1; }
int get_string(lua_State* state) { Lua->PushString(IEvents::cEvent->GetString(Lua->GetString(1))); return 1; }
int get_int(lua_State* state) { Lua->PushNumber(IEvents::cEvent->GetInt(Lua->GetString(1))); return 1; }
int get_bool(lua_State* state) { Lua->PushBool(IEvents::cEvent->GetBool(Lua->GetString(1))); return 1; }
int get_float(lua_State* state) { Lua->PushNumber(IEvents::cEvent->GetFloat(Lua->GetString(1))); return 1; }
int get_uint64(lua_State* state) { Lua->PushLong(IEvents::cEvent->GetUint64(Lua->GetString(1))); return 1; }

void IEvents::FireGameEvent(IGameEvent* pEvent)
{
	IEvents::cEvent = pEvent;

	if (!pEvent)
		return;

	Lua->PushSpecial(SPECIAL_GLOB);
	// this shit is very dangerous, since servers can detour hook.Add
	// shockpast: ^^ :thumbs_up:
	Lua->GetField(-1, "hook");
	Lua->GetField(-1, "Run");
	Lua->PushString("asuna.event");
	Lua->CreateTable();
	{
		Lua->PushString("get_name");
		Lua->PushCFunction(get_name);
		Lua->SetTable(-3);

		Lua->PushString("get_string");
		Lua->PushCFunction(get_string);
		Lua->SetTable(-3);

		Lua->PushString("get_int");
		Lua->PushCFunction(get_int);
		Lua->SetTable(-3);

		Lua->PushString("get_bool");
		Lua->PushCFunction(get_bool);
		Lua->SetTable(-3);

		Lua->PushString("get_float");
		Lua->PushCFunction(get_float);
		Lua->SetTable(-3);

		Lua->PushString("get_uint64");
		Lua->PushCFunction(get_uint64);
		Lua->SetTable(-3);
	}
	Lua->Call(2, 0);
	Lua->Pop(2);
}