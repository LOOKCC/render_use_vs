#pragma once

#ifndef TRANSFORM_H_ 
#define TRANSFORM_H_ 
#include"vector.h"
#include "matrix.h"

typedef struct {
	matrix world;
	matrix view;
	matrix projection;
	matrix transform_matrix;
	float width, height;
}transform;

void transform_update(transform *t);
void transform_init(transform *t, float width, float height);
void transform_apply(const transform *t, vector *y, const vector *x);
#endif // !

