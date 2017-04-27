#include"transform.h"

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
void transform_apply(const transform *t, vector *y, const vector *x) {
	vector_mul_matrix(y, *x, t->transform_matrix);
}
