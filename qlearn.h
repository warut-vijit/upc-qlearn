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
  vec b1;
  mat m2;
  vec b2;
} policy;

void policy_init(policy *p) {
  mat_create_gaussian(&p->m1, INPUT, HIDDEN, 0.0, sqrt(2.0/INPUT));
  vec_create_gaussian(&p->b1, HIDDEN, 0.0, sqrt(2.0/INPUT)); 
  mat_create_gaussian(&p->m2, HIDDEN, OUTPUT, 0.0, sqrt(2.0/(INPUT * HIDDEN)));
  vec_create_gaussian(&p->b2, OUTPUT, 0.0, sqrt(2.0/(INPUT * HIDDEN)));
}

void policy_delete(policy *p) {
  mat_delete(&p->m1);
  mat_delete(&p->m2);
}

double policy_fwd(policy *p, vec *v) {
  vec temp, out;
  double val;
  v_m_mult(v, &p->m1, &temp);
  v_v_add_assign(&temp, &p->b1);
  v_m_mult(&temp, &p->m2, &out);
  v_v_add_assign(&out, &p->b2);
  val = v_to_scalar(&out);
  vec_delete(&temp);
  vec_delete(&out);
  return val;
}

pair get_move(simulator *s, policy *p, int pid, double epsilon) {
  vec v;
  int action = 0;
  double tmp, val = -1 * DBL_MAX;
  // epsilon-greedy policy
  sim_to_v(s, &v, pid);
  if((double) rand() / RAND_MAX < epsilon) {
    action = rand() % 3 - 1;
    v.data[6] = action == -1;
    v.data[7] = action == 0;
    v.data[8] = action == 1;
    pair ret;
    ret.first = action;
    ret.second = policy_fwd(p, &v);
    return ret;
  }
   
  v.data[6] = 1;
  v.data[7] = 0;
  v.data[8] = 0;
  tmp = policy_fwd(p, &v);
  //printf("-1:%f\t", tmp);
  action = (tmp > val) ? -1 : action;
  val = fmax(tmp, val);
  v.data[6] = 0;
  v.data[7] = 1;
  v.data[8] = 0;
  tmp = policy_fwd(p, &v);
  //printf("0:%f\t", tmp);
  action = (tmp > val) ? 0 : action;
  val = fmax(tmp, val);
  v.data[6] = 0;
  v.data[7] = 0;
  v.data[8] = 1;
  tmp = policy_fwd(p, &v);
  //printf("1:%f\n", tmp);
  action = (tmp > val) ? 1 : action;
  val = fmax(tmp, val);
  vec_delete(&v);
  pair ret;
  ret.first = action;
  ret.second = val;
  return ret;  
}

void policy_backprop(policy *p, state *frame_buffer) {
  double targ_l = 0, targ_r = 0;
  int examples = 0;
  state *frame_iter = frame_buffer;
  simulator next_frame;
  vec b1_d, b2_d;
  mat m1_d, m2_d;
  vec_create_zeros(&b1_d, HIDDEN);
  mat_create_zeros(&m1_d, INPUT, HIDDEN);
  vec_create_zeros(&b2_d, OUTPUT);
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
    i.data[6] = frame_iter->a_l == -1;
    i.data[7] = frame_iter->a_l == 0;
    i.data[8] = frame_iter->a_l == 1;
    v_m_mult(&i, &p->m1, &h);
    v_v_add_assign(&h, &p->b1);
    v_m_mult(&h, &p->m2, &o);
    v_v_add_assign(&o, &p->b2);

    // backpropagate gradients
    if(frame_iter == frame_buffer) targ_l = frame_iter->reward.first;
    else targ_l = frame_iter->reward.first + DISCOUNTING * get_move(&next_frame, p, LEFT_PLAYER, 0).second;
    scalar_to_v(targ_l - v_to_scalar(&o), &dEdo);
    v_v_outer(&h, &dEdo, &m2_del);
    v_m_mult(&dEdo, &m2_T, &dEdh);
    v_v_outer(&i, &dEdh, &m1_del);

    // accumulate gradients
    v_v_add_assign(&b1_d, &dEdh);
    m_m_add_assign(&m1_d, &m1_del);
    v_v_add_assign(&b2_d, &dEdo);
    m_m_add_assign(&m2_d, &m2_del);
    printf("\nl:t:%f o:%f", targ_l, v_to_scalar(&o));
    
    // free allocated memory
    batch_vec_delete(5, &i, &h, &o, &dEdh, &dEdo);

    // RIGHT PLAYER forward pass, get input, hidden, out vectors
    sim_to_v(&frame_iter->sim, &i, RIGHT_PLAYER);

    i.data[6] = frame_iter->a_r == -1;
    i.data[7] = frame_iter->a_r == 0;
    i.data[8] = frame_iter->a_r == 1;
    v_m_mult(&i, &p->m1, &h);
    v_v_add_assign(&h, &p->b1);
    v_m_mult(&h, &p->m2, &o);
    v_v_add_assign(&o, &p->b2);

    // backpropagate gradients
    if(frame_iter == frame_buffer) targ_r = frame_iter->reward.second;
    else targ_r = frame_iter->reward.second + DISCOUNTING * get_move(&next_frame, p, RIGHT_PLAYER, 0).second;
    scalar_to_v(targ_r - v_to_scalar(&o), &dEdo);
    v_v_outer(&h, &dEdo, &m2_del);
    v_m_mult(&dEdo, &m2_T, &dEdh);
    v_v_outer(&i, &dEdh, &m1_del);
    
    // accumulate gradients
    v_v_add_assign(&b1_d, &dEdh);
    m_m_add_assign(&m1_d, &m1_del);
    v_v_add_assign(&b2_d, &dEdo);
    m_m_add_assign(&m2_d, &m2_del);

    printf("\t\tr:t:%f o:%f\n", targ_r, v_to_scalar(&o));
    // free allocated memory
    batch_vec_delete(5, &i, &h, &o, &dEdh, &dEdo);
    next_frame = frame_iter->sim;
    frame_iter = frame_iter->next;
  }

  // apply gradient update
  v_mult(&b1_d, LEARNING_RATE / examples);
  m_mult(&m1_d, LEARNING_RATE / examples);
  v_mult(&b2_d, LEARNING_RATE / examples);
  m_mult(&m2_d, LEARNING_RATE / examples);
  v_v_add_assign(&p->b1, &b1_d);
  m_m_add_assign(&p->m1, &m1_d);
  v_v_add_assign(&p->b2, &b2_d);
  m_m_add_assign(&p->m2, &m2_d);
  printf("norms: m1:%f m2:%f\n", frobenius_norm(&m1_d), frobenius_norm(&m2_d));

  // free allocated memory
  batch_vec_delete(2, &b1_d, &b2_d);
  batch_mat_delete(4, &m1_d, &m2_d, &m1_T, &m2_T);
}

void collect(simulator *s, policy *p, state **frame_buffer, double epsilon) {
  int i = 0;
  int a_left, a_right;
  state *frame, *head;
  head = *frame_buffer;
  pair reward;
  while(!is_terminal(s)) {
    i++;
    // compute action for players
    a_left = get_move(s, p, LEFT_PLAYER, epsilon).first;
    a_right = get_move(s, p, RIGHT_PLAYER, epsilon).first;
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
