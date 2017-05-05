#include"transform.h"
#include"stdafx.h"
#include"matrix.h"

void transform_update(transform *t) {
	matrix m;
	matrix_mul_matrix(&m, t->world, t->view);
	matrix_mul_matrix(&(t->transform_matrix), m, t->projection);
}
void transform_init(transform *t, float width, float height) {
	float aspect = width / height;
	matrix_set_identity(&(t->world));
	matrix_set_identity(&(t->view));
	matrix_perspective(&(t->projection), 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	t->width = width;
	t->height = height;
	transform_update(t);
}
/*
将矢量x进行project变换
*/
void transform_apply(const transform *t, vector *y, const vector *x) {
	vector_mul_matrix(y, *x, t->transform_matrix);
}
/*
检查齐次坐标同 cvv 的边界用于视锥剪裁
不懂
*/
int transform_check_cvv(const vector *v) {
	float w = v->w;
	int check = 0;
	if (v->z < 0.0f) check |= 1;
	if (v->z >  w) check |= 2;
	if (v->x < -w) check |= 4;
	if (v->x >  w) check |= 8;
	if (v->y < -w) check |= 16;
	if (v->y >  w) check |= 32;
	return check;
}
//单位化


