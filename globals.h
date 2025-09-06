#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include <map>
#include <mutex>
#include <stack>

#include "ext/imgui_ext/texteditor.h"

#include "headers/tier0/shareddefs.h"
#include "headers/tier1/bitbuf.h"
#include "headers/client/ConVar.h"
#include "headers/client/CHLClient.h"
#include "headers/client/C_BasePlayer.h"
#include "headers/client/CViewRender.h"
#include "headers/client/CInputSystem.h"
#include "headers/client/CClientEntityList.h"
#include "headers/engine/CEngineClient.h"
#include "headers/engine/CVRenderView.h"
#include "headers/engine/CModelInfo.h"
#include "headers/engine/CModelRender.h";
#include "headers/engine/CMaterialSystem.h"
#include "headers/vgui/VPanelWrapper.h"
#include "headers/lua_shared/CLuaShared.h"
#include "headers/vguimatsurface/CMatSystemSurface.h"

#include <d3d9.h>

#define _VERSION "22.02.2024"

#define ViewRenderOffset 0xC4
#define GlobalVarsOffset 0x94
#define ClientModeOffset 0x0
#define InputOffset 0x0

#define PresentModule "gameoverlayrenderer64"
#define PresentPattern "\xFF\x15????\x8B\xF8\xEB\x1E"

typedef HRESULT(__stdcall* _Present)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef void(__thiscall* _PaintTraverse)(void*, VPanel*, bool, bool);

CLuaShared* LuaShared;
CLuaInterface* Lua;
CClientEntityList* ClientEntityList;
CHLClient* CHLclient;
CGlobalVarsBase* GlobalVars;
C_BasePlayer* LocalPlayer;
CEngineClient* EngineClient;
CViewRender* ViewRender;
CInputSystem* InputSystem;
CCvar* CVar;
CModelInfo* ModelInfo;
CMaterialSystem* MaterialSystem;
CMatSystemSurface* MatSystemSurface;
CVRenderView* RenderView;
VPanelWrapper* PanelWrapper;

void* EngineVGui;

char* present;
_Present oPresent;
_PaintTraverse oPaintTraverse;

namespace globals {
	HWND window;
	WNDPROC oWndProc;

	int screenWidth;
	int screenHeight;

	std::atomic<vmatrix_t> viewMatrix;

	namespace menu {
		namespace tabs {
			bool drawLua = false;
			bool drawMisc = false;
			bool drawCredits = false;
			bool drawVisuals = false;
		}

		bool visible = false;
		TextEditor editor;
	}

	namespace lua {
		std::mutex mutex;
		std::stack<std::string> queue;
	}
}

namespace settings {
	namespace menu {
		ButtonCode_t key = KEY_INSERT;
	}

	namespace lua {
		namespace scripthook {
			bool dumpServer;
			int dumpMode;
		}
	}
}