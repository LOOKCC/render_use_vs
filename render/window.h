#pragma once
//������ֱ�ӳ��ģ��о�����һЩ����
int window_init(Window* window, int w, int h, const TCHAR* title, HINSTANCE histance);
int window_close(Window* window);
void window_present(Window* window);
void window_dispatch();
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LRESULT lParam);

