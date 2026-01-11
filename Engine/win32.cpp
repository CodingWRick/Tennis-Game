#include <windows.h>
#include <iostream>
#include "base.h"
#include "../Game/game.h"

globalVar bool running = true;

// Window callback
LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY:
            running = false;
            break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            renderState.width  = rect.right - rect.left;
            renderState.height = rect.bottom - rect.top;

            int renderSize = renderState.width * renderState.height * sizeof(u32);

            if (renderState.memory)
                VirtualFree(renderState.memory, 0, MEM_RELEASE);
            renderState.memory = VirtualAlloc(0, renderSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            renderState.bitMapInfo.bmiHeader.biSize        = sizeof(renderState.bitMapInfo.bmiHeader);
            renderState.bitMapInfo.bmiHeader.biWidth       = renderState.width;
            renderState.bitMapInfo.bmiHeader.biHeight      = -renderState.height;
            renderState.bitMapInfo.bmiHeader.biPlanes      = 1;
            renderState.bitMapInfo.bmiHeader.biBitCount    = 32;
            renderState.bitMapInfo.bmiHeader.biCompression = BI_RGB;
        } break;

        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return result;
}

HWND gWindow = NULL;

static bool gIsFullscreen = true;
static DWORD gPrevStyle = WS_OVERLAPPEDWINDOW;
static DWORD gPrevExStyle = 0;
static RECT gPrevRect = { CW_USEDEFAULT, CW_USEDEFAULT, 1060, 1060 };

static void ToggleFullscreen(HWND hwnd) {
    if (!hwnd) return;
    if (gIsFullscreen) {
        // switch to windowed (bordered) but still cover the full screen
        SetWindowLongPtrW(hwnd, GWL_STYLE, (LONG_PTR)WS_OVERLAPPEDWINDOW);
        SetWindowLongPtrW(hwnd, GWL_EXSTYLE, (LONG_PTR)gPrevExStyle);
        int screenW = GetSystemMetrics(SM_CXSCREEN);
        int screenH = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, screenW, screenH,
                     SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        gIsFullscreen = false;
    } else {
        // save current windowed styles/rect then go fullscreen
        gPrevStyle = (DWORD)GetWindowLongPtrW(hwnd, GWL_STYLE);
        gPrevExStyle = (DWORD)GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
        GetWindowRect(hwnd, &gPrevRect);

        int screenW = GetSystemMetrics(SM_CXSCREEN);
        int screenH = GetSystemMetrics(SM_CYSCREEN);

        SetWindowLongPtrW(hwnd, GWL_STYLE, (LONG_PTR)WS_POPUP);
        SetWindowLongPtrW(hwnd, GWL_EXSTYLE, 0);
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, screenW, screenH, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        gIsFullscreen = true;
    }
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASS wc = {};
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = "GameWindowClass";
    wc.lpfnWndProc   = WindowCallback;
    wc.hInstance     = hInstance;

    RegisterClass(&wc);

    // Create a borderless fullscreen window by default
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    HWND window = CreateWindowA(
        wc.lpszClassName,
        "test.exe",
        WS_POPUP | WS_VISIBLE,
        0, 0,
        screenW, screenH,
        NULL, NULL, hInstance, NULL
    );

    if (!window) return 0;

    // Ensure the window is topmost and sized to cover the entire screen
    SetWindowPos(window, HWND_TOPMOST, 0, 0, screenW, screenH, SWP_SHOWWINDOW);

    gWindow = window;

    HDC hdc = GetDC(window);
    Input input = {};

    float deltaTime = .016f;
    LARGE_INTEGER frameStartTime;
    QueryPerformanceCounter(&frameStartTime);

    float performanceFrequency; {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performanceFrequency = (float)perf.QuadPart;
    }

    while (running) {
        for (int i = 0; i < BUTTON_COUNT; i++) input.buttons[i].changed = false;

        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vkCode = (u32)message.wParam;
                    bool isDown = ((message.lParam & (1 << 31)) == 0);

                    switch (vkCode) {
                        case VK_F11:
                            input.buttons[BUTTON_F11] = {isDown, true};
                            if (isDown) ToggleFullscreen(window);
                            break;
                    }
                } break;

                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP: {
                    bool isDown = (message.message == WM_LBUTTONDOWN);
                    input.buttons[LEFT_MOUSE] = {isDown, true};
                } break;

                default:
                    TranslateMessage(&message);
                    DispatchMessage(&message);
            }
        }

        SimulateGame(&input, deltaTime);

        StretchDIBits(
            hdc,
            0, 0,
            renderState.width, renderState.height,
            0, 0,
            renderState.width, renderState.height,
            renderState.memory,
            &renderState.bitMapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        LARGE_INTEGER frameEndTime;
        QueryPerformanceCounter(&frameEndTime);
        deltaTime = (float)(frameEndTime.QuadPart - frameStartTime.QuadPart) / performanceFrequency;
        frameStartTime = frameEndTime;
    }

    return 0;
}