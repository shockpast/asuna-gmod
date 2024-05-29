#pragma once

#include "../globals.h"

#include "gui.h"
#include "lua.h"

#include "../headers/sdk/luajit.h"

#include <filesystem>
#include <fstream>
#include <random>

namespace fs = std::filesystem;

namespace lua_api
{
	LUA_FUNCTION(log)
	{
		logger::AddLog(LUA->GetString(-1));

		return 0;
	}

	LUA_FUNCTION(clientcmd)
	{
		EngineClient->ClientCmd_Unrestricted(LUA->GetString(1));

		return 0;
	}

	LUA_FUNCTION(change_name)
	{
		const char* name = LUA->GetString(1);
		static uint8_t packet[256 + sizeof(name)];

		CNetChan* netchannel = EngineClient->GetNetChannelInfo();
		bf_write write;
		write.StartWriting(packet, sizeof(packet));
		write.WriteUInt(static_cast<uint32_t>(NetMessage::net_SetConVar), NET_MESSAGE_BITS);
		write.WriteByte(1);
		write.WriteString("name");
		write.WriteString(name);

		netchannel->SendData(&write, false);

		return 0;
	}

	LUA_FUNCTION(custom_disconnect)
	{
		const char* reason = LUA->GetString(1);
		static uint8_t packet[256 + sizeof(reason)];

		CNetChan* netchannel = EngineClient->GetNetChannelInfo();
		bf_write write;
		write.StartWriting(packet, sizeof(packet));
		write.WriteUInt(static_cast<uint32_t>(NetMessage::net_Disconnect), NET_MESSAGE_BITS);
		write.WriteString(reason);

		netchannel->SendData(&write, true);
		netchannel->Transmit(false);

		return 0;
	}

	LUA_FUNCTION(include)
	{
		std::string pathName("C:\\asuna\\lua\\");
		std::string fileName(LUA->GetString(1));
		std::string rootPath = pathName + fileName;

		if (!fs::exists(rootPath.c_str()))
		{
			logger::AddLog("[warning] %s doesn't exist!", fileName.c_str());
			return 0;
		}

		std::ifstream file(rootPath.c_str());
		std::string content((std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));
		if (content.length() <= 0)
		{
			logger::AddLog("[warning] %s is empty!", fileName.c_str());
			return 0;
		}

		try
		{
			std::thread(RunScript, content).detach();
		}
		catch (std::exception& e)
		{
			logger::AddLog("[error] %s", e.what());
		}

		return 0;
	}

	LUA_FUNCTION(common_random_string)
	{
		int length = LUA->GetNumber(1);
		const std::string set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, set.length() - 1);

		std::string result;
		result.reserve(length);

		for (size_t i = 0; i < length; ++i)
		{
			result += set[dis(gen)];
		}

		LUA->PushString(result.c_str());

		return 1;
	}

	void init()
	{
		// _ENV
		Lua->CreateTable();

		// _ENV metatable
		Lua->CreateTable();
		{
			Lua->PushSpecial(SPECIAL_GLOB);
			Lua->SetField(-2, "__index");
		}
		Lua->SetMetaTable(-2);

		// asuna table
		Lua->CreateTable();
		{
			// commons function that can make coding easier
			Lua->CreateTable();
			{
				// common_random_string should be cryptographically secure
				Lua->PushCFunction(common_random_string);
				Lua->SetField(-2, "random_string");
			}
			Lua->SetField(-2, "common");

			// asuna - logger
			// [debug], [warning], [error]
			Lua->PushCFunction(log);
			Lua->SetField(-2, "log");

			// unrestricted clientcmd for lua
			Lua->PushCFunction(clientcmd);
			Lua->SetField(-2, "clientcmd");

			// loads external script from
			// C:/asuna/lua/*.lua
			Lua->PushCFunction(include);
			Lua->SetField(-2, "include");

			if (EngineClient->IsInGame())
			{
				// changes name thru netchannel programatically
				Lua->PushCFunction(change_name);
				Lua->SetField(-2, "change_name");

				// disconnects localplayer with custom reason
				Lua->PushCFunction(custom_disconnect);
				Lua->SetField(-2, "custom_disconnect");
			}
		}
		Lua->SetField(-2, "asuna");

		luajit::call_lua_setfenv(Lua->GetLuaState(), -2);
	}
}