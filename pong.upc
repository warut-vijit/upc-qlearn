#include "pong.h"

void init(simulator *s) {
  s->p_l = s->p_r = HEIGHT / 2;
  s->b_y = HEIGHT / 2;
  s->b_x = WIDTH / 2;
  s->b_theta = rand_dbl(0, 2 * M_PI);
}

void step(simulator *s, int d_l, int d_r) {
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
    } else {
      printf("Game over, p_l lost\n");
    }
  }
  else if(s->b_x > WIDTH) {
    if(abs(s->b_y - s->p_r) < PADDLE_HEIGHT / 2){
      s->b_x = 2 * WIDTH - s->b_x;
      s->b_theta = M_PI - s->b_theta;
    } else {
      printf("Game over, p_r lost\n");
    }
  }
}

void print(simulator *s) {
  printf("sim: h:%d w:%d b:(%f,%f) pl:%f pr:%f\n", HEIGHT, WIDTH, s->b_x, s->b_y, s->p_l, s->p_r);
}
