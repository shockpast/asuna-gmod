#pragma once

#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_dx9.h"
#include "../../ext/imgui/imgui_impl_win32.h"

#include "../../globals.h"

#include <cstdarg>

std::once_flag SetTextFlag = {};

namespace logger {
	namespace settings {
		bool autoScroll;
	}

	ImVector<char*> logs;

	// ext/imgui/imgui_demo.cpp:7006
	static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = ImGui::MemAlloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }

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

#include "../lua.h"

namespace gui {
	const char* executorState[2] {
		"Client",
		"Menu",
	};

	// https://github.com/BalazsJako/ImGuiColorTextEdit/issues/145#issuecomment-1868678140
	void UpdateEditor(std::string text)
	{
		globals::menu::editor.SetText(text);
	}

	void DrawLua()
	{
		if (ImGui::Begin("asuna - lua", nullptr, ImGuiWindowFlags_NoResize))
		{
			globals::menu::editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
			globals::menu::editor.SetColorizerEnable(true);

			TextEditor::Coordinates cursorPosition = globals::menu::editor.GetCursorPosition();

			ImGui::Text("%6d/%-6d %6d lines | %s | %s | %s", cursorPosition.mLine + 1, cursorPosition.mColumn + 1, globals::menu::editor.GetTotalLines(),
				globals::menu::editor.IsOverwrite() ? "Ovr" : "Ins",
				globals::menu::editor.CanUndo() ? "*" : " ",
				globals::menu::editor.GetLanguageDefinition().mName.c_str());

			if (ImGui::Button("Execute", ImVec2(100.f, 0.f)))
			{
				std::thread(RunScript, globals::menu::editor.GetText()).detach();
			}

			globals::menu::editor.Render("Lua Executor", ImVec2(700.f, 400.f), true);
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
		}

		ImGui::End();
	}

	void DrawCredits()
	{
		ImGui::SetNextWindowContentSize(ImVec2(300.f, 0.0f));
		if (ImGui::Begin("asuna - credits", nullptr, ImGuiWindowFlags_NoResize))
		{
			ImGui::Text("0w0");
		}

		ImGui::End();
	}

	void DrawLogger()
	{
		ImGui::SetNextWindowSize(ImVec2(500, 300.f));
		if (ImGui::Begin("asuna - logger", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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

			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
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
}