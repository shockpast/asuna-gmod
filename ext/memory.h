#pragma once

#pragma once

#include <Windows.h>

#include <string_view>
#include <limits>
#include <array>
#include <string>
#include <vector>

#include <psapi.h>
#include <signal.h>

void BytePatch(PVOID source, BYTE newValue);

struct HookData { PVOID** src; PVOID dst; int index; };
extern std::vector<HookData> VMTHooks;

template<typename T>
T VMTHook(PVOID** src, PVOID dst, int index, bool noRestore = false)
{
    PVOID* VMT = *src;
    PVOID ret = (VMT[index]);

    DWORD originalProtection;

    VirtualProtect(&VMT[index], sizeof(PVOID), PAGE_EXECUTE_READWRITE, &originalProtection);
    VMT[index] = dst;
    VirtualProtect(&VMT[index], sizeof(PVOID), originalProtection, &originalProtection);
    
    if (!noRestore)
    {
        HookData currData = { src, ret, index };
        VMTHooks.push_back(currData);
    }

    return (T)ret;
};

void RestoreVMTHook(PVOID** src, PVOID dst, int index);
void RestoreVMTHooks();

const char* FindPattern(const char* moduleName, std::string_view pattern, std::string patternName) noexcept;

char* GetRealFromRelative(char* address, int offset, int instructionSize = 6, bool isRelative = true);

template<typename T>
T* GetVMT(uintptr_t address, int index, uintptr_t offset)
{
    uintptr_t step = 3;
    uintptr_t instructionSize = 7;
    uintptr_t instruction = ((*(uintptr_t**)(address))[index] + offset);

    uintptr_t relativeAddress = *(DWORD*)(instruction + step);
    uintptr_t realAddress = instruction + instructionSize + relativeAddress;
    return *(T**)(realAddress);
}

template<typename T>
T* GetVMT(uintptr_t address, uintptr_t offset)
{
    uintptr_t step = 3;
    uintptr_t instructionSize = 7;
    uintptr_t instruction = address + offset;

    uintptr_t relativeAddress = *(DWORD*)(instruction + step);
    uintptr_t realAddress = instruction + instructionSize + relativeAddress;
    return *(T**)(realAddress);
}

std::string RandomString(int length);