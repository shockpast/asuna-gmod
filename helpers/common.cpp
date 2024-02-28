#include <string>
#include <ctime>
#include <filesystem>

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