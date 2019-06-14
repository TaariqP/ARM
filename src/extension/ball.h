//
// Created by Akanksha on 12/06/19.
//
#include <curses.h>
#include <unistd.h>

#define STICK_WINDOW_WIDTH 98
#define STICK_WINDOW_HEIGHT 28
#ifndef ARM11_11_BALL_H
#define ARM11_11_BALL_H

#define BALL_INITIAL_X 50
#define BALL_INITIAL_Y 14


typedef struct {
    int x;
    int y;
} direction;


typedef struct {
    int x_position;
    int y_position;
    direction *direction;
} ball;


void display_ball(WINDOW *, ball *);

void move_ball(ball *);

void free_ball(ball *);

void bounce_ball(WINDOW *, ball *, int, int, int *, int *);

ball *create_ball();


#endif //ARM11_11_BALL_H
