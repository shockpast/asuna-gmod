#pragma once

#include "../globals.h"

typedef void(__thiscall* _SendNetMsg)(CNetChan*, INetMessage&, bool, bool);
_SendNetMsg oSendNetMsg;

void __fastcall hkSendNetMsg(CNetChan* _this, INetMessage& message, bool bForceReliable = false, bool bVoice = false)
{
    return oSendNetMsg(_this, message, bForceReliable, bVoice);
}