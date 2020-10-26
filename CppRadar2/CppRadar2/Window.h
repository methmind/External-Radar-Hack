#pragma once
#include <Windows.h>

#define ScreenHeight 260
#define ScreenWidth 260

LRESULT WINAPI OwnWndProc(HWND pHwnd, UINT dwMsg, WPARAM wParam, LPARAM lParam);

bool InitWindowClass(HINSTANCE hInst, LPCWSTR lpClassName);

bool SetWindowTransparent(HWND hWnd);

bool ShowOwnWindow(HWND hWnd);