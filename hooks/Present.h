#pragma once

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../ext/imgui/imgui_impl_win32.h"

#include "../globals.h"

#include "../features/gui/gui.h"

#ifndef GWL_WNDPROC
#define GWL_WNDPROC GWLP_WNDPROC
#endif

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static bool lastState = false;

	if (wParam == InputSystem->ButtonCodeToVirtualKey(settings::menu::key))
	{
		if (uMsg == WM_KEYDOWN && lastState == false) 
		{
			lastState = true;
			globals::menu::visible = !globals::menu::visible;
		}
		else if (uMsg == WM_KEYUP)
		{
			lastState = false;
		}
	}

	if (globals::menu::visible) 
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProcA(globals::oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	static bool initialized = false;
	static int tab = 3;

	if (!initialized)
	{
		ImGui::CreateContext();

		globals::window = FindWindowA("Valve001", nullptr);
		globals::oWndProc = (WNDPROC)SetWindowLongPtrA(globals::window, GWL_WNDPROC, (LONG_PTR)WndProc);

		IDirect3DSwapChain9* pChain = nullptr;
		
		D3DPRESENT_PARAMETERS pp = {};
		D3DDEVICE_CREATION_PARAMETERS param = {};

		pDevice->GetCreationParameters(&param);
		pDevice->GetSwapChain(0, &pChain);

		if (pChain)
		{
			pChain->GetPresentParameters(&pp);
		}

		ImGui_ImplWin32_Init(globals::window);
		ImGui_ImplDX9_Init(pDevice);

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		io.LogFilename = NULL;
		io.IniFilename = NULL;

		initialized = true;
	}

	ITexture* rt = nullptr;
	auto context = MaterialSystem->GetRenderContext();

	if (context)
	{
		context->BeginRender();
		rt = context->GetRenderTarget();
		context->SetRenderTarget(nullptr);
		context->EndRender();
	}

	DWORD colorWrite, srgbWrite;
	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorWrite);
	pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbWrite);

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	IDirect3DVertexDeclaration9* vertexDeclaration;
	IDirect3DVertexShader9* vertexShader;
	pDevice->GetVertexDeclaration(&vertexDeclaration);
	pDevice->GetVertexShader(&vertexShader);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	{
		if (globals::menu::visible)
		{
			ImGui::Begin("asuna", nullptr, ImGuiWindowFlags_NoResize);

			// Tabs
			if (ImGui::Button("Misc", ImVec2(100.f, 0.f)))
			{
				globals::menu::tabs::drawMisc = !globals::menu::tabs::drawMisc;
			}
			ImGui::SameLine(0.f, 2.f);
			if (ImGui::Button("Lua", ImVec2(100.f, 0.f)))
			{
				globals::menu::tabs::drawLua = !globals::menu::tabs::drawLua;
			}
			ImGui::SameLine(0.f, 2.f);
			if (ImGui::Button("Credits", ImVec2(100.f, 0.f)))
			{
				globals::menu::tabs::drawCredits = !globals::menu::tabs::drawCredits;
			}
			ImGui::SameLine(0.f, 2.f);

			// Switches
			if (true) gui::DrawLogger(); // just to make it consistent
			if (globals::menu::tabs::drawMisc) gui::DrawMisc();
			if (globals::menu::tabs::drawLua) gui::DrawLua();
			if (globals::menu::tabs::drawCredits) gui::DrawCredits();

			ImGui::End();
		}
	}

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorWrite);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbWrite);

	pDevice->SetVertexDeclaration(vertexDeclaration);
	pDevice->SetVertexShader(vertexShader);

	if (rt)
	{
		if (context = MaterialSystem->GetRenderContext())
		{
			context->BeginRender();
			context->SetRenderTarget(rt);
			context->EndRender();
		}
	}

	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}