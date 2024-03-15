#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "ext/memory.h"
#include "ext/interface.h"
#include "ext/kiero/kiero.h"

#include "hooks/PaintTraverse.h"
#include "hooks/RunStringEx.h"
#include "hooks/SendNetMsg.h"
#include "hooks/CreateMove.h"
#include "hooks/RenderView.h"
#include "hooks/FireEvent.h"
#include "hooks/Present.h"
#include "hooks/Paint.h"

#include "features/lua_api.h"
#include "features/config.h"
#include "features/events.h"

#include "globals.h"

#include <fstream>
#include <d3d9.h>

int main()
{
#ifdef _DEBUG
	AllocConsole();
	SetConsoleTitleA(std::format("[debug] t.me/neshockpast - {}", _VERSION).c_str());
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	EngineClient = GetInterface<CEngineClient>("engine.dll", "VEngineClient015");
	LuaShared = GetInterface<CLuaShared>("lua_shared.dll", "LUASHARED003");
	ClientEntityList = GetInterface<CClientEntityList>("client.dll", "VClientEntityList003");
	CHLclient = GetInterface<CHLClient>("client.dll", "VClient017");
	MaterialSystem = GetInterface<CMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
	InputSystem = GetInterface<CInputSystem>("inputsystem.dll", "InputSystemVersion001");
	CVar = GetInterface<CCvar>("vstdlib.dll", "VEngineCvar007");
	ModelRender = GetInterface<CModelRender>("engine.dll", "VEngineModel016");
	RenderView = GetInterface<CVRenderView>("engine.dll", "VEngineRenderView014");
	PanelWrapper = GetInterface<VPanelWrapper>("vgui2.dll", "VGUI_Panel009");
	MatSystemSurface = GetInterface<CMatSystemSurface>("vguimatsurface.dll", "VGUI_Surface030");
	EngineVGui = GetInterface<IEngineVGui>("engine.dll", "VEngineVGui001");
	ModelInfo = GetInterface<CModelInfo>("engine.dll", "VModelInfoClient006");
	EventManager = GetInterface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");

	ViewRender = GetVMT<CViewRender>((uintptr_t)CHLclient, 2, ViewRenderOffset);
	GlobalVars = GetVMT<CGlobalVarsBase>((uintptr_t)CHLclient, 0, GlobalVarsOffset);
	ClientMode = GetVMT<ClientModeShared>((uintptr_t)CHLclient, 10, ClientModeOffset);

	oCreateLuaInterfaceFn = VMTHook<_CreateLuaInterfaceFn>((PVOID**)LuaShared, (PVOID)hkCreateLuaInterfaceFn, 4);
	oCloseLuaInterfaceFn = VMTHook<_CloseLuaInterfaceFn>((PVOID**)LuaShared, (PVOID)hkCloseInterfaceLuaFn, 5);

	oPaint = VMTHook<_Paint>((PVOID**)EngineVGui, (PVOID)hkPaint, 13);
	oPaintTraverse = VMTHook<_PaintTraverse>((PVOID**)PanelWrapper, (PVOID)hkPaintTraverse, 41);
	oRenderView = VMTHook<_RenderView>((PVOID**)ViewRender, (PVOID)hkRenderView, 6);
	oFireEvent = VMTHook<_FireEvent>((PVOID**)EventManager, (PVOID)hkFireEvent, 8);
	oCreateMove = VMTHook<_CreateMove>((PVOID**)ClientMode, (PVOID)hkCreateMove, 21);

	printf("[+] 'kiero' initializing... (%d)\n", (int)kiero::init(kiero::RenderType::D3D9));
	printf("[+] 'kiero' binding to Present... (%d)\n", (int)kiero::bind(17, (void**)&oPresent, hkPresent));

	Events = new IEvents();
	config::init();
}

BOOL WINAPI DllMain(HMODULE module, uintptr_t reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		std::thread(main).detach();

	return true;
}