#pragma once
#include <Windows.h>

bool DrawLine(HDC hDC, DWORD dwBrushColor, int iSize, int iType, float fLeft, float fTop, float fRigth, float fDown);

bool DrawElipse(HDC hDC, DWORD dwBrushColor, int iSize, float fLeft, float fTop);