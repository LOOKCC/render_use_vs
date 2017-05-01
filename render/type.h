#pragma once
#ifndef TYPE_H_
#define TYPE_H_
#include<Windows.h>
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#define MRE_INDEX_NONE				-1
#define MRE_INIT_SUCCEED			0
#define MRE_TRUE					1
#define MRE_FALSE					0
#define RENDER_STATE_WIREFRAME 1  //渲染线框
#define RENDER_STATE_TEXTURE   2  //渲染纹理
#define RENDER_STATE_COLOR     4  //渲染颜色
#define RENDER_STATE_VERTEX     8  //渲染顶点
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#define MIN(a,b) ( ((a)>(b)) ? (b):(a) )  
int CMID(int x, int min, int max) { return(x < min) ? min : ((x > max) ? max : x); }
float point_interp(float t, float x, float y);
typedef unsigned int IUINT32;
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
	vector nornmal;
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
	float max_u; //纹理最大宽度 ： tex_width - 1;
	float max_v; //纹理最大高度 ： tex_height - 1;
	int render_state;
	IUINT32 background;
	IUINT32 foreground;
	Light light;
}device;


#endif // !TYPE_H_
