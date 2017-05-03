#include"stdafx.h"
#include<tchar.h>
#include<iostream>
#include<conio.h>
#include"machine.h"
#include"light.h"
#include"camera.h"
HINSTANCE hInstance;
void Init();
void tick(float time_delta);
IUINT32 rgb(int r, int g, int b) {
	r = CMID(r, 0, 255);
	g = CMID(g, 0, 255);
	b = CMID(b, 0, 255);
	return (((int)r << 16) | ((int)g << 8) | ((int)b << 0));
}
vertex v1 = { { 10,1,10,1 },{ 1,1 },{ 255,0,0 },1,{ 1,1,1,1 } };
vertex v2 = { { 10,10,10,1 },{ 1,1 },{ 0,255,0 },1,{ 1,1,1,1 } };
vertex v3 = { { 10,10,10,1 },{ 1,1 },{ 0,0,255 },1,{ 1,1,1,1 } };
void main()
{
	hInstance = GetModuleHandle(NULL);
	machine_init(640, 480, _T("year"), hInstance);
	Init();
	machine_start(&tick);
}
void Init() {
	Color color = { 255,255,255 };
	vector position = { 0,0,0,1 };
	Light *light = light_create_point(color, position);
	machine.dev.light = *light;
	machine.dev.foreground = rgb(255, 100, 100);
	camera_at_zero(&machine.dev, -1, -1, -1);
}
void tick(float time_delta) {
	device_rasterize(&machine.dev, &v1, &v2, &v3);
	device_draw_primitive(&machine.dev, &v3, &v1, &v2);
	//device_draw_line(&machine.dev, 200, 200, 400, 400, rgb(255, 100, 100));
}
