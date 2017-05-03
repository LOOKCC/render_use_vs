#include"stdafx.h"
#include"matrix.h"


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


/*
平移变换
*/
void transform_matrix(matrix *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}
/*
缩放变换
*/
void scale_matrix(matrix *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}
/*
关于x轴的对称
1    0     0     0
0    cos   -sin  0
0    sin   cos   0
0    0     0     1 
*/
void rotate_x_matrix(matrix *m, float a) {
	matrix_set_zero(m);
	m->m[0][0] = m->m[3][3] = 1;
	m->m[1][1] = cos(a);
	m->m[2][2] = cos(a);
	m->m[1][2] = -sin(a);
	m->m[2][1] = sin(a);
}
/*
关于y轴的旋转
cos     0      sin   0
0       1      0     0
-sin    0      cos   0
0       0      0     1
*/
void rotate_y_matrix(matrix *m, float a) {
	matrix_set_zero(m);
	m->m[1][1] = m->m[3][3] = 1;
	m->m[0][0] = cos(a);
	m->m[0][2] = sin(a);
	m->m[2][0] = -sin(a);
	m->m[2][2] = cos(a);
}

/*
关于z轴的旋转
cos       -sin       0        0
sin       cos        0        0
0          0         1        0
0		   0	     0        1
*/
void rotate_z_matrix(matrix *m, float a) {
	matrix_set_zero(m);
	m->m[2][2] = m->m[3][3] = 1;
	m->m[0][0] = cos(a);
	m->m[0][1] = -sin(a);
	m->m[1][0] = sin(a);
	m->m[1][1] = cos(a);
}
/*
cot fovy的半角余切
aspect 屏幕宽高比
f n 远近截面的距离
cot/aspect  0         0         0
0           cot       0         0
0           0         0         1
0           0         fn/(n-f)  0
*/
void matrix_perspective(matrix* out, float fovy, float aspect, float zn, float zf) {
	float cot = 1.0f / (float)tan(fovy * 0.5f);
	matrix_set_zero(out);
	out->m[0][0] = cot / aspect;
	out->m[1][1] = cot;
	out->m[2][2] = zf / (zf - zn);
	out->m[2][3] = 1.0f;
	out->m[3][2] = (zn * zf) / (zn - zf);
}
/*
设置摄像机
参数 eye眼睛位置 ，at看的位置 up向上向量
x.x     y.x    z.x     0
x.y     y.y    z.y     0
x.z     y.z    z.z     0   
-up*x   -up*y  -up*z   1
*/
void matrix_set_lookat(matrix *m, const vector *eye, const vector *at, const vector *up) {
    //先声明三个方向的向量
	vector xaxis, yaxis, zaxis;
	//通过运算求出三个方向的向量
	vector_sub(&zaxis, *at, *eye);
	vector_unitize(&zaxis);
	vector_cross_mul(&xaxis, *up, zaxis);
	vector_unitize(&xaxis);
	vector_cross_mul(&yaxis, *up, zaxis);

	//求解矩阵
	m->m[0][0] = xaxis.x;
	m->m[1][0] = xaxis.y;
	m->m[2][0] = xaxis.z;
	m->m[3][0] = -vector_point_mul(xaxis, *eye);
	
	m->m[0][1] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[2][1] = yaxis.z;
	m->m[3][1] = -vector_point_mul(yaxis, *eye);

	m->m[0][2] = zaxis.x;
	m->m[1][2] = zaxis.y;
	m->m[2][2] = zaxis.z;
	m->m[3][2] = -vector_point_mul(zaxis, *eye);

	m->m[0][3] = 0.0f;
	m->m[1][3] = 0.0f;
	m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}
