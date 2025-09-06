#pragma once

#include "../globals.h"

#include "../headers/sdk/luajit.h"

#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <filesystem>
#include <mutex>

std::mutex runnerMutex;

namespace fs = std::filesystem;

static std::vector<std::string> reserved
{
	"con", "prn", "aux", "nul", "com0", "com1", "com2", "com3", "com4", "com5",
	"com6", "com7", "com8", "com9", "com10", "lpt0", "lpt1", "lpt2", "lpt3",
	"lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9", "lpt10"
};

static bool IsBadCharacter(const char& c)
{
	if ((c >= 0x0 && c <= 0x1f) || c < 0) {
		return true;
	}

	if (c == '_' || c == '-' || c == '.' || c == '/' || c == '\\' || isalnum(c)) {
		return false;
	}

	return true;
}

bool IsReserved(fs::path& path)
{
	std::string pathstr = path.string();
	std::transform(pathstr.begin(), pathstr.end(), pathstr.begin(), ::tolower);

	return (std::find(reserved.begin(), reserved.end(), pathstr) != reserved.end());
}

fs::path RemoveReservedWords(const fs::path& path)
{
	auto newpath = std::filesystem::path();

	for (const auto& e : path) 
	{
		const auto& stem = e.stem();

		if ((std::find(stem.begin(), stem.end(), "..") != stem.end()))
		{
			continue;
		}

		if (IsReserved((fs::path&)stem)) 
		{
			newpath /= ("_" + stem.string());
		}
		else 
		{
			newpath /= stem.string();
		}
	}

	return newpath;
}

fs::path SanitizePath(std::string pathstr)
{
	pathstr = std::filesystem::path(pathstr).relative_path().string();
	pathstr.erase(std::remove_if(pathstr.begin(), pathstr.end(), IsBadCharacter), pathstr.end());

	while (!pathstr.empty() && (pathstr.back() == ' ' || pathstr.back() == '.'))
	{
		pathstr.pop_back();
	}

	auto path = std::filesystem::path(pathstr);
	path = RemoveReservedWords(path);

	if (path.string().length() >= 200) 
	{
		return std::filesystem::path(path.string().substr(0, 196) + ".lua");
	}

	if (!path.has_filename() || path.filename().string().front() == '.') 
	{
		path.replace_filename("noname");
	}

	path.replace_extension(".lua");

	return path;
}

void RunScript(std::string input)
{
	Lua = LuaShared->GetLuaInterface((unsigned char)LuaInterfaceType::LUA_MENU);
	
	if (EngineClient->IsInGame())
	{
		Lua = LuaShared->GetLuaInterface((unsigned char)LuaInterfaceType::LUA_CLIENT);
	}

	globals::lua::mutex.lock();
	globals::lua::queue.push(input);
	globals::lua::mutex.unlock();
}

void SaveScript(std::string fileName, std::string fileContent)
{
	try
	{
		fs::path rootPath = "C:\\asuna\\servers\\";
		std::string serverName;

		if (EngineClient->IsConnected())
		{
			auto netChannel = EngineClient->GetNetChannelInfo();
			serverName = netChannel ? netChannel->GetAddress() : "unknown";
		}
		else
		{
			serverName = "menustate";
		}

		std::replace(serverName.begin(), serverName.end(), '.', '-');
		std::replace(serverName.begin(), serverName.end(), ':', '_');

		auto sanitizedFileName = SanitizePath(fileName);

		auto& path = rootPath;
		path /= serverName;
		path /= sanitizedFileName;

		try
		{
			fs::create_directories(path.parent_path());
		}
		catch (...)
		{
		}

		auto of = std::ofstream(path, settings::lua::scripthook::dumpMode);
		of << fileContent;
		of.close();
	}
	catch (...)
	{
	}
}