#pragma once
#include"stdafx.h"

#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct { float x, y, z, w; } vector;
typedef vector point;
float interp(float t, float x, float y);
float length(vector v);
vector addv(vector a, vector b);
vector subv(vector a, vector b);
float pointmultiply(vector a, vector b);
vector crossmultiply(vector a, vector b);
void unitize(vector *v);
vector interp_vector(vector a, vector b, float t);
#endif
