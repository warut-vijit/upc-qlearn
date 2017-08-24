#include "linalg.h"

double rand_gauss(double mean, double stddev) {
  double v1, v2, s;
  do{
    v1 = 2.0 * ((double)rand() / RAND_MAX) - 1;
    v2 = 2.0 * ((double)rand() / RAND_MAX) - 1;
    s = v1 * v1 + v2 * v2;
  } while (s >= 1.0);
  return v1 * sqrt(-2.0 * log(s) / s) * stddev + mean;
}

void vec_create(vec *v, int d1) {
  v->d1 = d1;
  v->data = (double *) malloc(d1 * sizeof(double));
}

void vec_create_gaussian(vec *v, int d1, double mean, double stddev) {
  v->d1 = d1;
  v->data = (double *) malloc(d1 * sizeof(double));
  int i;
  for(i = 0; i < d1; i++) v->data[i] = rand_gauss(mean, stddev);
}

void vec_delete(vec *v) {
  free(v->data);
}

void batch_vec_delete(int ct, ...) {
  va_list args;
  int i;
  va_start(args, ct);
  for(i = 0; i < ct; i++) {
    vec *v = va_arg(args, vec *);
    free(v->data);
  }
  va_end(args);
}

void mat_create(mat *m, int d1, int d2) {
  m->d1 = d1;
  m->d2 = d2;
  m->data = (double *) malloc(d1 * d2 * sizeof(double));
}

void mat_create_zeros(mat *m, int d1, int d2) {
  mat_create(m, d1, d2);
  int i, j;
  for(i = 0; i < d1; i++) {
    for(j = 0; j < d2; j++) {
      m->data[i*d2 + j] = 0;
    }
  }
}

void mat_create_gaussian(mat *m, int d1, int d2, double mean, double stddev) {
  mat_create(m, d1, d2);
  int i, j;
  for(i = 0; i < d1; i++) {
    for(j = 0; j < d2; j++) {
      m->data[i*d2 + j] = rand_gauss(mean, stddev);
    }
  }
}

void mat_delete(mat *m) {
  free(m->data);
}

void batch_mat_delete(int ct, ...) {
  va_list args;
  int i;
  va_start(args, ct);
  for(i = 0; i<ct; i++) {
    mat *m = va_arg(args, mat *);
    free(m->data);  
  }
}

void v_v_sub(vec *v1, vec *v2, vec *out) {
  if(v1->data == NULL || v2->data == NULL) printf("ERROR: v_v_sub argument vector has not been initialized\n");
  if(v1->d1 != v2->d1) printf("ERROR: v_v_sub argument vectors do not align\n");
  vec_create(out, v1->d1);
  int i;
  for(i = 0; i < v1->d1; i++) {
    out->data[i] = v1->data[i] - v2->data[i];
  }
}

void m_m_add_assign(mat *m1, mat *m2) {
  if(m1->d1 != m2->d1 || m1->d2 != m2->d2) printf("ERROR: m_m_add argument vectors do not align\n");
  int i;
  for(i = 0; i < m1->d1; i++) {
    m1->data[i] += m2->data[i];
  }
}

double v_v_inner(vec *v1, vec *v2) {
  if(v1->d1 != v2->d1) printf("ERROR: v_v_inner argument vectors do not align\n");
  int i;
  double out;
  for(i = 0; i < v1->d1; i++) {
    out += v1->data[i] * v2->data[i];
  }
  return out;
}

void v_v_outer(vec *v1, vec *v2, mat *out) {
  mat_create(out, v1->d1, v2->d1);
  int i, j;
  for(i = 0; i < v1->d1; i++) {
    for(j = 0; j < v2->d1; j++) {
      out->data[i*v2->d1+j] = v1->data[i] * v2->data[j];
    }
  }
}

void v_m_mult(vec *v, mat *m, vec *out) {
  // check matching dimensions
  if(v->d1 != m->d1) printf("ERROR: v_m_mult arguments do not align\n");
  vec_create(out, m->d2);
  int i, j;
  for(i = 0; i < out->d1; i++) {
    for(j = 0; j < v->d1; j++) {
      out->data[i] += m->data[j*m->d2 + i] * v->data[j];
    }
  }
}

void m_mult(mat *m, double d) {
  int i;
  for(i = 0; i < m->d1 * m->d2; i++) m->data[i] *= d;
}

void mat_transpose(mat *m, mat *out) {
  // check matching dimensions
  mat_create(out, m->d2, m->d1);
  int i, j;
  for(i = 0; i < m->d1; i++) {
    for(j = 0; j < m->d2; j++) {
      out->data[j*m->d1+i] = m->data[i*m->d2+j];
    }
  }
}

double v_to_scalar(vec *v) {
  return v->data[0];
}

void scalar_to_v(double val, vec *out) {
  vec_create(out, 1);
  out->data[0] = val;
}

void m_print(mat *m) {
  int i, j;
  printf("\n");
  for(i = 0; i < m->d1; i++) {
    for(j = 0; j < m->d2; j++) printf("%f ", m->data[i*m->d2+j]);
    printf("\n");
  }
}

void v_print(vec *v) {
  int i;
  printf("\n");
  for(i = 0; i < v->d1; i++) printf("%f ", v->data[i]);
  printf("\n");
}
