#include "rendering.h"


//device 设置
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
	dev->render_state = RENDER_STATE_WIREFRAME;
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
//贴图相关
void device_set_texture(device *dev, void *bits, long pitch, int w, int h) {
	char* ptr = (char*)bits;
	assert(w <= 1024 && h <= 1024);
	for (int j = 0; j < h; ptr += pitch, j++) {
		dev->texture[j] = (IUINT32*)ptr;
	}
	dev->tex_width = w;
	dev->tex_height = h;
	dev->max_u = (float)(w - 1);
	dev->max_v = (float)(h - 1);
}

IUINT32 device_texture_read(const device *dev, float u, float v) {
	int x, y;
	u = u*dev->max_u;
	v = v*dev->max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = CMID(x, 0, dev->tex_width - 1);
	y = CMID(y, 0, dev->tex_height - 1);
	return dev->texture[y][x];
}

//渲染相关
void device_draw_pixel(device *dev, int x, int y, IUINT32 color) {
	if (((IUINT32)x) < (IUINT32)dev->width && ((IUINT32)y) < (IUINT32)dev->height)
		dev->framebuffer[y][x] = color;
}
//画线，Bresenham算法
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
//对三角形进行排序（按y值）v1-v2-v3，并调用扫描线函数进行画线
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
//画横线（扫描线）已知三角形
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
				device_color(dev, &svx, rhw, x, (int)y);
			}
		}
		vertex_add_step(&svx, &step);
		if (x >= width) break;

	}
}
//变到屏幕视角，原点在屏幕左上角
void device_transform_view(device *dev, vector* out, const vector *v) {
	out->x = (v->x + 1.0f) * dev->width * 0.5f;
	out->y = (1.0f-v->y) * dev->height * 0.5f;
	out->z = v->z;
	out->w = 1.0f;
}
//画三角形
void device_draw_primitive(device* dev, const vertex *v1,const vertex *v2,const vertex *v3) {
	vector p1, p2, p3, c1, c2, c3, a1, a2, a3;
	int render_state = dev->render_state;
	vertex t1=*v1, t2=*v2, t3=*v3;
	device_vertex(dev, &t1, v1);
	device_vertex(dev, &t2, v2);
	device_vertex(dev, &t3, v3);

	c1 = t1.point;
	c2 = t2.point;
	c3 = t3.point;
	//cvv裁剪
	if (transform_check_cvv(&c1) != 0) return;
	if (transform_check_cvv(&c2) != 0) return;
	if (transform_check_cvv(&c3) != 0) return;

	vector_rhw(&a1, &c1);
	vector_rhw(&a2, &c2);
	vector_rhw(&a3, &c3);

	device_transform_view(dev, &p1, &a1);
	device_transform_view(dev, &p2, &a2);
	device_transform_view(dev, &p3, &a3);

	if (render_state & RENDER_STATE_WIREFRAME) {
		device_draw_line(dev, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, dev->foreground);
		device_draw_line(dev, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, dev->foreground);
		device_draw_line(dev, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, dev->foreground);
	}

	if (render_state & RENDER_STATE_VERTEX) {
		device_draw_pixel(dev, (int)p1.x, (int)p1.y, dev->foreground);
		device_draw_pixel(dev, (int)p2.x, (int)p2.y, dev->foreground);
		device_draw_pixel(dev, (int)p3.x, (int)p3.y, dev->foreground);
	}

	if (render_state & (RENDER_STATE_COLOR | RENDER_STATE_TEXTURE)) {
		vertex t1, t2, t3;
		t1 = *v1;
		t2 = *v2;
		t3 = *v3;
		t1.point = p1;
		t2.point = p2;
		t3.point = p3;
		t1.point.w = c1.w;
		t2.point.w = c2.w;
		t3.point.w = c3.w;

		vertex_rhw_init(&t1);
		vertex_rhw_init(&t2);
		vertex_rhw_init(&t3);

		device_rasterize(dev, &t1, &t2, &t3);
	}
}

void dvice_set_light(device* dev, Light light) {
	dev->light = light;
}
//应用光照
vertex device_apply_light(device *dev, vertex v) {
	Light light = dev->light;
	vector light_direction;
	vector_add(&light_direction, v.point, light.position);
	vector_unitize(&light_direction);

	vector normal;
	vector_mul_matrix(&normal, v.nornmal, dev->t.world);
	normal.w = 0.0f;
	vector_unitize(&normal);
	float ndotl = vector_point_mul(normal, light_direction);
	ndotl = MAX(ndotl, 0);
	vertex out = v;
	transform_apply(&dev->t, &out.point, &v.point);
	if (dev->render_state == RENDER_STATE_COLOR) {
		out.color.r = ndotl*light.color.r*v.color.r + 0.1;
		out.color.g = ndotl*light.color.g*v.color.g + 0.1;
		out.color.b = ndotl*light.color.b*v.color.b + 0.1;
	}
	if (dev->render_state == RENDER_STATE_TEXTURE) {
		out.color.r = ndotl*light.color.r;
		out.color.g = ndotl*light.color.g;
		out.color.b = ndotl*light.color.b;
	}
	return out;
}

void device_vertex(device* dev, vertex *out, const vertex* v) {
	vertex temp = device_apply_light(dev, *v);
	out->point = temp.point;
	out->color = temp.color;
}
//应用颜色
void device_color(device* dev, const vertex *v, float rhw, int x, int y) {
	int render_state = dev->render_state;
	float w = 1.0f / rhw;
	if (render_state& RENDER_STATE_COLOR) {
		vertex vx = *v;
		float r = vx.color.r*w;
		float g = vx.color.g*w;
		float b = vx.color.b*w;
		IUINT32 temp = store_color(vx, dev);
		dev->framebuffer[y][x] = temp;
	}
	if (render_state& RENDER_STATE_TEXTURE) {
		vertex vx = *v;
		vx.texcoord.u = v->texcoord.u*w;
		vx.texcoord.v = v->texcoord.v*w;
		IUINT32 temp = store_color(vx, dev);
		dev->framebuffer[y][x] = temp;
	}
}
//((int)r << 16) | ((int)g << 8) | ((int)b << 0)使用位的方式储存颜色
IUINT32 store_color(vertex v, device *dev) {
	float r, g, b;
	IUINT32 color;
	if (dev->render_state == RENDER_STATE_COLOR) {
		r = (int)(v.color.r*255.0f);
		g = (int)(v.color.g*255.0f);
		b = (int)(v.color.b*255.0f);

		r = CMID(r, 0, 255);
		g = CMID(g, 0, 255);
		b = CMID(b, 0, 255);
	}
	else if (dev->render_state == RENDER_STATE_TEXTURE) {
		color = device_texture_read(dev, v.texcoord.u, v.texcoord.v);
		r = color >> 16 & 255;
		g = color >> 8 & 255;
		b = color >> 0 & 255;

		r = r*v.color.r;
		g = g*v.color.g;
		b = b*v.color.b;

		r = CMID(r, 0, 255);
		g = CMID(g, 0, 255);
		b = CMID(b, 0, 255);
	}
	color = ((int)r << 16) | ((int)g << 8) | ((int)b << 0);
	return color;
}