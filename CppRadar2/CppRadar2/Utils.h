#pragma once
#include <Windows.h>

LPVOID GetImageBase();

void GenerateRandomName(LPWSTR lpDest);

DWORD GetProcID(LPCWSTR lpProcName);

DWORD GetModuleBaseAddress(HANDLE hProcess, LPCWSTR lpModuleName);