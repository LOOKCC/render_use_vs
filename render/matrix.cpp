#include<stdio.h>
#include<math.h>
#include"matrix.h"
#include"vector.h"

void matrix_add(matrix* out, const matrix a, const matrix b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out->m[i][j] = a.m[i][j] + b.m[i][j];
		}
	}
}
void matrix_sub(matrix *out, const matrix a, const matrix b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out->m[i][j] = a.m[i][j] - b.m[i][j];
		}
	}
}
void matrix_mul_f(float f, matrix *m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m->m[i][j] *= f;
		}
	}
}
void matrix_mul_matrix(matrix *out, const matrix a,const matrix b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out->m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++) {
				out->m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
}
void matrix_set_identity(matrix *m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)   m->m[i][j] = 1.0f;
			else m->m[i][j] = 0.0f;
		}
	}
}
void matrix_set_zero(matrix *m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m->m[i][j] = 0.0f;
		}
	}
}
void vector_mul_matrix(vector* out, const vector v, const matrix m) {
	out->x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	out->y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	out->z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	out->w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
}
void matrix_mul_vector(vector* out, const vector v, const matrix m) {
	out->x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3];
	out->y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3];
	out->z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3];
	out->w = v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3];
}



void transform_matrix(matrix *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}
void scale_matrix(matrix *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}
void rotate_x_matrix(matrix *m, float a) {
	matrix_set_zero(m);
	m->m[0][0] = m->m[3][3] = 1;
	m->m[1][1] = cos(a);
	m->m[2][2] = cos(a);
	m->m[1][2] = -sin(a);
	m->m[2][1] = sin(a);
}
void rotate_y_matrix(matrix *m, float a) {
	matrix_set_zero(m);
	m->m[1][1] = m->m[3][3] = 1;
	m->m[0][0] = cos(a);
	m->m[0][3] = sin(a);
	m->m[2][0] = -sin(a);
	m->m[2][2] = cos(a);
}
void rotate_z_matrix(matrix *m, float a) {
	matrix_set_zero(m);
	m->m[2][2] = m->m[3][3] = 1;
	m->m[0][0] = cos(a);
	m->m[0][1] = -sin(a);
	m->m[1][0] = sin(a);
	m->m[1][1] = cos(a);
}
/*



*/





void matrix_persoective(matrix* out, float fovy, float aspect, float zn, float zf) {
	float cot = 1.0f / (float)tan(fovy * 0.5f);
	matrix_set_zero(out);


}