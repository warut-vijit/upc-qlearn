#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "linalg.h"

/*
 * Simulator of Pong Atari game used by qlearn.h
 */
#ifndef SIM_H
#define SIM_H
#define rand_dbl(MIN, MAX) (((double)rand() / RAND_MAX) * (MAX - MIN) + MIN)

typedef struct {
  double b_x, b_y, b_theta;
  double p_l, p_r;
} simulator;

typedef struct {
  int first;
  int second;
} pair;

void sim_init(simulator *s);
pair sim_step(simulator *s, int d_l, int d_r);
void sim_print(simulator *s);
void sim_to_v(simulator *s, vec *v, int pid); 

#endif
