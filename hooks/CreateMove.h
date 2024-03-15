#pragma once

#include "../globals.h"

#include "../features/gui.h"

typedef void(__thiscall* _CreateMove)(ClientModeShared*, float, CUserCmd*);
_CreateMove oCreateMove;

void __fastcall hkCreateMove(ClientModeShared* _this, float flInputSampleTime, CUserCmd* cmd)
{
	// simple bunnyhop
	if (settings::misc::bunnyhop)
	{
		// getting local player every fucking tick is good good!
		C_BasePlayer* player = (C_BasePlayer*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());

		if (!player)
			return;
		if (!player->IsAlive())
			return;
		if (player->GetMoveType() == MOVETYPE_LADDER || player->GetMoveType() == MOVETYPE_NOCLIP)
			return;
		if (player->GetFlags() & FL_INWATER)
			return;

		if (cmd->buttons & IN_JUMP && !(player->GetFlags() & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}

	return oCreateMove(_this, flInputSampleTime, cmd);
}