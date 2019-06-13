//
// Created by Akanksha on 12/06/19.
//
#include <ncurses.h>
#include <unistd.h>

#ifndef ARM11_11_BALL_H
#define ARM11_11_BALL_H


typedef struct {
    int x;
    int y;
} vector;

typedef enum {
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST
} ball_direction;

typedef struct {
    int x_position;
    int y_position;
    ball_direction direction;
    vector *vector;
} ball;



void display_ball(WINDOW *, ball *);
void move_ball(ball *);
void free_ball(ball *);
void bounce_ball(ball *);
ball *create_ball();



#endif //ARM11_11_BALL_H
