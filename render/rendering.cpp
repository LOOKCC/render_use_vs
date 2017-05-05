#include"stdafx.h"
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
	//dev->texture[0] = (IUINT32*)ptr;
	//dev->texture[1] = (IUINT32*)(ptr + 16);
	//memset(dev->texture[0], 0, 32);
	dev->tex_width = 256;
	dev->tex_height = 256;
	dev->max_u = 255;
	dev->max_v = 255;
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
		for (x = dev->width; x > 0;dst++, x--) 
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
	x = CMID(x, 0, dev->width - 1);
	y = CMID(y, 0, dev->height - 1);
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
	char isfront = 1;
	if (v1->point.y > v2->point.y) {
		vertex *temp = v2;
		v2 = v1;
		v1 = temp;
		isfront = !isfront;
	}
	if (v2->point.y > v3->point.y) {
		vertex *temp = v2;
		v2 = v3;
		v3 = temp;
		isfront = !isfront;
	}
	if (v1->point.y > v2->point.y) {
		vertex *temp = v2;
		v2 = v1;
		v1 = temp;
		isfront = !isfront;
	}
	float slop12, slop13;
	char flag = 0;
	if (v2->point.y - v1->point.y>0) {
		slop12 = (v2->point.x - v1->point.x) / (v2->point.y - v1->point.y);
	}
	else {
		slop12 = 0;

		if (v2->point.x > v1->point.x)flag = 1;
		else flag = -1;
	}
	if (v3->point.y - v1->point.y>0) {
		slop13 = (v3->point.x - v1->point.x) / (v3->point.y - v1->point.y);
	}
	else {
		slop13 = 0;
	}
	if ((slop12 > slop13 || flag==1)&&flag!=-1&&isfront) {
		for (int y = (int)(v1->point.y); y < (int)(v3->point.y); y++) {
			if (y < v2->point.y-1) {
				device_draw_scanline(dev, y + 1.0f, v1, v3, v1, v2);
			}
			else {
				device_draw_scanline(dev, y + 1.0f, v1, v3, v2, v3);
			}
		}
	}
	else {
		if (!isfront) {
			for (int y = (int)(v1->point.y); y < (int)(v3->point.y); y++) {
				if (y < v2->point.y - 1) {
					device_draw_scanline(dev, y + 1.0f, v1, v2, v1, v3);
				}
				else {
					device_draw_scanline(dev, y + 1.0f, v2, v3, v1, v3);
				}
			}
		}
	}
}
//画横线（扫描线）已知三角形
void device_draw_scanline(device *dev, float y, vertex *va, vertex *vb, vertex *vc, vertex *vd) {
	vertex svx, evx, step;


	float tempy1 = (vb->point.y - va->point.y);
	float tempy2 = (vd->point.y - vc->point.y);
	if (tempy1 <= 1.5f&&tempy1 >= -1.5f) {
		//svx = va->point.x < vb->point.x ? *va : *vb;
		//evx = va->point.x < vb->point.x ? *vb : *va;
		return;
	}
	else if (tempy2 <= 1.5f&&tempy2 >= -1.5f) {
		//svx = vc->point.x < vd->point.x ? *vc : *vd;
		//evx = vc->point.x < vd->point.x ? *vd : *vc;
		return;
	}
	else {
		float inter1 = (y - va->point.y) / tempy1;
		float inter2 = (y - vc->point.y) / tempy2;
		vertex_interp(&svx, va, vb, inter1);
		vertex_interp(&evx, vc, vd, inter2);
	}

	float scanlinewidth = evx.point.x - svx.point.x;
	vertex_get_step(&step, &svx, &evx, scanlinewidth);
	int sx = svx.point.x+1;
	int ex = evx.point.x+1;
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
	if (render_state & RENDER_STATE_LIGHT) {
		vector v12, v13, normal;
		vector_sub(&v12, v1->point, v2->point);
		vector_sub(&v13, v1->point, v3->point);
		vector_cross_mul(&normal, v12, v13);
		vector_unitize(&normal);
		device_apply_light(dev, v1, &t1, normal);
		device_apply_light(dev, v2, &t2, normal);
		device_apply_light(dev, v3, &t3, normal);
	}

	transform_apply(&dev->t, &t1.point, &v1->point);
	transform_apply(&dev->t, &t2.point, &v2->point);
	transform_apply(&dev->t, &t3.point, &v3->point);
	

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
void device_draw_box(device* dev,float x, float y, float z) {
	vertex v1 = { { x + 0.5f,y + 0.5f,z + 0.5f,1 },{ 1,1 },{ 1,0,0 },1 };
	vertex v2 = { { x + 0.5f,y + 0.5f,z - 0.5f,1 },{ 0,1 },{ 0,0,1 },1 };
	vertex v3 = { { x + 0.5f,y - 0.5f,z + 0.5f,1 },{ 1,0 },{ 0,0,1 },1 };
	vertex v4 = { { x + 0.5f,y - 0.5f,z - 0.5f,1 },{ 1,1 },{ 1,0,0 },1 };
	vertex v5 = { { x - 0.5f,y + 0.5f,z + 0.5f,1 },{ 1,0 },{ 0,0,1 },1 };
	vertex v6 = { { x - 0.5f,y + 0.5f,z - 0.5f,1 },{ 1,1 },{ 1,0,0 },1 };
	vertex v7 = { { x - 0.5f,y - 0.5f,z + 0.5f,1 },{ 0,0 },{ 1,0,0 },1 };
	vertex v8 = { { x - 0.5f,y - 0.5f,z - 0.5f,1 },{ 1,0 },{ 0,0,1 },1 };
	device_draw_primitive(dev, &v1, &v2, &v3);
	device_draw_primitive(dev, &v4, &v3, &v2);
	device_draw_primitive(dev, &v5, &v7, &v6);
	device_draw_primitive(dev, &v8, &v6, &v7);
	device_draw_primitive(dev, &v1, &v5, &v2);
	device_draw_primitive(dev, &v6, &v2, &v5);
	device_draw_primitive(dev, &v3, &v4, &v7);
	device_draw_primitive(dev, &v8, &v7, &v4);
	device_draw_primitive(dev, &v3, &v7, &v1);
	device_draw_primitive(dev, &v5, &v1, &v7);
	device_draw_primitive(dev, &v6, &v8, &v2);
	device_draw_primitive(dev, &v4, &v2, &v8);
}
void dvice_set_light(device* dev, Light light) {
	dev->light = light;
}
//应用光照
void device_apply_light(device *dev,const vertex *v,vertex *out, vector &normal) {
	Light light = dev->light;
	vector light_direction;
	vector_sub(&light_direction, v->point, light.position);
	vector_unitize(&light_direction);
	normal.w = 0.0f;
	vector_unitize(&normal);
	float ndotl = vector_point_mul(normal, light_direction);
	ndotl = MAX(ndotl, 0);
	
	
	if (dev->render_state & RENDER_STATE_COLOR) {
		out->color.r = (ndotl + 0.1)*light.color.r*v->color.r + 0.1f;
		out->color.g = (ndotl + 0.1)*light.color.g*v->color.g + 0.1f;
		out->color.b = (ndotl + 0.1)*light.color.b*v->color.b + 0.1f;
	}
	if (dev->render_state & RENDER_STATE_TEXTURE) {
		out->color.r = ndotl*light.color.r;
		out->color.g = ndotl*light.color.g;
		out->color.b = ndotl*light.color.b;
	}
}

//应用颜色
void device_color(device* dev, const vertex *v, float rhw, int x, int y) {
	int render_state = dev->render_state;
	if (render_state& RENDER_STATE_COLOR) {
		vertex vx = *v;
		if (render_state&RENDER_STATE_LIGHT) {
			vector disVector;
			vector light;
			vector t1;
			transform_apply(&dev->t, &light, &dev->light.position);
			vector_rhw(&t1, &light);
			device_transform_view(dev, &light, &t1);
			device_draw_pixel(dev, light.x, light.y, RGB(255, 255, 255));
			vector_sub(&disVector, v->point, light);
			disVector.x /= dev->width;
			disVector.y /= dev->height;
			float dis=vector_length(disVector);
			float Fatt = 1.0f / (1.0f + 1*dis + 2.5f*dis*dis)+0.1f;
			vx.color.r *= Fatt;
			vx.color.g *= Fatt;
			vx.color.b *= Fatt;
		}
		IUINT32 temp = store_color(vx, dev);
		dev->framebuffer[y][x] = temp;
	}
	if (render_state& RENDER_STATE_TEXTURE) {
		vertex vx = *v;
		vx.texcoord.u = v->texcoord.u;
		vx.texcoord.v = v->texcoord.v;
		IUINT32 temp = store_color(vx, dev);
		dev->framebuffer[y][x] = temp;
	}
}
//((int)r << 16) | ((int)g << 8) | ((int)b << 0)使用位的方式储存颜色
IUINT32 store_color(vertex v, device *dev) {
	float r, g, b;
	IUINT32 color;
	if (dev->render_state & RENDER_STATE_COLOR) {
		r = (int)(v.color.r*255.0f);
		g = (int)(v.color.g*255.0f);
		b = (int)(v.color.b*255.0f);

		r = CMID(r, 0, 255);
		g = CMID(g, 0, 255);
		b = CMID(b, 0, 255);
	}
	else if (dev->render_state & RENDER_STATE_TEXTURE) {
		color = device_texture_read(dev, v.texcoord.u, v.texcoord.v);
		r = color >> 16 & 255;
		g = color >> 8 & 255;
		b = color >> 0 & 255;

	//	r = r*v.color.r;
	//	g = g*v.color.g;
	//	b = b*v.color.b;

		r = CMID(r, 0, 255);
		g = CMID(g, 0, 255);
		b = CMID(b, 0, 255);
	}
	color = ((int)r << 16) | ((int)g << 8) | ((int)b << 0);
	return color;
}