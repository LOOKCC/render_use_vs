#include"stdafx.h"
#include<stdio.h>
#include<math.h>
#include"matrix.h"
#include"vector.h"

matrix addm(matrix a, matrix b) {
	matrix res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] = a.m[i][j] + b.m[i][j];
		}
	}
	return res;
}
matrix subm(matrix a, matrix b) {
	matrix res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] = a.m[i][j] - b.m[i][j];
		}
	}
	return res;
}
matrix mulnum(float f, matrix m) {
	matrix res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] = m.m[i][j] * f;
		}
	}
	return res;
}
matrix mulmatr(matrix a, matrix b) {
	matrix res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++) {
				res.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return res;
}
void set_identity(matrix *m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)   m->m[i][j] = 1.0f;
			else m->m[i][j] = 0.0f;
		}
	}
}
void set_zero(matrix *m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m->m[i][j] = 0.0f;
		}
	}
}
vector vmulmat(vector v, matrix m) {
	vector res;
	res.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	res.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	res.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	res.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
	return res;
}
void trans_mat(matrix *m, float x, float y, float z) {
	set_identity(m);
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}
void scale_mat(matrix *m, float x, float y, float z) {
	set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}
void totate_x_mat(matrix *m, float a) {
	set_zero(m);
	m->m[0][0] = m->m[3][3] = 1;
	m->m[1][1] = cos(a);
	m->m[2][2] = cos(a);
	m->m[1][2] = -sin(a);
	m->m[2][1] = sin(a);
}
void totate_y_mat(matrix *m, float a) {
	set_zero(m);
	m->m[1][1] = m->m[3][3] = 1;
	m->m[0][0] = cos(a);
	m->m[0][3] = sin(a);
	m->m[2][0] = -sin(a);
	m->m[2][2] = cos(a);
}
void totate_z_mat(matrix *m, float a) {
	set_zero(m);
	m->m[2][2] = m->m[3][3] = 1;
	m->m[0][0] = cos(a);
	m->m[0][1] = -sin(a);
	m->m[1][0] = sin(a);
	m->m[1][1] = cos(a);
}