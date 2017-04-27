#include"window.h"
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
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0,
		w, h,
		NULL,
		NULL,
		hinstance,
		NULL,
		);

	if (window->handle == NULL) {
		-1;
	}





}