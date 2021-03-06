#pragma once

#ifndef TRANSFORM_H_ 
#define TRANSFORM_H_ 
#include"vector.h"
#include "matrix.h"
#include"type.h"


void transform_update(transform *t);
void transform_init(transform *t, float width, float height);
void transform_apply(const transform *t, vector *y, const vector *x);
int transform_check_cvv(const vector *v);

#endif 

