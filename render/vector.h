#pragma once
#include"stdafx.h"

#ifndef VECTOR_H_
#define VECTOR_H_
#include"type.h"

float vector_length(vector v);
void vector_add(vector* out,const vector a,const vector b);
void vector_sub(vector* out,const vector a,const vector b);
float vector_point_mul(vector a, vector b);
void vector_cross_mul(vector*out,const vector a,const vector b);
void vector_unitize(vector *v);
void vector_interp(vector* out,const vector a, const vector b, float t);
void vector_rhw(vector *out, const vector *v);
#endif
