#pragma once

#include <utils.h>

#define MatrixCell(m, nrow, column, row)  ((m)[(column) * (nrow) + (row)])
#define Matrix3fCell(m, column, row)    MatrixCell(m, 3, column, row)

/* w = u + v */
void vector3d_add(const double *u, const double *v, double *w);
/* u += v */
void vector3d_add_(double *u, const double *v);
/* w = u - v */
void vector3d_sub(const double *u, const double *v, double *w);
/* u -= v*/
void vector3d_sub_(double *u, const double *v);
/* u 点乘 v */
double vector3d_dmul(const double *u, const double *v);
/* u 叉乘 v */
void vector3d_xmul(const double * u, const double *v, double *w);
/* v = cu */
void vector3d_mul(const double *u, double c, double *v);
/* u *= c */
void vector3d_mul_(double *u, double c);
/* out = mv */
void matrix3d_lmul_vector(const double *m, const double *v, double *out);
/* m = m^T */
void matrix3d_transpose(double *m);

/*
* quat4f_to_matrix3f - 从四元数转换到旋转矩阵
*
* @q: 四元数
* @m: 旋转矩阵
*/
void quat4f_to_matrix3f(const float *q, float *m)
