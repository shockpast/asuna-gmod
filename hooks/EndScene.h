#pragma once

#include "../deps/imgui/imgui_impl_dx9.h"
#include "../deps/fonts/cascadia_mono.h"

#include <intrin.h>

typedef int(__thiscall* _EndScene)(IDirect3DDevice9*);
_EndScene oEndScene;

bool initialized = false;

IDirect3DDevice9* game_device;
uintptr_t overlay;
ImFont* font;

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

int __fastcall hkEndScene(IDirect3DDevice9* device)
{
	if (!game_device)
		game_device = device;
	
	if (!overlay)
	{
		uintptr_t address = (uintptr_t)_ReturnAddress();
		MEMORY_BASIC_INFORMATION info;

		VirtualQuery((LPVOID)address, &info, sizeof(MEMORY_BASIC_INFORMATION));
		char moduleName[MAX_PATH];
		GetModuleFileNameA((HMODULE)info.AllocationBase, moduleName, MAX_PATH);

		if (std::string(moduleName).find("gameoverlay") != std::string::npos)
			overlay = address;
	}

	if (overlay != (uintptr_t)_ReturnAddress())
		return oEndScene(device);

	if (!initialized)
	{
		ImGui::CreateContext();

		EngineClient->GetScreenSize(globals::screenWidth, globals::screenHeight);

		globals::window = FindWindowA("Valve001", nullptr);
		globals::oWndProc = (WNDPROC)SetWindowLongPtrA(globals::window, GWLP_WNDPROC, (LONG_PTR)WndProc);

		IDirect3DSwapChain9* pChain = nullptr;

		D3DPRESENT_PARAMETERS pp = {};
		D3DDEVICE_CREATION_PARAMETERS param = {};

		device->GetCreationParameters(&param);
		device->GetSwapChain(0, &pChain);

		if (pChain)
			pChain->GetPresentParameters(&pp);

		ImGui_ImplWin32_Init(globals::window);
		ImGui_ImplDX9_Init(device);

		ImGuiIO& io = ImGui::GetIO();
		io.LogFilename = NULL;
		io.IniFilename = NULL;

		io.FontDefault = io.Fonts->AddFontFromMemoryCompressedTTF(
			CascadiaMono_compressed_data,
			CascadiaMono_compressed_size,
			14.0f,
			nullptr,
			io.Fonts->GetGlyphRangesCyrillic()
		);

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

	IDirect3DVertexDeclaration9* vertexDeclaration;
	IDirect3DVertexShader9* vertexShader;

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorWrite);
	device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbWrite);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	device->GetVertexDeclaration(&vertexDeclaration);
	device->GetVertexShader(&vertexShader);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

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

		// Switches
		if (true) gui::DrawLogger(); // just to make it consistent
		if (globals::menu::tabs::drawMisc) gui::DrawMisc();
		if (globals::menu::tabs::drawLua) gui::DrawLua();

		ImGui::End();
	}

	if (settings::misc::watermark) gui::DrawWatermark();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetRenderState(D3DRS_COLORWRITEENABLE, colorWrite);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbWrite);
	device->SetVertexDeclaration(vertexDeclaration);
	device->SetVertexShader(vertexShader);

	if (rt)
	{
		if (context = MaterialSystem->GetRenderContext())
		{
			context->BeginRender();
			context->SetRenderTarget(rt);
			context->EndRender();

			logger::AddLog("[warning] screengrab -> [render] @ %s", GetTime().c_str());
		}
	}

	return oEndScene(device);
}