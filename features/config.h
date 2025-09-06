#pragma once

#include <fstream>
#include <filesystem>

#include "../ext/toml/toml.hpp"

#include "../globals.h"

namespace config {
	void init()
	{
		std::ifstream file("C:\\asuna\\user.toml");

		std::map<std::string, int> dumpModes;
		dumpModes["truncate"] = std::ofstream::trunc;
		dumpModes["append"] = std::ofstream::app;
		dumpModes["binary"] = std::ofstream::binary;

		if (!file)
		{
			std::filesystem::create_directory("C:\\asuna");

			std::ofstream file("C:\\asuna\\user.toml");
			auto data = toml::parse("C:\\asuna\\user.toml");

			data["scripthook"] = {};
			data["scripthook"]["mode"] = "truncate";
			data["scripthook"]["enabled"] = settings::lua::scripthook::dumpServer;

			data["menu"] = {};
			data["menu"]["key"] = static_cast<int>(KEY_INSERT);

			file << data;

			return;
		}

		auto data = toml::parse("C:\\asuna\\user.toml");

		const auto& scripthookPreferences = toml::find(data, "scripthook");
		settings::lua::scripthook::dumpMode = dumpModes[toml::find<std::string>(scripthookPreferences, "mode")];
		settings::lua::scripthook::dumpServer = toml::find<bool>(scripthookPreferences, "enabled");

		const auto& menuPreferences = toml::find(data, "menu");
		settings::menu::key = static_cast<ButtonCode_t>(toml::find<int>(menuPreferences, "key"));
	}
}