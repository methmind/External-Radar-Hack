#include <Windows.h>
#include <TlHelp32.h>
#include "psapi.h"
#pragma comment(lib,"psapi.lib")

#pragma function(memset)
void* memset(void* _Dst, int _Val, size_t _Size)
{
	byte* x = (byte*)_Dst;

	while (_Size--)
	{
		*x++ = _Val;
	}

	return _Dst;
}

LPCWSTR ExtractFileName(LPCWSTR lpString)
{
	if (!lpString)
		return 0;

	LPCWSTR pFileName = 0;

	while (*lpString)
	{
		if (*lpString == '/' || *lpString == '\\')
			pFileName = lpString + 1;

		*lpString++;
	}

	return pFileName;
}

LPVOID GetImageBase()
{
	__asm
	{
		mov eax, FS: [30h]
		mov eax, [eax + 0x08]
	}
}

void GenerateRandomName(LPWSTR lpDest)
{
	for (int i = 0; i < 16; i++)
	{
		lpDest[i] = (wchar_t)(65 + (GetTickCount() % (90 - 65 + 1)));
		Sleep(10);
	}

	lpDest[16] = 0;
}

DWORD GetProcID(LPCWSTR lpProcName)
{
	DWORD dwPID = 0;
	HANDLE hSnap = 0;
	PROCESSENTRY32W procEntry;

	do
	{
		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hSnap)
			break;

		procEntry.dwSize = sizeof(procEntry);

		if (!Process32FirstW(hSnap, &procEntry))
			break;

		do
		{
			if (lstrcmpW(procEntry.szExeFile, lpProcName) == 0)
			{
				dwPID = procEntry.th32ProcessID;
				break;
			}
		} while (Process32NextW(hSnap, &procEntry));

	} while (false);

	if (hSnap)
		CloseHandle(hSnap);

	return dwPID;
}

DWORD GetModuleBaseAddress(HANDLE hProcess, LPCWSTR lpModuleName)
{
	HMODULE hMods[1024];
	DWORD dwMods = 0;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &dwMods))
	{
		for (int i = 0; i < (dwMods / sizeof(HMODULE)); i++)
		{
			wchar_t szModName[MAX_PATH + 1] = { 0 };
			if (GetModuleFileNameExW(hProcess, hMods[i], szModName, sizeof(szModName)))
			{
				if (lstrcmpW(ExtractFileName(szModName), lpModuleName) == 0)
					return (DWORD)hMods[i];
			}
		}
	}
	return 0;
}