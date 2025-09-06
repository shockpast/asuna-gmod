#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "ext/memory.h"
#include "ext/interface.h"

#include "hooks/PaintTraverse.h"
#include "hooks/Present.h"
#include "hooks/RunStringEx.h"
#include "hooks/RenderView.h"
#include "hooks/Paint.h"

#include "features/lua_api.h"
#include "features/config.h"

#include "globals.h"

#include <fstream>

void main()
{
	EngineClient = (CEngineClient*)GetInterface("engine.dll", "VEngineClient015");
	LuaShared = (CLuaShared*)GetInterface("lua_shared.dll", "LUASHARED003");
	ClientEntityList = (CClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
	CHLclient = (CHLClient*)GetInterface("client.dll", "VClient017");
	MaterialSystem = (CMaterialSystem*)GetInterface("materialsystem.dll", "VMaterialSystem080");
	InputSystem = (CInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion001");
	CVar = (CCvar*)GetInterface("vstdlib.dll", "VEngineCvar007");
	RenderView = (CVRenderView*)GetInterface("engine.dll", "VEngineRenderView014");
	PanelWrapper = (VPanelWrapper*)GetInterface("vgui2.dll", "VGUI_Panel009");
	MatSystemSurface = (CMatSystemSurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface030");
	EngineVGui = (void*)GetInterface("engine.dll", "VEngineVGui001");
	ModelInfo = (CModelInfo*)GetInterface("engine.dll", "VModelInfoClient006");

	ViewRender = GetVMT<CViewRender>((uintptr_t)CHLclient, 2, ViewRenderOffset);
	GlobalVars = GetVMT<CGlobalVarsBase>((uintptr_t)CHLclient, 0, GlobalVarsOffset);

	if (Lua = LuaShared->GetLuaInterface((unsigned char)LuaInterfaceType::LUA_CLIENT))
	{
		oRunStringEx = VMTHook<_RunStringEx>((PVOID**)Lua, (PVOID)hkRunStringEx, 111);
	}
	oCreateLuaInterfaceFn = VMTHook<_CreateLuaInterfaceFn>((PVOID**)LuaShared, (PVOID)hkCreateLuaInterfaceFn, 4);
	oCloseLuaInterfaceFn = VMTHook<_CloseLuaInterfaceFn>((PVOID**)LuaShared, (PVOID)hkCloseInterfaceLuaFn, 5);

	oPaint = VMTHook<_Paint>((PVOID**)EngineVGui, (PVOID)hkPaint, 13);
	oPaintTraverse = VMTHook<_PaintTraverse>((PVOID**)PanelWrapper, (PVOID)hkPaintTraverse, 41);
	oRenderView = VMTHook<_RenderView>((PVOID**)ViewRender, (PVOID)hkRenderView, 6);

	present = GetRealFromRelative((char*)FindPattern(PresentModule, PresentPattern, "Present"), 0x2, 6, false);

	oPresent = *(_Present*)(present);
	*(_Present**)(present) = (_Present*)hkPresent;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	MatSystemSurface->PlaySound("HL1/fvox/activated.wav");

	config::init();
}

BOOL APIENTRY DllMain(HMODULE hModule, uintptr_t uReason, LPVOID lpReserved)
{
	if (uReason == DLL_PROCESS_ATTACH)
	{
		std::thread(main).detach();
	}

	return true;
}