#pragma once

#ifndef RENDERING_H_
#define RENDERING_H_

#include<stdlib.h>
#include<assert.h>
#include"vector.h"
#include"matrix.h"
#include"transform.h"
#include"vertex.h"

#define RENDER_SATATE_WIREFRAME 1
#define RENDER_SATATE_TEXTURE   2
#define RENDER_SATATE_COLOR     4
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#define MIN(a,b) ( ((a)>(b)) ? (b):(a) )  

typedef unsigned int IUINT32;
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
}device;

void device_init(device* dev, int width, int height, void* fb);
void device_destroy(device *d);
void device_set_texture(device *dev, void *bits, long pitch, int w, int h); //未实现
void device_clear(device *dev, int mode);
void device_draw_pixel(device *dev, int x, int y, IUINT32 color);
void device_draw_line(device *dev, int x1, int y1, int x2, int y2, IUINT32 c);
IUINT32 device_texture_read(const device *dev, float u, float v);//未实现
void device_rasterize(device* dev, vertex* v1, vertex* v2, vertex *v3);
void device_draw_scanline(device *dev, float y, vertex *va, vertex *vb, vertex *vc, vertex *vd);

#endif // 

