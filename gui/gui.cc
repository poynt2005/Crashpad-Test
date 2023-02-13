#include <windows.h>
#include <string>
#include <string.h>
#include "gui.h"

#define BUTTON_01 0xce5

const char *szWindowClass = "CrashExampleApp";
const char *szTitle = "CrashExampleApp";
const char *textMsg = "Click To Crash!!";

bool isAppInitialized = false;
HWND hWnd;

std::function<void()> BUTTON_01Callback;

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 15 + 80, 15, textMsg, strlen(textMsg));
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case BUTTON_01:
            if (BUTTON_01Callback)
            {
                BUTTON_01Callback();
            }
            break;
        }
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
}

bool CreateBtn()
{
    HWND btnHwnd = CreateWindow(
        "BUTTON",
        "Crash!!",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        6 + 80, 50,
        120, 30,
        hWnd,
        reinterpret_cast<HMENU>(BUTTON_01),
        reinterpret_cast<HINSTANCE>(GetModuleHandle(0)),
        NULL);

    if (!hWnd)
    {
        return false;
    }
    return true;
}

bool InitializeApp()
{
    WNDCLASSEX wcex;

    HMODULE hInstance = GetModuleHandle(0);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_ASTERISK);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_ASTERISK);

    if (!RegisterClassEx(&wcex))
    {
        return false;
    }

    hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        (ScreenX - 300) / 2, (ScreenY - 150) / 2,
        300, 150,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!hWnd)
    {
        return false;
    }

    if (!CreateBtn())
    {
        return false;
    }

    isAppInitialized = true;
    return true;
}

void RenderFrame(int *message)
{
    if (isAppInitialized)
    {
        MSG msg;
        while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
        {
            *message = static_cast<unsigned int>(msg.message);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void RegisterButtonCallback(std::function<void()> &cb)
{
    if (BUTTON_01Callback)
    {
        std::function<void()> oldCallback = std::move(BUTTON_01Callback);
        BUTTON_01Callback = [&]() -> void
        {
            oldCallback();
            cb();
        };
    }
    else
    {
        BUTTON_01Callback = std::move(cb);
    }
}

void ShowAndUpdateWindow()
{
    if (isAppInitialized)
    {
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
}

void MainLoop()
{
    if (isAppInitialized)
    {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}
