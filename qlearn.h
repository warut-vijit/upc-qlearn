#include "pong.h"
#include <float.h>

#ifndef QLEARN_H
#define QLEARN_H

typedef struct {
  simulator state;
  int reward;
} state;

/*
 * Q function approximator using simple nn
 */
typedef struct {
  mat m1;
  mat m2;
} policy;

void policy_init(policy *p) {
  mat_create_gaussian(&p->m1, INPUT, HIDDEN, 0.0, sqrt(2.0/INPUT));
  mat_create_gaussian(&p->m2, HIDDEN, OUTPUT, 0.0, sqrt(2.0/INPUT));
}

double policy_fwd(policy *p, vec *v) {
  vec temp, out;
  vec_create(&temp, 32);
  vec_create(&out, 1);
  double val;
  v_m_mult(v, &p->m1, &temp);
  v_m_mult(&temp, &p->m2, &out);
  val = v_to_scalar(&out);
  vec_delete(&temp);
  vec_delete(&out);
  return val;
}

void init(simulator *s, policy *p) {
  sim_init(s);
  policy_init(p);
}

int get_move(simulator *s, policy *p, int pid) {
  vec v;
  int action = 0;
  double tmp, val = -1 * DBL_MAX;
  vec_create(&v, 5);
  sim_to_v(s, &v, pid);
  v.data[4] = -1;
  tmp = policy_fwd(p, &v);
  action = (tmp > val) ? -1 : action;
  val = fmax(tmp, val);
  v.data[4] = 0;
  tmp = policy_fwd(p, &v);
  action = (tmp > val) ? 0 : action;
  val = fmax(tmp, val);
  v.data[4] = 1;
  tmp = policy_fwd(p, &v);
  action = (tmp > val) ? 1 : action;
  val = fmax(tmp, val);
  printf("p:%d dir:%d q:%f\n", pid, action, val);
  vec_delete(&v);
  return action;
}

void step(simulator *s, policy *p) {
  // compute action for players
  int a_left = get_move(s, p, LEFT_PLAYER);
  int a_right = get_move(s, p, RIGHT_PLAYER);
  sim_step(s, a_left, a_right);
}

#endif
