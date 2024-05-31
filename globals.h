#pragma once

#define WIN32_LEAN_AND_MEAN

#include "deps/texteditor/text_editor.h"
#include "deps/imfilebr/file_browser.h"

#include "headers/tier0/shareddefs.h"
#include "headers/client/ConVar.h"
#include "headers/client/CHLClient.h"
#include "headers/client/C_BasePlayer.h"
#include "headers/client/CViewRender.h"
#include "headers/client/CInputSystem.h"
#include "headers/client/CClientEntityList.h"
#include "headers/client/ClientModeShared.h"
#include "headers/engine/CEngineClient.h"
#include "headers/engine/CVRenderView.h"
#include "headers/engine/CModelInfo.h"
#include "headers/engine/CModelRender.h"
#include "headers/engine/CMaterialSystem.h"
#include "headers/engine/CGameEventManager.h"
#include "headers/vgui/IEngineVGui.h"
#include "headers/vgui/VPanelWrapper.h"
#include "headers/lua_shared/CLuaShared.h"
#include "headers/vguimatsurface/CMatSystemSurface.h"

#include <d3d9.h>

#include <mutex>
#include <stack>

#define _VERSION "01.06.2024"

#define ViewRenderOffset 0xC4
#define GlobalVarsOffset 0x94
#define ClientModeOffset 0x0

class IEvents;

typedef long(__stdcall* _Present)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef void(__thiscall* _PaintTraverse)(void*, VPanel*, bool, bool);

CLuaShared* LuaShared;
CLuaInterface* Lua;
CClientEntityList* ClientEntityList;
CHLClient* CHLclient;
ClientModeShared* ClientMode;
CGlobalVarsBase* GlobalVars;
C_BasePlayer* LocalPlayer;
CEngineClient* EngineClient;
CViewRender* ViewRender;
CInputSystem* InputSystem;
CCvar* CVar;
CModelRender* ModelRender;
CModelInfo* ModelInfo;
CMaterialSystem* MaterialSystem;
CMatSystemSurface* MatSystemSurface;
CVRenderView* RenderView;
VPanelWrapper* PanelWrapper;
IGameEventManager2* EventManager;
IEngineVGui* EngineVGui;

char* present;
_Present oPresent;
_PaintTraverse oPaintTraverse;

IEvents* Events;

namespace globals
{
	HWND window;
	WNDPROC oWndProc;

	int screenWidth;
	int screenHeight;

	namespace menu
	{
		namespace tabs
		{
			bool drawLua = false;
			bool drawMisc = false;
			bool drawExploits = false;
		}

		bool visible = false;
		TextEditor editor;
		imgui_addons::ImGuiFileBrowser file_browser; // wtf is this naming bro
	}

	namespace lua
	{
		std::mutex mutex;
		std::stack<std::string> queue;
	}
}

namespace settings
{
	namespace menu
	{
		ButtonCode_t key = KEY_INSERT;
	}

	namespace lua
	{
		namespace scripthook
		{
			bool dumpServer;
			int dumpMode;
		}
	}

	namespace misc
	{
		bool watermark;
		bool bunnyhop;
	}
}