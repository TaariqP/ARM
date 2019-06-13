//
// Created by Akanksha on 12/06/19.
//
#include <ncurses.h>
#include <unistd.h>

#ifndef ARM11_11_BALL_H
#define ARM11_11_BALL_H

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

void display_ball(WINDOW *, int, int);



#endif //ARM11_11_BALL_H
