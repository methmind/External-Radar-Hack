#include "Window.h"
#include "Utils.h"
#include "Draw.h"
#include "Offset.h"
#include "Math.h"
#include <stdio.h>

DWORD GetHealthColor(int iHealth)
{
    DWORD dwGreen = iHealth * 2.55f;
    DWORD dwRed = 255 - dwGreen;

    return RGB(dwRed, dwGreen, 0);
}

bool IsC4Owner(HANDLE hProc, DWORD dwEntity, DWORD dwClientModule)
{
    DWORD dwWeapons[24] = { 0 };
    DWORD dwWeaponEntity = 0;
    int iWeaponId = 0;

    bool bRes = false;

    do
    {
        if (!ReadProcessMemory(hProc, (LPCVOID)(dwEntity + m_hMyWeapons), &dwWeapons, 24, 0))
            break;

        for (int i = 0; dwWeapons[i]; i++)
        {
            if (!ReadProcessMemory(hProc, (LPCVOID)(dwClientModule + Offset::dwEntityList + ((dwWeapons[i] & 0xFFF) - 1) * 0x10), &dwWeaponEntity, sizeof(DWORD), 0))
                break;

            if (!ReadProcessMemory(hProc, (LPCVOID)(dwWeaponEntity + Offset::m_iItemDefinitionIndex), &iWeaponId, sizeof(int), 0))
                break;

            if (iWeaponId == 49)
            {
                bRes = true;
                break;
            }
        }
    } while (false);

    return bRes;
}

DWORD DrawRadar(HANDLE hProc, HDC hDC, HDC backDC, RECT lpRect, DWORD dwClientModule, DWORD dwEngineModule)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(23, 23, 23));

    Vector2 vRadarCenter = { ScreenWidth / 2, ScreenHeight / 2 };
    Vector2 vMyPos;
    Vector2 veMyViewAngles;
    Vector2 vEntityPos;

    Vector3 veMyPos;
    Vector3 veEntityPos;

    DWORD dwLocalPlayer = 0;
    DWORD dwEntity = 0;
    DWORD dwClientState = 0;
    DWORD dwExitCode = 0;
    DWORD dwColor = 0;
    DWORD dwCode = 0;

    int iLocalTeam = 0;
    int iEntityTeam = 0;
    int iEntityHealth = 0;

    float fEntityDistance = 0;
    float fRadarZoom = 4.7;

    bool bIsDormant = false;
    bool bIsC4Owner = false;

    while (true)
    {
        if (!GetExitCodeProcess(hProc, &dwExitCode))
        {
            dwCode = 1;
            break;
        }

        if (dwExitCode != STILL_ACTIVE)
        {
            dwCode = 1;
            break;
        }

        if (!ReadProcessMemory(hProc, (LPCVOID)(dwClientModule + Offset::dwLocalPlayer), &dwLocalPlayer, sizeof(DWORD), 0))
            continue;

        if (!ReadProcessMemory(hProc, (LPCVOID)(dwLocalPlayer + Offset::m_iTeamNum), &iLocalTeam, sizeof(int), 0))
            continue;

        if (!ReadProcessMemory(hProc, (LPCVOID)(dwLocalPlayer + Offset::m_vecOrigin), &veMyPos, sizeof(Vector3), 0))
            continue;

        if (!ReadProcessMemory(hProc, (LPCVOID)(dwEngineModule + Offset::dwClientState), &dwClientState, sizeof(DWORD), 0))
            continue;

        if (!ReadProcessMemory(hProc, (LPCVOID)(dwClientState + Offset::dwClientState_ViewAngles), &veMyViewAngles, sizeof(Vector2), 0))
            continue;

        vMyPos.ToVector2(veMyPos);
        FillRect(backDC, &lpRect, hBrush);

        DrawLine(backDC, RGB(84, 84, 84), 1, PS_SOLID, 0, vRadarCenter.y, ScreenWidth, vRadarCenter.y);
        DrawLine(backDC, RGB(84, 84, 84), 1, PS_SOLID, vRadarCenter.x, 0, vRadarCenter.x, ScreenHeight);
        DrawElipse(backDC, RGB(0, 132, 255), 7, vRadarCenter.x, vRadarCenter.y);

        for (int i = 0; i < 64; i++)
        {
            if (!ReadProcessMemory(hProc, (LPCVOID)(dwClientModule + Offset::dwEntityList + i * 0x10), &dwEntity, sizeof(DWORD), 0))
                continue;

            if (!ReadProcessMemory(hProc, (LPCVOID)(dwEntity + Offset::m_iTeamNum), &iEntityTeam, sizeof(int), 0))
                continue;

            if (iEntityTeam == iLocalTeam)
                continue;

            if (!ReadProcessMemory(hProc, (LPCVOID)(dwEntity + Offset::m_iHealth), &iEntityHealth, sizeof(int), 0))
                continue;

            if (!iEntityHealth)
                continue;

            if (!ReadProcessMemory(hProc, (LPCVOID)(dwEntity + Offset::m_bDormant), &bIsDormant, sizeof(bool), 0))
                continue;

            if (!bIsDormant)
                dwColor = GetHealthColor(iEntityHealth);
            else
                dwColor = RGB(84, 84, 84);

            bIsC4Owner = IsC4Owner(hProc, dwEntity, dwClientModule);

            if (!ReadProcessMemory(hProc, (LPCVOID)(dwEntity + Offset::m_vecOrigin), &veEntityPos, sizeof(Vector3), 0))
                continue;

            vEntityPos.ToVector2(veEntityPos);
            vEntityPos = vMyPos - vEntityPos;

            fEntityDistance = vEntityPos.Length() * (0.02 * fRadarZoom);
            fEntityDistance = _min(fEntityDistance, fRadarZoom / 2);

            vEntityPos.Normalize();
            vEntityPos *= fEntityDistance;
            vEntityPos.Add(vRadarCenter);
            vEntityPos = RotatePoint(vEntityPos, vRadarCenter, -veMyViewAngles.y - 1);

            if (vEntityPos.x > ScreenWidth - 1)
                vEntityPos.x = ScreenWidth - 2;
            else if (vEntityPos.x < 1)
                vEntityPos.x = 2;

            if (vEntityPos.y > ScreenHeight - 1)
                vEntityPos.y = ScreenHeight - 2;
            else if (vEntityPos.y < 1)
                vEntityPos.y = 2;

            if (bIsC4Owner)
                DrawElipse(backDC, RGB(255, 0, 0), 10, vEntityPos.x, vEntityPos.y);

            DrawElipse(backDC, dwColor, 7, vEntityPos.x, vEntityPos.y);
            Sleep(1);
        }

        DrawLine(backDC, RGB(112, 112, 112), 2, PS_SOLID, 1, 0, 1, ScreenWidth);
        DrawLine(backDC, RGB(112, 112, 112), 2, PS_SOLID, ScreenWidth - 1, 0, ScreenWidth - 1, ScreenWidth);
        //
        DrawLine(backDC, RGB(112, 112, 112), 2, PS_SOLID, 0, 1, ScreenHeight, 1);
        DrawLine(backDC, RGB(112, 112, 112), 2, PS_SOLID, 0, ScreenWidth - 1, ScreenHeight, ScreenWidth - 1);

        BitBlt(hDC, 0, 0, ScreenWidth, ScreenHeight, backDC, 0, 0, SRCCOPY);
        Sleep(2);
    }
    
    return dwCode;
}

DWORD RadarThread(LPVOID lpArgs)
{
    HWND hWnd = (HWND)lpArgs;
    HANDLE hProc = 0;

    DWORD dwProcID = 0;
    DWORD dwClientModule = 0;
    DWORD dwEngineModule = 0;
    DWORD dwCode = 0;

    HDC hDC = 0;
    HDC backDC = 0;
    HBITMAP hBitmap = 0;

    LPCWSTR lpProcName = L"csgo.exe";
    LPCWSTR lpClientModule = L"client.dll";
    LPCWSTR lpEngineModule = L"engine.dll";

    RECT lpRect = { 0, 0, ScreenWidth, ScreenHeight };

    while (true)
    {
        dwProcID = GetProcID(lpProcName);
        if (!dwProcID)
            goto Cleanup;

        hProc = OpenProcess(PROCESS_ALL_ACCESS, true, dwProcID);
        if (!hProc)
            goto Cleanup;

        hDC = GetWindowDC(hWnd);
        if (!hDC)
            goto Cleanup;

        backDC = CreateCompatibleDC(hDC);
        if (!backDC)
            goto Cleanup;

        hBitmap = CreateCompatibleBitmap(hDC, ScreenWidth, ScreenHeight);
        if (!hBitmap)
            goto Cleanup;

        SelectObject(backDC, hBitmap);

        dwClientModule = GetModuleBaseAddress(hProc, lpClientModule);
        if (!dwClientModule)
            goto Cleanup;

        dwEngineModule = GetModuleBaseAddress(hProc, lpEngineModule);
        if (!dwEngineModule)
            goto Cleanup;

        dwCode = DrawRadar(hProc, hDC, backDC, lpRect, dwClientModule, dwEngineModule); //Radar

    Cleanup:

        if (hProc && !dwCode)
            CloseHandle(hProc);

        if (hBitmap)
            DeleteObject(hBitmap);

        if (hDC)
            ReleaseDC(hWnd, hDC);//DeleteDC(hDC);

        if (backDC)
            ReleaseDC(hWnd, hDC);//DeleteDC(hDC);

        Sleep(50);
    }

    return 0;
}