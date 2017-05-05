#include"stdafx.h"
#include<tchar.h>
#include<iostream>
#include<conio.h>
#include"machine.h"
#include"light.h"
#include"camera.h"
#include"texture.h"
HINSTANCE hInstance;
IUINT32 rgb(int r, int g, int b) {
	r = CMID(r, 0, 255);
	g = CMID(g, 0, 255);
	b = CMID(b, 0, 255);
	return (((int)r << 16) | ((int)g << 8) | ((int)b << 0));
}
void Init();
void tick(float time_delta);
matrix m;
float alpha = 1;
float posx = 5, posy = 0, posz = 0;
void main()
{
	hInstance = GetModuleHandle(NULL);
	machine_init(640, 480, _T("year"), hInstance);

	machine.dev.texture = texture_create_default();
	camera_at_zero(&machine.dev, posx, posy, posz);
	Init();
	machine_start(&tick);
	texture_destroy(machine.dev.texture);
}
void Init() {
	Color color = { 1,1,1 };
	vector position = { 3,3,3,1 };
	Light *light = light_create_point(color, position);
	machine.dev.light = *light;
	machine.dev.foreground = rgb(100, 255, 100);
	machine.dev.background = 0;
	machine.dev.render_state = RENDER_STATE_COLOR|RENDER_STATE_LIGHT;
}
void tick(float time_delta) {
	camera_at_zero(&machine.dev, posx, posy, posz);
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':alpha += 0.05f; break;
		case 'd':alpha -= 0.05f; break;
		case 's':posx += 0.2f; break;
		case 'w':posx -= 0.2f; break;
		case 'q':posy += 0.2f; break;
		case 'e':posy -= 0.2f; break;
		case 'z':posz += 0.2f; break;
		case 'c':posz -= 0.2f; break;
		case '1':machine.dev.render_state = RENDER_STATE_VERTEX; break;
		case '2':machine.dev.render_state = RENDER_STATE_WIREFRAME; break;
		case '3':machine.dev.render_state = RENDER_STATE_COLOR; break; 
		case '4':machine.dev.render_state = RENDER_STATE_LIGHT | RENDER_STATE_COLOR; break;
		case '5':machine.dev.render_state = RENDER_STATE_TEXTURE; break;


		}
	}
	//alpha -= 0.005f;
	matrix_set_rotate(&m, -1, -0.5, 1, alpha);
	machine.dev.t.world = m;
	transform_update(&machine.dev.t);
	device_draw_box(&machine.dev, 0, 0, 0);
	device_draw_box(&machine.dev, 0, 2, 0);
	device_draw_box(&machine.dev, 0, -2, 0);
	Sleep(1);
}
