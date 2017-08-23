#include "pong.h"
#include <float.h>

#ifndef QLEARN_H
#define QLEARN_H

typedef struct _state {
  simulator sim;
  pair reward;
  int a_l, a_r;
  struct _state *next;
} state;

void state_delete(state *s) {
  if(s == NULL) return;
  free(s);
}

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
  vec_delete(&v);
  return action;
}

void collect(simulator *s, policy *p, state **frame_buffer) {
  int a_left, a_right;
  state *frame, *head;
  head = *frame_buffer;
  pair reward;
  while(!is_terminal(s)) {
    // compute action for players
    a_left = get_move(s, p, LEFT_PLAYER);
    a_right = get_move(s, p, RIGHT_PLAYER);
    reward = sim_step(s, a_left, a_right);
    // create new linked link node
    frame = (state *) malloc(sizeof(state));
    frame->sim = *s;
    frame->reward = reward;
    frame->a_l = a_left;
    frame->a_r = a_right;
    frame->next = head;
    head = frame;
  }
  *frame_buffer = head;
}

#endif
