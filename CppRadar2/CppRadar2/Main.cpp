#include "Window.h"
#include "Utils.h"
#include "Radar.h"

int main()
{
    HINSTANCE hInstance = (HINSTANCE)GetImageBase();
    HWND hWnd = 0;
    MSG pMsg = { 0 };

    wchar_t lpWinClassName[16];
    //DWORD dwCpuHash = 0;

    do
    {
        //dwCpuHash = GetCpuHash();
        //wsprintfW(lpWinClassName, L"%08X%04lX", dwCpuHash);
        GenerateRandomName(lpWinClassName);

        if (!InitWindowClass(hInstance, lpWinClassName))
            break;

        hWnd = CreateWindowExW(0, lpWinClassName, lpWinClassName, WS_POPUP, 20, 73, ScreenWidth, ScreenHeight, 0, 0, hInstance, 0);
        if (!hWnd)
            break;

        if (!SetWindowTransparent(hWnd))
            break;

        if (!ShowOwnWindow(hWnd))
            break;

        if (!SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE))
            break;

        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RadarThread, hWnd, 0, 0));

        while (GetMessageW(&pMsg, hWnd, 0, 0))
        {
            TranslateMessage(&pMsg);
            DispatchMessageW(&pMsg);
        }

    } while (false);

    FreeLibraryAndExitThread(hInstance, 0);
}