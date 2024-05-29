#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "deps/memory.h"
#include "deps/interface.h"
#include "deps/kiero/kiero.h"

#include "hooks/WriteUsercmdDeltaToBuffer.h"
#include "hooks/PaintTraverse.h"
#include "hooks/RunStringEx.h"
#include "hooks/CreateMove.h"
#include "hooks/RenderView.h"
#include "hooks/FireEvent.h"
#include "hooks/EndScene.h"
#include "hooks/Present.h"
#include "hooks/Paint.h"

#include "features/config.h"
#include "features/events.h"

#include "globals.h"

#include <thread>

int main()
{
#ifdef _DEBUG
	FreeConsole();
	AllocConsole();
	AttachConsole(GetCurrentProcessId());

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
	oWriteUserCMDDeltaToBuffer = VMTHook<_WriteUserCMDDeltaToBuffer>((PVOID**)CHLclient, (PVOID) hkWriteUserCMDDeltaToBuffer,  23);

	kiero::init(kiero::RenderType::D3D9);
	kiero::bind(17, (void**)&oPresent, reinterpret_cast<void*>(hkPresent));
	kiero::bind(42, (void**)&oEndScene, reinterpret_cast<void*>(hkEndScene));

	Events = new IEvents();
	config::init();
}

BOOL WINAPI DllMain(HMODULE hModule, uintptr_t uReason, LPVOID lpReserved)
{
	if (uReason == DLL_PROCESS_ATTACH)
	{
		std::thread(main).detach();
	}

	return true;
}