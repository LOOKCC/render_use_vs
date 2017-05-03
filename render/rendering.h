#pragma once

#include"vector.h"
#include"matrix.h"
#include"transform.h"
#include"vertex.h"

#include "light.h"

void device_init(device* dev, int width, int height, void* fb);
void device_destroy(device *d);
void device_clear(device *dev, int mode);

void device_draw_pixel(device *dev, int x, int y, IUINT32 color);
void device_draw_line(device *dev, int x1, int y1, int x2, int y2, IUINT32 c);
void device_draw_scanline(device *dev, float y, vertex *va, vertex *vb, vertex *vc, vertex *vd);

void dvice_set_light(device* dev, Light light);
vertex device_apply_light(device *dev, vertex v);
void device_set_texture(device *dev, void *bits, long pitch, int w, int h);
IUINT32 device_texture_read(const device *dev, float u, float v);

void device_rasterize(device* dev, vertex* v1, vertex* v2, vertex *v3);
void device_draw_primitive(device* dev, const vertex *v1, vertex *v2, vertex *v3);
void device_vertex(device* dev, vertex *out, const vertex* v);
void device_transform_view(device *dev, vector* out, const vector *v);
void device_color(device* dev, const vertex *v, float rhw, int x, int y);
IUINT32 store_color(vertex v, device *dev);

