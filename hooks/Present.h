#pragma once

#include "../deps/imgui/imgui.h"
#include "../deps/imgui/imgui_impl_dx9.h"
#include "../deps/imgui/imgui_impl_win32.h"

#include "../globals.h"

#include "../features/gui.h"

long __stdcall hkPresent(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}