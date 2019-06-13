//
// Created by Akanksha on 12/06/19.
//
#include <ncurses.h>
#include <unistd.h>

#define STICK_WINDOW_WIDTH 98
#define STICK_WINDOW_HEIGHT 28
#ifndef ARM11_11_BALL_H
#define ARM11_11_BALL_H

typedef enum {
    ZERO = 0,
    POSITIVE = 1,
    NEGATIVE = -1
} magnitude;

typedef struct {
    magnitude x;
    magnitude y;
} direction;


typedef struct {
    int x_position;
    int y_position;
    direction *direction;
} ball;


void display_ball(WINDOW *, ball *);

void move_ball(ball *);

void free_ball(ball *);

void bounce_ball(ball *);

ball *create_ball();


#endif //ARM11_11_BALL_H
