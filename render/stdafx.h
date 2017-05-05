#pragma once
//���е�typedef��struct
#include<Windows.h>
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
//window.h���õģ�Ӧ�ü���״̬
#define MRE_INDEX_NONE				-1
#define MRE_INIT_SUCCEED			0
#define MRE_TRUE					1
#define MRE_FALSE					0

#define RENDER_STATE_WIREFRAME 1  //��Ⱦ�߿�
#define RENDER_STATE_TEXTURE   2  //��Ⱦ����
#define RENDER_STATE_COLOR     4  //��Ⱦ��ɫ
#define RENDER_STATE_LIGHT     8  //��Ⱦ����
#define RENDER_STATE_VERTEX     16  //��Ⱦ����
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#define MIN(a,b) ( ((a)>(b)) ? (b):(a) )  
extern int CMID(int x, int min, int max);
float point_interp(float t, float x, float y); //��ֵ
typedef void(*AppTick)(float);

typedef unsigned int IUINT32; //Ϊ�˱��ֲ�ͬ���Ե��ֽ�����ͬ
//window
typedef struct {
	int width;
	int height;
	int keys[512];
	int exit;
	HWND handle;
	HDC context;
	unsigned char* screen_framebuf;

	HBITMAP dib_cur;
	HBITMAP dib_old;

}Window;
//vector
typedef struct { float x, y, z, w; } vector;
//matrix
typedef struct {
	float m[4][4];
}matrix;
//Texcoord
typedef struct {
	float u;
	float v;
}Texcoord;
//Color
typedef struct {
	float r, g, b;
}Color;
//Vertex
typedef struct {
	vector point;
	Texcoord texcoord;
	Color color;
	float rhw;
}vertex;
//transform
typedef struct {
	matrix world;
	matrix view;
	matrix projection;
	matrix transform_matrix;
	float width, height;
}transform;

//light
typedef struct {
	unsigned int light_type;
	Color color;
	vector direction;
	vector position;
}Light;
//device
typedef struct {
	transform t;
	int width;
	int height;
	IUINT32**  framebuffer;
	float** zbuffer;
	IUINT32** texture;
	int tex_width;
	int tex_height;
	float max_u; //��������� �� tex_width - 1;
	float max_v; //�������߶� �� tex_height - 1;
	int render_state;
	IUINT32 background;
	IUINT32 foreground;
	Light light;
}device;
typedef struct
{
	Window window;
	device dev;
	float fps;
	float frame_time_last;

	AppTick logic_tick;
	AppTick physics_tick;
	AppTick render_tick;

} Machine;
typedef struct {
	IUINT32 buffer[256][256];
	int width;
	int height;
}Texture;