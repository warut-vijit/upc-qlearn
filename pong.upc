#include "pong.h"

void sim_init(simulator *s) {
  s->p_l = s->p_r = HEIGHT / 2;
  s->b_y = HEIGHT / 2;
  s->b_x = WIDTH / 2;
  s->b_theta = rand_dbl(0, 2 * M_PI);
}

pair sim_step(simulator *s, int d_l, int d_r) {
  pair reward;

  // paddle motion
  s->p_l = fmax(0, fmin(10, s->p_l + PADDLE_SPEED * d_l));
  s->p_r = fmax(0, fmin(10, s->p_r + PADDLE_SPEED * d_r));
  
  // ball motion
  s->b_x += BALL_SPEED * cos(s->b_theta);
  s->b_y += BALL_SPEED * sin(s->b_theta);
  if(s->b_y > HEIGHT) {
    s->b_y = 2*HEIGHT - s->b_y;
    s->b_theta = 2 * M_PI - s->b_theta;
  } else if(s->b_y < 0) {
    s->b_y *= -1;
    s->b_theta = 2 * M_PI - s->b_theta;
  }
  if(s->b_x < 0) {
    if(abs(s->b_y - s->p_l) < PADDLE_HEIGHT / 2){
      s->b_x *= -1;
      s->b_theta = M_PI - s->b_theta;
      reward.first = BOUNCE_REWARD;
      reward.second = 0;
    } else {
      reward.first = POINT_PENALTY;
      reward.second = POINT_REWARD;
    }
  }
  else if(s->b_x > WIDTH) {
    if(abs(s->b_y - s->p_r) < PADDLE_HEIGHT / 2){
      s->b_x = 2 * WIDTH - s->b_x;
      s->b_theta = M_PI - s->b_theta;
      reward.first = 0;
      reward.second = BOUNCE_REWARD;
    } else {
      reward.first = POINT_REWARD;
      reward.second = POINT_PENALTY;
    }
  }
  return reward;
}

void sim_print(simulator *s) {
  printf("sim: h:%d w:%d b:(%f,%f) pl:%f pr:%f\n", HEIGHT, WIDTH, s->b_x, s->b_y, s->p_l, s->p_r);
}

/*
 * Feature vector: ball x, ball y, own paddle, other paddle
 */
void sim_to_v(simulator *s, vec *v, int pid) {
  if(v->data == NULL) vec_create(v, 5);
  if(pid == LEFT_PLAYER) {
    v->data[0] = s->b_x;
    v->data[2] = s->p_l;
    v->data[3] = s->p_r;
  } else if(pid == RIGHT_PLAYER) {
    v->data[0] = WIDTH - s->b_x;
    v->data[2] = s->p_r;
    v->data[3] = s->p_l;
  }
  v->data[1] = s->b_y;
}
