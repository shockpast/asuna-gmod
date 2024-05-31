#include <Windows.h>

#include <filesystem>
#include <fstream>
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

std::string GetFileContent(const char* path)
{
	std::ifstream file(path);
	if (!file)
		return "";

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	if (content.empty())
		return "";

	file.close();

	return content;
};