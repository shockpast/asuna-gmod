#pragma once

#include "../globals.h"

enum PaintMode_t
{
    PAINT_UIPANELS = (1 << 0),
    PAINT_INGAMEPANELS = (1 << 1),
    PAINT_CURSOR = (1 << 2)
};

typedef void(__thiscall* _Paint)(void*, PaintMode_t);
_Paint oPaint;

void __fastcall hkPaint(void* _this, PaintMode_t mode)
{
    return oPaint(_this, mode);
}