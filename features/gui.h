#pragma once

#include "../deps/imgui/imgui.h"
#include "../deps/imgui/imgui_impl_dx9.h"
#include "../deps/imgui/imgui_impl_win32.h"

#include "lua.h"

#include "../globals.h"

#include "../helpers/common.h"

#include <cstdarg>
#include <format>

std::once_flag SetTextFlag = {};

namespace logger {
	namespace settings {
		bool autoScroll;
	}

	ImVector<char*> logs;

	// ext/imgui/imgui_demo.cpp:7006
	static char* Strdup(const char* s) {
		IM_ASSERT(s);
		size_t len = strlen(s) + 1;
		void* buf = ImGui::MemAlloc(len);
		IM_ASSERT(buf);
		return (char*)memcpy(buf, (const void*)s, len);
	}

	void ClearConsole()
	{
		for (int i = 0; i < logs.Size; i++)
			ImGui::MemFree(logs[i]);

		logs.clear();
	}

	void AddLog(const char* fmt, ...)
	{
		char buf[1024];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);

		buf[IM_ARRAYSIZE(buf) - 1] = 0;

		va_end(args);

		logs.push_back(Strdup(buf));
	}
}

namespace gui {
	const char* filename = "unknown.lua";

	void UpdateEditor(std::string text)
	{
		globals::menu::editor.SetText(text.c_str());
	}

	void DrawLua()
	{
		if (ImGui::Begin("asuna - lua", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
		{
			bool open = false, save = false;
			TextEditor::Coordinates cursorPosition = globals::menu::editor.GetCursorPosition();

			ImGui::Text(
				"%6d/%-6d %6d lines | %s | %s", 
				cursorPosition.mLine + 1, 
				cursorPosition.mColumn + 1, 
				globals::menu::editor.GetTotalLines(),
				globals::menu::editor.GetLanguageDefinition().mName.c_str(),
				gui::filename
			);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save"))
						save = true;
					if (ImGui::MenuItem("Open"))
						open = true;

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Dark Theme"))
						globals::menu::editor.SetPalette(TextEditor::GetDarkPalette());
					if (ImGui::MenuItem("Light Theme"))
						globals::menu::editor.SetPalette(TextEditor::GetLightPalette());
					if (ImGui::MenuItem("Retro-Blue Theme"))
						globals::menu::editor.SetPalette(TextEditor::GetRetroBluePalette());

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			if (ImGui::Button("Execute", ImVec2(100.f, 0.f)))
				std::thread(RunScript, globals::menu::editor.GetText()).detach();

			if (open)
				ImGui::OpenPopup("Open File");
			if (save)
				ImGui::OpenPopup("Save File");

			if (globals::menu::file_browser.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".lua"))
			{
				gui::filename = globals::menu::file_browser.selected_fn.c_str();
				std::string content = GetFileContent(globals::menu::file_browser.selected_path.c_str());
				globals::menu::editor.SetText(content);
			}
			else if (globals::menu::file_browser.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".lua"))
			{
				gui::filename = globals::menu::file_browser.selected_fn.c_str();
				std::ofstream file(globals::menu::file_browser.selected_path.c_str());
				file << globals::menu::editor.GetText();
				file.close();
			}

			globals::menu::editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
			globals::menu::editor.SetColorizerEnable(true);

			globals::menu::editor.Render("Lua Executor", ImVec2(900.f, 500.f), true);
		}

		std::call_once(SetTextFlag, UpdateEditor, globals::menu::editor.GetText());
		ImGui::End();
	}

	void DrawMisc()
	{
		ImGui::SetNextWindowContentSize(ImVec2(300.f, 0.0f));
		if (ImGui::Begin("asuna - misc", nullptr, ImGuiWindowFlags_NoResize))
		{
			ImGui::Checkbox("ScriptHook", &settings::lua::scripthook::dumpServer);
			ImGui::Checkbox("Watermark", &settings::misc::watermark);
			ImGui::Checkbox("Bunnyhop", &settings::misc::bunnyhop);
		}

		ImGui::End();
	}

	void DrawLogger()
	{
		ImGui::SetNextWindowSize(ImVec2(500, 264)); // magic number

		if (ImGui::Begin("asuna - console", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::BeginPopupContextItem())
			{
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &logger::settings::autoScroll);
				ImGui::EndPopup();
			}

			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");

			ImGui::Separator();

			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar))
			{
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::Selectable("Clear"))
						logger::ClearConsole();
					ImGui::EndPopup();
				}

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
				for (const char* item : logger::logs)
				{
					ImVec4 color;
					bool has_color = false;

					if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
					if (strstr(item, "[warning]")) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
					if (strstr(item, "[debug]")) { color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); has_color = true; }
					if (strstr(item, "[success]")) { color = ImVec4(0.49f, 0.90f, 0.46f, 1.0f); has_color = true; }

					if (has_color)
						ImGui::PushStyleColor(ImGuiCol_Text, color);

					ImGui::TextUnformatted(item);

					if (has_color)
						ImGui::PopStyleColor();
				}

				if (logger::settings::autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
					ImGui::SetScrollHereY(1.0f);

				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
		}

		ImGui::End();
	}

	void DrawWatermark()
	{
		std::string time = GetTime();
		std::string username = "unknown";

		if (EngineClient->IsInGame())
		{
			player_info_s info;
			EngineClient->GetPlayerInfo(EngineClient->GetLocalPlayer(), &info);

			username = info.name;
		}

		std::string text = std::format("asuna | {} | {}", username, time);
		ImVec2 size = ImGui::CalcTextSize(text.c_str());

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		int x = globals::screenWidth;
		int y = globals::screenHeight;

		// side * amount of sides
		int padding = 4 * 2;

		int min = x - size.x - padding - 6;
		int max = min + size.x + padding;

		list->AddRectFilled(ImVec2(min, 4), ImVec2(max, 6), ImColor(128, 132, 255));
		list->AddRectFilled(ImVec2(min, 6), ImVec2(max, 25), ImColor(18, 18, 18, 100));

		list->AddText(ImVec2(x - size.x - 10, 8), ImColor(255, 255, 255), text.c_str());
	}
}