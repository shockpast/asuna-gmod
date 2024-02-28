#pragma once

#include <Windows.h>
#include <stdint.h>

class IGameEvent {
public:
    virtual ~IGameEvent() {};
    virtual const char* GetName() const = 0;

    virtual bool  IsReliable() const = 0;
    virtual bool  IsLocal() const = 0;
    virtual bool  IsEmpty(const char* keyName = NULL) = 0;

    virtual bool  GetBool(const char* keyName = NULL, bool defaultValue = false) = 0;
    virtual int   GetInt(const char* keyName = NULL, int defaultValue = 0) = 0;
    virtual unsigned long long GetUint64(char const* keyName = NULL, unsigned long long defaultValue = 0) = 0;
    virtual float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f) = 0;

    virtual const char* GetString(const char* keyName = NULL, const char* defaultValue = "") = 0;
    virtual const wchar_t* GetWString(char const* keyName = NULL, const wchar_t* defaultValue = L"") = 0;

    virtual void Unk() = 0;

    virtual void SetBool(const char* keyName, bool value) = 0;
    virtual void SetInt(const char* keyName, int value) = 0;
    virtual void SetUInt64(const char* keyName, unsigned long long value) = 0;
    virtual void SetFloat(const char* keyName, float value) = 0;
    virtual void SetString(const char* keyName, const char* value) = 0;
    virtual void SetWString(const char* keyName, const wchar_t* value) = 0;
};

class IGameEventListener2
{
public:
    virtual    ~IGameEventListener2(void) {};

    virtual void FireGameEvent(IGameEvent* event) = 0;

};

// Auto reconstructed from vtable block @ 0x004F3AD8
// from "engine.dylib", by ida_vtables.idc
// Modified VTable dumper script obviously by t.me/Gaztoof.
class IGameEventManager2
{
public:
    /* 0 */ virtual ~IGameEventManager2(void) {};
    /* 1 */ virtual int LoadEventsFromFile(const char* filename) = 0;
    /* 2 */ virtual void Reset() = 0;
    /* 3 */ virtual bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide) = 0;
    /* 4 */ virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
    /* 5 */ virtual void RemoveListener(IGameEventListener2* listener) = 0;
    /* 6 */ virtual void AddListenerGlobal(IGameEventListener2* listener, bool bIsServerSide) = 0;
    /* 7 */ virtual IGameEvent* CreateEvent(const char* name, bool bForce = false, int* pCookie = NULL) = 0;
    /* 8 */ virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
    /* 9 */ virtual bool FireEventClientSide(IGameEvent* event) = 0;
    /* 10 */ virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
    /* 11 */ virtual void FreeEvent(IGameEvent* event) = 0;
    /* 12 */ virtual bool SerializeEvent(IGameEvent* event, int* buf) = 0;
    /* 13 */ virtual IGameEvent* UnserializeEvent(int* buf) = 0;
    /* 14 */ virtual void* GetEventDataTypes(IGameEvent* event) = 0;
};