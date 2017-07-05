#include <matrix.h>

/* w = u + v */
void vector3d_add(const double *u, const double *v, double *w) {
    w[0] = u[0] + v[0];
    w[1] = u[1] + v[1];
    w[2] = u[2] + v[2];
}
/* u += v */
void vector3d_add_(double *u, const double *v) {
    u[0] += v[0];
    u[1] += v[1];
    u[2] += v[2];
}
/* w = u - v */
void vector3d_sub(const double *u, const double *v, double *w) {
    w[0] = u[0] - v[0];
    w[1] = u[1] - v[1];
    w[2] = u[2] - v[2];
}
/* u -= v*/
void vector3d_sub_(double *u, const double *v) {
    u[0] -= v[0];
    u[0] -= v[0];
    u[0] -= v[0];
}
/* u 点乘 v */
double vector3d_dmul(const double *u, const double *v) {
    return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}
/* u 叉乘 v */
void vector3d_xmul(const double * u, const double *v, double *w) {
    w[0] = u[1] * v[2] - u[2] * v[1];
    w[1] = u[2] * v[0] - u[0] * v[2];
    w[2] = u[0] * v[1] - u[1] * v[0];
}
/* v = cu */
void vector3d_mul(const double *u, double c, double *v) {
    v[0] = c * u[0];
    v[1] = c * u[1];
    v[2] = c * u[2];
}
/* u *= c */
void vector3d_mul_(double *u, double c) {
    u[0] *= c;
    u[1] *= c;
    u[2] *= c;
}
/* out = mv */
void matrix3d_lmul_vector(const double *m, const double *v, double *out) {
    out[0] = vector3d_dmul(&m[0], v);
    out[1] = vector3d_dmul(&m[3], v);
    out[2] = vector3d_dmul(&m[6], v);
}
/* m = m^T */
void matrix3d_transpose(double *m) {
    swap_double(&m[1 + 0 * 3], &m[0 + 1 * 3]);
    swap_double(&m[2 + 0 * 3], &m[0 + 2 * 3]);
    swap_double(&m[2 + 1 * 3], &m[1 + 2 * 3]);
}


/*
 * quat4f_to_matrix3f - 从四元数转换到旋转矩阵
 *
 * @q: 四元数
 * @m: 旋转矩阵
 */
void quat4f_to_matrix3f(const float *q, float *m) {
    float q1q1 = q[1] * q[1];
    float q2q2 = q[2] * q[2];
    float q3q3 = q[3] * q[3];

    float q0q3 = q[0] * q[3];
    float q0q2 = q[0] * q[2];
    float q0q1 = q[0] * q[1];
    float q1q2 = q[1] * q[2];
    float q1q3 = q[1] * q[3];
    float q2q3 = q[2] * q[3];

    Matrix3fCell(m, 0, 0) = 1 - 2 * (q2q2 + q3q3);
    Matrix3fCell(m, 0, 1) = 2 * (q1q2 - q0q3);
    Matrix3fCell(m, 0, 2) = 2 * (q1q3 + q0q2);
    Matrix3fCell(m, 1, 0) = 2 * (q1q2 + q0q3);
    Matrix3fCell(m, 1, 1) = 1 - 2 * (q1q1 + q3q3);
    Matrix3fCell(m, 1, 2) = 2 * (q2q3 - q0q1);
    Matrix3fCell(m, 2, 0) = 2 * (q1q3 - q0q2);
    Matrix3fCell(m, 2, 1) = 2 * (q2q3 + q0q1);
    Matrix3fCell(m, 2, 2) = 1 - 2 * (q1q1 + q2q2);
}


