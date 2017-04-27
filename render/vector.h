#pragma once
#include"stdafx.h"

#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct { float x, y, z, w; } vector;
float interp(float t, float x, float y);
float length(vector v);
void addv(vector* out,const vector a,const vector b);
void subv(vector* out,const vector a,const vector b);
float pointmultiply(vector a, vector b);
void crossmultiply(vector*out,const vector a,const vector b);
void unitize(vector *v);
void interp_vector(vector* out,const vector a, const vector b, float t);
#endif
