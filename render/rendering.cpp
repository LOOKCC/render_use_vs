#include "rendering.h"

void device_init(device* dev, int width, int height, void* fb) {
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char* ptr = (char*)malloc(need + 64);
	char* framebuf, *zbuf;
	assert(ptr);
	dev->framebuffer = (IUINT32**)ptr;
	dev->zbuffer = (float**)(ptr + sizeof(void*)*height);
	ptr += sizeof(void*)*height * 2;
	dev->texture = (IUINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width*height * 4;
	ptr += width*height * 8;
	if (fb != NULL) {
		framebuf = (char*)fb;
	}
	for (int j = 0; j < height; j++) {
		dev->framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
		dev->zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}
	dev->texture[0] = (IUINT32*)ptr;
	dev->texture[1] = (IUINT32*)(ptr + 16);
	memset(dev->texture[0], 0, 64);
	dev->tex_width = 2;
	dev->tex_height = 2;
	dev->max_u = 1.0f;
	dev->max_v = 1.0f;
	dev->width = width;
	dev->height = height;
	dev->background = 0xc0c0c0;
	dev->foreground = 0;
	transform_init(&dev->t, width, height);
	dev->render_state = RENDER_SATATE_WIREFRAME;
}
void device_destroy(device *dev) {
	if (dev->framebuffer) {
		free(dev->framebuffer);
	}
	dev->framebuffer = NULL;
	dev->zbuffer = NULL;
	dev->texture = NULL;
}
void device_clear(device *dev, int mode){
	int y, x, height = dev->height;
	for (y = 0; y < dev->height; y++) {
		IUINT32 *dst = dev->framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0)
			cc = dev->background;
		for (x = dev->width; x > 0; x--) 
			dst[0] = cc;
	}
	for (y = 0; y < dev->height; y++) {
		float *dst = dev->zbuffer[y];
		for (x = dev->width; x > 0; dst++, x--)
			dst[0] = 0.0f;
	}
}
void device_draw_pixel(device *dev, int x, int y, IUINT32 color) {
	if (((IUINT32)x) < (IUINT32)dev->width && ((IUINT32)y) < (IUINT32)dev->height)
		dev->framebuffer[y][x] = color;
}
void device_draw_line(device *dev, int x1, int y1, int x2, int y2, IUINT32 c) {
	int  x,y,rem = 0;
	if (x1 == x2 && y1 == y2) {
		device_draw_pixel(dev, x1, y1, c);
	}
	else if(x1 == x2){
		for (x = MIN(y1, y2); x < MAX(y1, y2); x++)
			device_draw_pixel(dev, x1, x, c);
	}else if (y1 == y2) {
		for (y = MIN(x1, x2); y < MAX(x1, x2); y++)
			device_draw_pixel(dev, y, y1, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) {
				x = x1, y = y1, x1 = x2, x2 = x, y1 = y2, y2 = y;
			}
			for (x = x1, y = y1; x < x2; x++) {
				device_draw_pixel(dev, x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					device_draw_pixel(dev, x, y, c);
				}
			}
			device_draw_pixel(dev, x2, y2, c);
		}
		else {
			if (y2 < y1) {
				x = x1, y = y1, x1 = x2, x2 = x, y1 = y2, y2 = y;
			}
			for (x = x1, y = y1; y < y2; y++) {
				device_draw_pixel(dev, x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					device_draw_pixel(dev, x, y, c);
				}
			}
			device_draw_pixel(dev, x2, y2, c);
		}
	}
}
//v1-v2-v3
void device_rasterize(device* dev, vertex* v1, vertex* v2, vertex *v3) {
	if (v1->point.y > v2->point.y) {
		vertex *temp = v2;
		v2 = v1;
		v1 = temp;
	}
	if (v2->point.y > v3->point.y) {
		vertex *temp = v2;
		v2 = v3;
		v3 = temp;
	}
	if (v1->point.y > v2->point.y) {
		vertex *temp = v2;
		v2 = v1;
		v1 = temp;
	}
	float slop12, slop13;
	if (v2->point.y - v1->point.y>0) {
		slop12 = (v2->point.x - v1->point.x) / (v2->point.y - v1->point.y);
	}
	else {
		slop12 = 0;
	}
	if (v3->point.y - v1->point.y>0) {
		slop12 = (v3->point.x - v1->point.x) / (v3->point.y - v1->point.y);
	}
	else {
		slop13 = 0;
	}
	if (slop12 > slop13) {
		for (int y = (int)(v1->point.y + 1.0f); y < (int)(v3->point.y + 1.0f); y++) {
			if (y < v2->point.y) {
				device_draw_scanline(dev,y+1.0f,v1,v3,v1,v2);
			}
			else {
				device_draw_scanline(dev, y + 1.0f, v1, v3, v2, v3);
			}
		}
	}
	else {
		for (int y = (int)(v1->point.y + 1.0f); y < (int)(v3->point.y + 1.0f); y++) {
			if (y < v2->point.y) {
				device_draw_scanline(dev, y + 1.0f, v1, v3, v1, v2);
			}
			else {
				device_draw_scanline(dev, y + 1.0f, v2, v3, v1, v3);
			}
		}
	}
}
void device_draw_scanline(device *dev, float y, vertex *va, vertex *vb, vertex *vc, vertex *vd) {
	float inter1 = (float)va->point.y != vb->point.y ? (y - va->point.y) / (vb->point.y - va->point.y) : 1;
	float inter2 = (float)vc->point.y != vd->point.y ? (y - vc->point.y) / (vd->point.y - vc->point.y) : 1;
	vertex svx, evx, step;
	vertex_interp(&svx, va, vb, inter1);
	vertex_interp(&evx, vc, vd, inter2);
	float scanlinewidth = evx.point.x - svx.point.x;
	vertex_get_step(&step, &svx, &evx, scanlinewidth);
	int sx = (int)(svx.point.x + 1.0f);
	int ex = (int)(evx.point.x + 1.0f);
	int width = dev->width;
	float* zbuffer = dev->zbuffer[(int)y];
	for (int x = sx; x < ex; x++) {
		if (x >= 0 && x < width) {
			float rhw = svx.rhw;
			if(rhw >=zbuffer[x]){
				zbuffer[x] = rhw;
				//  do something
			}
		}
		vertex_add_step(&svx, &step);
		if (x >= width) break;

	}
}