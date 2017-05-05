#include"stdafx.h"

#include"vertex.h"
void vertex_rhw_init(vertex *v) {
	float rhw = 1.0f / v->point.w;
	v->rhw = rhw;
}
void vertex_interp(vertex *y, const vertex *x1, const vertex *x2, float t) {
	vector_interp(&y->point, x1->point, x2->point,t );
	y->texcoord.u = point_interp(t,x1->texcoord.u, x2->texcoord.u );
	y->texcoord.v = point_interp(t,x1->texcoord.v, x2->texcoord.v );
	y->color.r = point_interp(t,x1->color.r, x2->color.r );
	y->color.g = point_interp(t,x1->color.g, x2->color.g );
	y->color.b = point_interp(t,x1->color.b, x2->color.b );
	y->rhw = point_interp(t,x1->rhw, x2->rhw );
}
void vertex_get_step(vertex *step, const vertex* x1, const vertex* x2, float width) {
	float inv = 1.0f / width;
	step->point.x = (x2->point.x - x1->point.x) * inv;
	step->point.y = (x2->point.y - x1->point.y) * inv;
	step->point.z = (x2->point.z - x1->point.z) * inv;
	step->point.w = (x2->point.w - x1->point.w) * inv;
	step->texcoord.u = (x2->texcoord.u - x1->texcoord.u) * inv;
	step->texcoord.v = (x2->texcoord.v - x1->texcoord.v) * inv;
	step->color.r = (x2->color.r - x1->color.r) * inv;
	step->color.g = (x2->color.g - x1->color.g) * inv;
	step->color.b = (x2->color.b - x1->color.b) * inv;
	step->rhw = (x2->rhw - x1->rhw) * inv;
}
void vertex_add_step(vertex *out, vertex *step) {
	out->point.x += step->point.x;
	out->point.y += step->point.y;
	out->point.z += step->point.z;
	out->point.w += step->point.w;
	out->rhw += step->rhw;
	out->texcoord.u += step->texcoord.u;
	out->texcoord.v += step->texcoord.v;
	out->color.r += step ->color.r;
	out->color.g += step->color.g;
	out->color.b += step->color.b;
}