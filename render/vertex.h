#pragma once
#include"vector.h"

void vertex_rhw_init(vertex *v);
void vertex_interp(vertex *y, const vertex *x1, const vertex *x2, float t);
void vertex_get_step(vertex *step, const vertex* x1, const vertex* x2, float width);
void vertex_add_step(vertex *out, vertex *step);

