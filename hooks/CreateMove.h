#pragma once

#include "../globals.h"

#include "../features/gui.h"
#include "../helpers/common.h"

typedef bool(__thiscall* _CreateMove)(ClientModeShared*, float, CUserCmd*);
_CreateMove oCreateMove;

bool __fastcall hkCreateMove(ClientModeShared* _this, float flInputSampleTime, CUserCmd* cmd)
{
	if (settings::misc::bunnyhop)
	{
		C_BasePlayer* player = (C_BasePlayer*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());

		if (!player)
			return false;
		if (!player->IsAlive())
			return false;
		if (player->GetMoveType() == MOVETYPE_LADDER || player->GetMoveType() == MOVETYPE_NOCLIP)
			return false;
		if (player->GetFlags() & FL_INWATER)
			return false;

		if (cmd->buttons & IN_JUMP && !(player->GetFlags() & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}

	return oCreateMove(_this, flInputSampleTime, cmd);
}