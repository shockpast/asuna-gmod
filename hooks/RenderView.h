#pragma once

#include "../globals.h"

#include "../features/gui/gui.h"

typedef bool(__thiscall* _RenderView)(CViewRender*, CViewSetup&, int, int);
_RenderView oRenderView;

// https://www.unknowncheats.me/forum/2378329-post1.html
bool __fastcall hkRenderView(CViewRender* ViewRender, CViewSetup& view, int nClearFlags, int whatToDraw)
{
	return oRenderView(ViewRender, view, nClearFlags, whatToDraw);
}