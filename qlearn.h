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
  mat_create_gaussian(&p->m2, HIDDEN, OUTPUT, 0.0, sqrt(2.0/(INPUT * HIDDEN)));
}

void policy_delete(policy *p) {
  mat_delete(&p->m1);
  mat_delete(&p->m2);
}

double policy_fwd(policy *p, vec *v) {
  vec temp, out;
  double val;
  v_m_mult(v, &p->m1, &temp);
  v_m_mult(&temp, &p->m2, &out);
  val = v_to_scalar(&out);
  vec_delete(&temp);
  vec_delete(&out);
  return val;
}

void policy_backprop(policy *p, state *frame_buffer) {
  double targ_l = 0, targ_r = 0;
  int examples = 0;
  state *frame_iter = frame_buffer;
  mat m1_d, m2_d;
  mat_create_zeros(&m1_d, INPUT, HIDDEN);
  mat_create_zeros(&m2_d, HIDDEN, OUTPUT);
  mat m1_T, m2_T;
  mat_transpose(&p->m1, &m1_T); 
  mat_transpose(&p->m2, &m2_T); 
  
  // iterate through history
  while(frame_iter != NULL) {
    examples ++;
    vec i, h, o;
    vec dEdh, dEdo;
    mat m1_del, m2_del;
    // LEFT PLAYER forward pass, get input, hidden, out vectors
    sim_to_v(&frame_iter->sim, &i, LEFT_PLAYER);
    i.data[6] = frame_iter->a_l;
    v_m_mult(&i, &p->m1, &h);
    v_m_mult(&h, &p->m2, &o);

    // backpropagate gradients
    targ_l *= DISCOUNTING;
    targ_l += frame_iter->reward.first;
    scalar_to_v(targ_l - v_to_scalar(&o), &dEdo);
    v_v_outer(&h, &dEdo, &m2_del);
    v_m_mult(&dEdo, &m2_T, &dEdh);
    v_v_outer(&i, &dEdh, &m1_del);
    
    // accumulate gradients
    m_m_add_assign(&m1_d, &m1_del);
    m_m_add_assign(&m2_d, &m2_del);
    
    // free allocated memory
    batch_vec_delete(5, &i, &h, &o, &dEdh, &dEdo);

    // RIGHT PLAYER forward pass, get input, hidden, out vectors
    sim_to_v(&frame_iter->sim, &i, RIGHT_PLAYER);
    //printf("m1: "); m_print(&p->m1);
    //printf("m2: "); m_print(&p->m2);

    i.data[6] = frame_iter->a_r;
    v_m_mult(&i, &p->m1, &h);
    v_m_mult(&h, &p->m2, &o);
    //printf("i: "); v_print(&i);
    //printf("h: "); v_print(&h);
    //printf("o: "); v_print(&o);

    // backpropagate gradients
    targ_r *= DISCOUNTING;
    targ_r += frame_iter->reward.first;
    printf("targ: %f o: %f\n", targ_r, v_to_scalar(&o));
    scalar_to_v(targ_r - v_to_scalar(&o), &dEdo);
    //printf("dEdo: "); v_print(&dEdo);
    v_v_outer(&h, &dEdo, &m2_del);
    v_m_mult(&dEdo, &m2_T, &dEdh);
    //printf("dEdh: "); v_print(&dEdh);
    v_v_outer(&i, &dEdh, &m1_del);
    
    // accumulate gradients
    //printf("m1_del: "); m_print(&m1_del);
    m_m_add_assign(&m1_d, &m1_del);
    //printf("m1_d: "); m_print(&m1_d);
    //printf("m2_del: "); m_print(&m2_del);
    m_m_add_assign(&m2_d, &m2_del);
    //printf("m2_d: "); m_print(&m2_d);
    
    // free allocated memory
    batch_vec_delete(5, &i, &h, &o, &dEdh, &dEdo);

    frame_iter = frame_iter->next;
  }

  // apply gradient update
  m_mult(&m1_d, LEARNING_RATE / examples);
  m_mult(&m2_d, LEARNING_RATE / examples);
  m_m_add_assign(&p->m1, &m1_d);
  m_m_add_assign(&p->m2, &m2_d);

  // free allocated memory
  batch_mat_delete(4, &m1_d, &m2_d, &m1_T, &m2_T);
}

int get_move(simulator *s, policy *p, int pid) {
  vec v;
  int action = 0;
  double tmp, val = -1 * DBL_MAX;
  // epsilon-greedy policy
  if((double) rand() / RAND_MAX < EPSILON) {
    printf("used epsilon policy\n");
    action = rand() % 3 - 1;
  }
  sim_to_v(s, &v, pid);
  v.data[6] = -1;
  tmp = policy_fwd(p, &v);
  action = (tmp > val) ? -1 : action;
  val = fmax(tmp, val);
  v.data[6] = 0;
  tmp = policy_fwd(p, &v);
  action = (tmp > val) ? 0 : action;
  val = fmax(tmp, val);
  v.data[6] = 1;
  tmp = policy_fwd(p, &v);
  action = (tmp > val) ? 1 : action;
  val = fmax(tmp, val);
  vec_delete(&v);
  return action;
}

void collect(simulator *s, policy *p, state **frame_buffer) {
  int i = 0;
  int a_left, a_right;
  state *frame, *head;
  head = *frame_buffer;
  pair reward;
  while(!is_terminal(s)) {
    i++;
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
