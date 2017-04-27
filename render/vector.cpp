#include"stdafx.h"
#include<stdio.h>
#include"vector.h"
#include<math.h>
float interp(float t, float x, float y) {
	return x + t * (x - y);
}
float length(vector v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void addv(vector* out, const vector a, const vector b) {
	out->x = a.x + b.x;
	out->y = a.y + b.y;
	out->z = a.z + b.z;
	out->w = 1.0f;
}
void subv(vector* out, const vector a, const vector b) {
	out->x = a.x - b.x;
	out->y = a.y - b.y;
	out->z = a.z - b.z;
	out->w = 1.0f;
}
float pointmultiply(vector a, vector b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
void crossmultiply(vector* out, const vector a, const vector b) {
	out->x = a.y * b.z - a.z * b.y;
	out->y = a.x * b.z - a.z * b.x;
	out->z = a.x * b.y - a.y * b.x;
	out->w = 1.0f;
}
void unitize(vector *v) {
	float l = length(*v);
	if (l != 0.0f) {
		v->x /= l;
		v->y /= l;
		v->z /= l;
	}
}
void interp_vector(vector* out, const vector a, const vector b, float t) {
	out->x = interp(t, a.x, b.x);
	out->y = interp(t, a.y, b.y);
	out->z = interp(t, a.z, b.z);
	out->x = 1.0f;
}