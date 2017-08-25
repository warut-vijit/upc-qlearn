/*
 * Config settings for simulator and hyperparameters for DQL
 */

#ifndef CONFIG_H
#define CONFIG_H
// Simulator settings
#define WIDTH 10
#define HEIGHT 10
#define PADDLE_HEIGHT 3.0
#define PADDLE_SPEED 0.5
#define BALL_SPEED 0.4
#define RANDOMNESS 0.02
#define BOUNCE_REWARD 1
#define POINT_REWARD 3
#define POINT_PENALTY -3

// Policy settings
#define INPUT 9
#define HIDDEN 16
#define OUTPUT 1
#define LEARNING_RATE 0.05
#define DISCOUNTING 0.95
#define ITERATIONS 300
#define EPSILON 0.2

// Convenience defines
#define LEFT_PLAYER 0
#define RIGHT_PLAYER 1

#endif
