#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"

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

void init(simulator *s);
void step(simulator *s, int d_l, int d_r);
void print(simulator *s);

#endif
