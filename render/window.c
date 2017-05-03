#include"window.h"
#include "machine.h"
int window_init(Window* window, int w, int h, const TCHAR* title, HINSTANCE hinstance) {
	window_close(window);

	WNDCLASSEX winclass;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = (WNDPROC)window_proc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszClassName = "winclass";
	winclass.lpszMenuName = NULL;

	RegisterClassEx(&winclass);

	window->handle = CreateWindowEx(
		NULL,
		"winclass",
		title,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0,
		w, h,
		NULL,
		NULL,
		hinstance,
		NULL
		);

	if (window->handle == NULL) {
		-1;
	}
	HDC hdc;
	window->exit = 0;
	hdc = GetDC(window->handle);
	window->context = CreateCompatibleDC(hdc);

	LPVOID ptr;
	BITMAPINFO bi = { {sizeof(BITMAPINFOHEADER),w ,- h ,1,32,BI_RGB,w*h * 4,0,0,0,0} };
	window->dib_cur = CreateDIBSection(window->context, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (window->dib_cur == NULL) {
		return -2;
	}

	window->dib_old = (HBITMAP)SelectObject(window->context, window->dib_cur);
	window->screen_framebuf = (unsigned char*)ptr;
	window->width = w;
	window->height = h;
	window->exit = MRE_FALSE;

	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	AdjustWindowRect(&rect, GetWindowLong(window->handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;

	SetWindowPos(window->handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(window->handle);
	ShowWindow(window->handle, SW_NORMAL);

	memset(window->keys, 0, sizeof(int) * 512);
	memset(window->screen_framebuf, 0, w * h * 4);
	return 0;
}
int window_close(Window* window)
{
	if (window->context)
	{
		if (window->dib_old)
		{
			SelectObject(window->context, window->dib_old);
			window->dib_old = NULL;
		}
		DeleteDC(window->context);
		window->context = NULL;
	}

	if (window->dib_cur)
	{
		DeleteObject(window->dib_cur);
		window->dib_cur = NULL;
	}

	if (window->handle)
	{
		CloseWindow(window->handle);
		window->handle = NULL;
	}
	return 0;
}

void window_present(Window* window)
{
	HDC hDC = GetDC(window->handle);
	BitBlt(hDC, 0, 0, window->width, window->height, window->context, 0, 0, SRCCOPY);
	ReleaseDC(window->handle, hDC);
}

void window_dispatch(void)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			machine.window.exit = MRE_TRUE;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK window_proc(HWND hWnd, UINT msg, WPARAM wParam, LRESULT lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		machine.window.keys[wParam & 511] = 1;
		break;
	}
	case WM_KEYUP:
	{
		machine.window.keys[wParam & 511] = 0;
		break;
	}
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
