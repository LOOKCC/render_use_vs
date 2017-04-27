#pragma once

#ifndef MATRIX_H_
#define MATRIX_H_

#include"vector.h"
typedef struct {
	float m[4][4];
}matrix;

void matrix_add(matrix* out, const matrix a, const matrix b);
void matrix_sub(matrix *out, const matrix a, const matrix b);
void matrix_mul_f(float f, matrix *m);
void matrix_mul_matrix(matrix *out, const matrix a, const matrix b);
void matrix_set_identity(matrix *m);
void matrix_set_zero(matrix *m);
void vector_mul_matrix(vector* out, const vector v, const matrix m);
void matrix_mul_vector(vector* out, const vector v, const matrix m);

void transform_matrix(matrix *m, float x, float y, float z);
void scale_matrix(matrix *m, float x, float y, float z);
void rotate_x_matrix(matrix *m, float a);
void rotate_y_matrix(matrix *m, float a);
void rotate_z_matrix(matrix *m, float a);

void matrix_perspective(matrix* out, float fovy, float aspect, float zn, float zf);
void matrix_set_lookat(matrix *m, const vector *eye, const vector *at, const vector *up);

#endif