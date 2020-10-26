#include <Windows.h>

LRESULT CALLBACK OwnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool InitWindowClass(HINSTANCE hInst, LPCWSTR lpClassName)
{
    WNDCLASSEXW winClass = { 0 };
    winClass.cbSize = sizeof(WNDCLASSEX);

    winClass.lpfnWndProc = OwnWndProc;
    winClass.hInstance = hInst;
    winClass.hbrBackground = CreateSolidBrush(RGB(23, 23, 23));
    winClass.lpszClassName = lpClassName;

    return RegisterClassExW(&winClass);
}

bool SetWindowTransparent(HWND hWnd)
{
    bool bRes = false;

    do
    {
        if (SetWindowLongW(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, -20) | 0x80000 | 0x20))
            break;

        if (!SetLayeredWindowAttributes(hWnd, 0, 200, LWA_ALPHA))
           break;

        bRes = true;
    } while (false);

    return bRes;
}

bool ShowOwnWindow(HWND hWnd)
{
    bool bRes = false;

    do
    {
        if (ShowWindow(hWnd, SW_SHOW))
            break;

        if (!UpdateWindow(hWnd))
            break;

        bRes = true;
    } while (false);

    return bRes;
}