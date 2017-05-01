#pragma once
#ifndef WINDOW_H_
#define WINDOW_H_
#include"type.h"

int window_init(Window* window, int w, int h, const TCHAR* title, HINSTANCE histance);
int window_close(Window* window);
void window_present(Window* window);
void window_dispatch();
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LRESULT lParam);

#endif

