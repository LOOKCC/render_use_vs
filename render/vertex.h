#pragma once

#ifndef VERTEX_H_
#define VERTEX_H_
#include "vector.h"
typedef struct {
	float u;
	float v;
}Texcoord;
typedef struct {
	float r, g, b;
}Color;
typedef struct {
	vector point;
	Texcoord texcoord;
	Color color;
	float rhw;
}vertex;
void vertex_rhw_init(vertex *v);
void vertex_interp(vertex *y, const vertex *x1, const vertex *x2, float t);
void vertex_get_step(vertex *step, const vertex* x1, const vertex* x2, float width);
void vertex_add_step(vertex *out, vertex *step);
#endif // !VERTEX_H_

