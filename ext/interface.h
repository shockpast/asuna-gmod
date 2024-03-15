#pragma once

#include <Windows.h>

template<typename T>
static T* GetInterface(const char* libraryName, const char* interfaceName)
{
    const HINSTANCE handle = GetModuleHandleA(libraryName);

    using Fn = T*(*)(const char*, int*);
    const Fn CreateInterface = reinterpret_cast<Fn>(GetProcAddress(handle, "CreateInterface"));

    printf("[?] GetInterface '%s' in '%s'\n", interfaceName, libraryName);

    return CreateInterface(interfaceName, nullptr);
}