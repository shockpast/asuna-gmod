#pragma once

#include "../globals.h"

#include "../features/gui.h"
#include "../features/lua.h"
#include "../features/lua_api.h"

#include "../headers/sdk/luajit.h"

#include "RunStringEx.h"

void __fastcall hkPaintTraverse(VPanelWrapper* _this, VPanel* panel, bool force_repaint, bool allow_force)
{
	if (!strcmp(PanelWrapper->GetName(panel), "FocusOverlayPanel"))
	{
		PanelWrapper->SetKeyBoardInputEnabled(panel, globals::menu::visible);
		PanelWrapper->SetMouseInputEnabled(panel, globals::menu::visible);

		InputSystem->EnableInput(!globals::menu::visible);

		globals::lua::mutex.lock();

		while (!globals::lua::queue.empty())
		{
			const std::string& script = globals::lua::queue.top();

			if (luajit::call_luaL_loadbufferx(Lua->GetLuaState(), script.c_str(), script.length(), RandomString(16).c_str(), NULL))
			{
				std::string error = Lua->GetString(-1);
				Lua->Pop();

				logger::AddLog("[error] syntax error: %s\n", error.c_str());
			}

			globals::lua::queue.pop();

			if (Lua->IsType(-1, LuaObjectType::FUNCTION))
			{
				lua_api::init();

				if (Lua->PCall(0, 0, 0))
				{
					std::string error = Lua->GetString(-1);
					Lua->Pop();

					logger::AddLog("[error] execution error: %s\n", error.c_str());
				}
			}
		}

		globals::lua::mutex.unlock();
	}

	return oPaintTraverse(_this, panel, force_repaint, allow_force);
}