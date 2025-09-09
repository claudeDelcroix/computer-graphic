#include <Windows.h>
#include <commctrl.h>
#include "stdio.h"
#include "Frame.h"
#include "Painter.h"

int pixelSize = 16; // Valeur par défaut, modifiable par F2/F3
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND hWndStatusBar;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInst, LPSTR IpszArgs, int nWinMode) {
	wchar_t szWinName[] = L"Graphics Window Class";
	HWND hWnd;
	MSG msg;
	WNDCLASS wcl;
	wcl.hInstance = hThisInstance;
	wcl.lpszClassName = szWinName;
	wcl.lpfnWndProc = WindowProc;
	wcl.style = 0;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	if (!RegisterClass(&wcl)) return 0;
	hWnd = CreateWindow(szWinName, L"Laboratoire 1 Lounou Claude Delcroix", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, hThisInstance, NULL);
	//je vais modifier cette ligne et remplacer les dimensions 544 et 375 par des variables CW_USEDEFAULT

	hWndStatusBar = CreateWindowExA(0, STATUSCLASSNAMEA, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, (HMENU)10001, hThisInstance, NULL);

	int statwidths[] = { 150,300,-1 };
	SendMessageA(hWndStatusBar, SB_SETPARTS, sizeof(statwidths) / sizeof(int), (LPARAM)statwidths);
	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, 1000 / 30, NULL);

	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect = ps.rcPaint;
		GetClientRect(hWnd, &rect);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		int ratio = pixelSize; // la variable pixelSize doit etre definie quelque part dans le code
		// Déclaration de la variable globale pixelSize en haut du fichier, avant toute utilisation

		int W = width / ratio;
		int H = (height - 22) / ratio;

		Frame frame(W, H); // la classe Frame doit etre definie quelque part dans le code
		Painter painter;
		painter.Draw(frame); // la classe Painter doit etre definie quelque part dans le code

		typedef struct tagRCBPIXEL {
			unsigned char BLUE;
			unsigned char GREEN;
			unsigned char RED;
			unsigned char ALPHA;
		} RGBPIXEL;

		RGBPIXEL* bitmap = (RGBPIXEL*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, width * height * sizeof(RGBPIXEL));
		for (int y = 0; y < H * ratio; y++) {
			for (int x = 0; x < W * ratio; x++) {
				RGBPIXEL* pixel = bitmap + (y * width + x);
				COLOR color = frame.GetPixel(x / ratio, y / ratio); // la methode GetPixel doit etre definie dans la classe Frame
				pixel->RED = color.RED;
				pixel->GREEN = color.GREEN;
				pixel->BLUE = color.BLUE;
				pixel->ALPHA = color.ALPHA;
			}
		}

		HBITMAP hBitmap = CreateBitmap(width, height, 1, sizeof(RGBPIXEL) * 8, bitmap);
		HDC srcHdc = CreateCompatibleDC(hdc);

		SelectObject(srcHdc, hBitmap);
		BitBlt(hdc, 0, 0, width, height, srcHdc, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);

		DeleteObject(hBitmap);
		DeleteDC(srcHdc);
	}
	break;

	case WM_MOUSEMOVE:
	{
		char str[256];
		sprintf_s(str, "Position de la souris: X=%d Y=%d", LOWORD(lParam), HIWORD(lParam));
		sprintf_s(str, "Coordonne du pixel: X=%d Y=%d", LOWORD(lParam) / pixelSize, HIWORD(lParam) / pixelSize);
		SendMessageA(hWndStatusBar, SB_SETTEXT, 0, (LPARAM)str); // je dois remplacer ici 0 par 1 pour que ce soit conforme a la doc

		sprintf_s(str, "Dimension(F2/F3): %d", pixelSize);
		SendMessageA(hWndStatusBar, SB_SETTEXT, 1, (LPARAM)str); // je dois remplacer ici 0 par 1 pour que ce soit conforme a la doc
	}
	break;

	case WM_LBUTTONDOWN:
	{
		global_clicked_pixel.X = LOWORD(lParam) / pixelSize; // la variable global_clicked_pixel doit etre definie quelque part dans le code
		global_clicked_pixel.Y = HIWORD(lParam) / pixelSize;
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	case WM_KEYDOWN:
		if (wParam == VK_F2 || wParam == VK_F3) {
			if (wParam == VK_F2 && pixelSize > 1) pixelSize--;
			if (wParam == VK_F3 && pixelSize < 64) pixelSize++;
			RECT rect; // cette partie n'existe pas dans le code original
			GetClientRect(hWnd, &rect); // cette partie n'existe pas dans le code original
			InvalidateRect(hWnd, &rect, FALSE);

			char str[256];
			sprintf_s(str, "Dimension(F2/F3): %d", pixelSize);
			SendMessageA(hWndStatusBar, SB_SETTEXT, 1, (LPARAM)str); // je dois remplacer ici 0 par 1 pour que ce soit conforme a la doc
		}
		if (wParam == VK_F1) {
			MessageBox(hWnd, L"Claude Delcroix Lounou did this work alone", L"about ", MB_ICONINFORMATION);
		}
		break;

	case WM_TIMER:
		global_angle += 0.05; // la variable global_angle doit etre definie quelque part dans le code
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
