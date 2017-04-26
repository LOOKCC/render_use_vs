#pragma once
#include"stdafx.h"
#ifndef MATRIX_H_
#define MATRIX_H_

#include"vector.h"
typedef struct {
	float m[4][4];
}matrix;

matrix addm(matrix a, matrix b);
matrix subm(matrix a, matrix b);
matrix mulnum(float f, matrix m);
matrix mulmatr(matrix a, matrix b);
void set_identity(matrix *m);
void set_zero(matrix *m);
vector vmulmat(vector v, matrix m);

void trans_mat(matrix *m, float x, float y, float z);
void scale_mat(matrix *m, float x, float y, float z);
void totate_x_mat(matrix *m, float a);
void totate_y_mat(matrix *m, float a);
void totate_z_mat(matrix *m, float a);



#endif