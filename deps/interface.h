#pragma once

#include <Windows.h>

template<typename T>
static T* GetInterface(const char* libraryName, const char* interfaceName)
{
    const HINSTANCE handle = GetModuleHandleA(libraryName);
    if (!handle)
    {
        printf("[!] Couldn't find handle for %s", libraryName);
        return nullptr;
    }

    using Fn = T*(*)(const char*, int*);
    const Fn CreateInterface = reinterpret_cast<Fn>(GetProcAddress(handle, "CreateInterface"));

    return CreateInterface(interfaceName, nullptr);
}