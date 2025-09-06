#pragma once

#include <Windows.h>

typedef PVOID(*CreateInterfaceFn)(const char* name, int* returnCode);
PVOID GetInterface(const char* moduleName, const char* interfaceName)
{
    CreateInterfaceFn createInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(moduleName), "CreateInterface");
    return createInterface(interfaceName, NULL);
}