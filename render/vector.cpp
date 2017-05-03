#include"stdafx.h"

#include"vector.h"
#include<math.h>
/*
（线段）插值
*/
float point_interp(float t, float x, float y) {
	return x + t * (x - y);
}
/*
求模
*/
float vector_length(vector v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void vector_add(vector* out, const vector a, const vector b) {
	out->x = a.x + b.x;
	out->y = a.y + b.y;
	out->z = a.z + b.z;
	out->w = 1.0f;
}
void vector_sub(vector* out, const vector a, const vector b) {
	out->x = a.x - b.x;
	out->y = a.y - b.y;
	out->z = a.z - b.z;
	out->w = 1.0f;
}
/*
点乘
*/
float vector_point_mul(vector a, vector b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
/*
X乘
*/
void vector_cross_mul(vector* out, const vector a, const vector b) {
	out->x = a.y * b.z - a.z * b.y;
	out->y = a.x * b.z - a.z * b.x;
	out->z = a.x * b.y - a.y * b.x;
	out->w = 1.0f;
}
/*
单位化
*/
void vector_unitize(vector *v) {
	float l = vector_length(*v);
	if (l != 0.0f) {
		v->x /= l;
		v->y /= l;
		v->z /= l;
	}
}
/*
（向量）插值
*/
void vector_interp(vector* out, const vector a, const vector b, float t) {
	out->x = point_interp(t, a.x, b.x);
	out->y = point_interp(t, a.y, b.y);
	out->z = point_interp(t, a.z, b.z);
	out->x = 1.0f;
}
void vector_rhw(vector *out, const vector *v){
	float rhw = 1.0f / v->w;
	out->x = v->x * rhw;
	out->y = v->y * rhw;
	out->z = v->z * rhw;
	out->w = 1.0f;
}