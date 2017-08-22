#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
void vec_create_gaussian(vec *v, int d1, double mean, double stddev);
void vec_delete(vec *v);

// Custom matrix implementation
typedef struct {
  int d1, d2;
  double *data;
} mat;

void mat_create(mat *m, int d1, int d2);
void mat_create_gaussian(mat *m, int d1, int d2, double mean, double stddev);
void mat_delete(mat *m);

// Linear algebra methods

void v_v_sub(vec *v1, vec *v2, vec *out);
double v_v_inner(vec *v1, vec *v2);
void v_v_outer(vec *v1, vec *v2, mat *out);
void v_m_mult(vec *v, mat *m, vec *out);
void mat_transpose(mat *m, mat *out);
double v_to_scalar(vec *v);
void scalar_to_v(double val, vec *out);
#endif
