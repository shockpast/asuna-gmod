#pragma once

#include "../globals.h"

typedef int (__thiscall* _WriteUserCMDDeltaToBuffer)(void*, int, int, bool);
_WriteUserCMDDeltaToBuffer oWriteUserCMDDeltaToBuffer;

// from "command_number"
// to   "command_number"
int __fastcall hkWriteUserCMDDeltaToBuffer(CHLClient* _this, int from, int to, bool isNewCommand)
{
	return oWriteUserCMDDeltaToBuffer(_this, from, to, isNewCommand);
}