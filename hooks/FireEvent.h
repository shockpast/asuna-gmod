#pragma once

#include "../globals.h"

#include "../features/gui/gui.h"

typedef void(__thiscall* _FireEvent)(IGameEventManager2*, IGameEvent*, bool);
_FireEvent oFireEvent;

void __fastcall hkFireEvent(IGameEventManager2* thisptr, IGameEvent* pEvent, bool bDontBroadcast)
{
    return oFireEvent(thisptr, pEvent, bDontBroadcast);
}