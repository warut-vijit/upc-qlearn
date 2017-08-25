#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#ifndef LINALG_H
#define LINALG_H

// Gaussian random number generator
double rand_gauss(double mean, double stddev);

// Custom vector implementation
typedef struct {
  int d1;
  double *data;
} vec;

void vec_create(vec *v, int d1);
void vec_create_zeros(vec *v, int d1);
void vec_create_gaussian(vec *v, int d1, double mean, double stddev);
void vec_delete(vec *v);
void batch_vec_delete(int ct, ...);

// Custom matrix implementation
typedef struct {
  int d1, d2;
  double *data;
} mat;

void mat_create(mat *m, int d1, int d2);
void mat_create_zeros(mat *m, int d1, int d2);
void mat_create_gaussian(mat *m, int d1, int d2, double mean, double stddev);
void mat_delete(mat *m);
void batch_mat_delete(int ct, ...);

// Linear algebra methods

void v_v_add_assign(vec *v1, vec *v2);
void m_m_add_assign(mat *m1, mat *m2);
double v_v_inner(vec *v1, vec *v2);
void v_v_outer(vec *v1, vec *v2, mat *out);
void v_mult(vec *v, double d);
void m_mult(mat *m, double d);
void v_m_mult(vec *v, mat *m, vec *out);
void mat_transpose(mat *m, mat *out);
double v_to_scalar(vec *v);
void scalar_to_v(double val, vec *out);
void m_print(mat *m);
void v_print(vec *m);
double frobenius_norm(mat *m);

// performs matrix addition B := aA+B
void matrix_add(int m, int n, double a, double *A, double *B);
void v_sanity(vec v);
void m_sanity(mat m);
#endif
