/*
 * Config settings for simulator and hyperparameters for DQL
 */

#ifndef CONFIG_H
#define CONFIG_H
// Simulator settings
#define WIDTH 10
#define HEIGHT 10
#define PADDLE_HEIGHT 1
#define PADDLE_SPEED 0.1
#define BALL_SPEED 0.1
#define RANDOMNESS 0.02
#define BOUNCE_REWARD 1
#define POINT_REWARD 10
#define POINT_PENALTY -10

// Policy settings
#define INPUT 5
#define HIDDEN 32
#define OUTPUT 1
#define LEARNING_RATE 0.01

// Convenience defines
#define LEFT_PLAYER 0
#define RIGHT_PLAYER 1

#endif
