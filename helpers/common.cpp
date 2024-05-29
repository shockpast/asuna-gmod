#include <Windows.h>

#include <filesystem>
#include <string>
#include <ctime>

std::string GetTime()
{
	time_t t = std::time(nullptr);
	tm tm;

	localtime_s(&tm, &t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S");

	return oss.str();
}

std::string RandomString(int length)
{
	std::string output;
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

	for (int i = 0; i < length; i++)
		output += alphabet[rand() % (alphabet.length() - 1)];

	return output;
}

std::string ToUTF8(std::string message)
{
	std::string ret;
	int len = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)message.c_str(), message.length(), NULL, 0, NULL, NULL);
	if (len > 0)
	{
		ret.resize(len);
		WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)message.c_str(), message.length(), &ret[0], len, NULL, NULL);
	}
	return ret;
}