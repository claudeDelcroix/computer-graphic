#include <Windows.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include <stdio.h>
#include "Frame.h"
#include "Painter.h"

float        global_angle = 0.0f;
ClickedPixel global_clicked_pixel = { -1, -1 };

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HWND hWndStatusBar;

int WINAPI WinMain(HINSTANCE hInst,
    HINSTANCE,
    LPSTR,
    int    nCmdShow)
{
    // Init de la barre de statut
    INITCOMMONCONTROLSEX icc{ sizeof(icc), ICC_BAR_CLASSES };
    InitCommonControlsEx(&icc);

    wchar_t szClass[] = L"Graphics Window Class";
    WNDCLASS wc{};
    wc.hInstance = hInst;
    wc.lpszClassName = szClass;
    wc.lpfnWndProc = WindowProc;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClass(&wc)) return 0;


    HWND hWnd = CreateWindow(
        szClass,
        L"Laboratoire 1 – Lounou Claude Delcroix",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, hInst, nullptr
    );

    // Barre de statut
    hWndStatusBar = CreateWindowExA(
        0, STATUSCLASSNAMEA, nullptr,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0,
        hWnd, (HMENU)10001, hInst, nullptr
    );
    int parts[] = { 150, 300, -1 };
    SendMessageA(hWndStatusBar, SB_SETPARTS,
        _countof(parts), (LPARAM)parts);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return int(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    static int pixelSize = 1;

    switch (msg) {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000 / 30, nullptr);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC        hdc = BeginPaint(hWnd, &ps);
        RECT       rc;
        GetClientRect(hWnd, &rc);

        int winW = rc.right;
        int winH = rc.bottom - 22;  // laisse la barre visible
        int W = winW / pixelSize;
        int H = winH / pixelSize;

        Frame   frame(W, H);
        Painter painter;
        painter.Draw(frame);

        typedef struct { BYTE B, G, R, A; } PIX;
        PIX* bmp = (PIX*)HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            winW * (winH + 22) * sizeof(PIX)
        );

        for (int y = 0; y < H * pixelSize; ++y) {
            for (int x = 0; x < W * pixelSize; ++x) {
                PIX* p = bmp + (y * winW + x);
                COLOR c = frame.GetPixel(x / pixelSize,
                    y / pixelSize);
                p->R = c.RED; p->G = c.GREEN;
                p->B = c.BLUE; p->A = c.ALPHA;
            }
        }

        HBITMAP hBmp = CreateBitmap(
            winW, winH + 22, 1,
            sizeof(PIX) * 8, bmp
        );
        HDC memDC = CreateCompatibleDC(hdc);
        SelectObject(memDC, hBmp);
        BitBlt(hdc, 0, 0, winW, winH + 22,
            memDC, 0, 0, SRCCOPY);

        DeleteDC(memDC);
        DeleteObject(hBmp);
        HeapFree(GetProcessHeap(), 0, bmp);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_MOUSEMOVE: {
        char buf[64];
        sprintf_s(buf, "MOUSE: X=%d Y=%d",
            LOWORD(lParam), HIWORD(lParam));
        SendMessageA(hWndStatusBar,
            SB_SETTEXTA, 2, (LPARAM)buf);

        sprintf_s(buf, "PIXELS: X=%d Y=%d",
            LOWORD(lParam) / pixelSize,
            HIWORD(lParam) / pixelSize);
        SendMessageA(hWndStatusBar,
            SB_SETTEXTA, 1, (LPARAM)buf);

        sprintf_s(buf, "Dimension(F2/F3): %d",
            pixelSize);
        SendMessageA(hWndStatusBar,
            SB_SETTEXTA, 0, (LPARAM)buf);
        break;
    }

    case WM_LBUTTONDOWN:
        global_clicked_pixel.X =
            LOWORD(lParam) / pixelSize;
        global_clicked_pixel.Y =
            HIWORD(lParam) / pixelSize;
        InvalidateRect(hWnd, nullptr, FALSE);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_F2 && pixelSize > 1) --pixelSize;
        if (wParam == VK_F3 && pixelSize < 64) ++pixelSize;
        InvalidateRect(hWnd, nullptr, FALSE);
        break;

    case WM_TIMER:
        global_angle += 0.05f;
        InvalidateRect(hWnd, nullptr, FALSE);
        break;

    case WM_SIZE:
        SendMessageA(hWndStatusBar, WM_SIZE, 0, 0);
        InvalidateRect(hWnd, nullptr, FALSE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
