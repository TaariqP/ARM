//
// Created by Akanksha on 12/06/19.
//
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "ball.h"

ball *create_ball() {
  ball *ball = calloc(1, sizeof(ball));
  if (ball == NULL) {
    perror("could not allocate memory to ball");
    exit(EXIT_FAILURE);
  }
  ball->vector = calloc(1, sizeof(vector));
  if (ball->vector == NULL) {
    perror ("could not allocate memory to ball vector");
    free(ball);
    exit(EXIT_FAILURE);
  }
  return ball;
}

void display_ball(WINDOW *window, ball *ball) {
  mvwprintw(window, ball->y_position, ball->x_position, "o");
  wrefresh(window);
  usleep(30000);
}

static void set_ball_y(ball *ball) {
  switch (ball->direction) {
    case NORTH:
    case NORTHEAST:
    case NORTHWEST:
      ball->vector->y = 1;
      ball->y_position--;
      break;
    case SOUTH:
    case SOUTHEAST:
    case SOUTHWEST:
      ball->vector->y = -1;
      ball->y_position++;
      break;
    default:
      break;
  }

}

static void set_ball_x(ball *ball) {
  switch (ball->direction) {
    case EAST:
    case NORTHEAST:
    case SOUTHEAST:
      ball->vector->x = 1;
      ball->x_position++;
      break;
    case WEST:
    case NORTHWEST:
    case SOUTHWEST:
      ball->vector->x = -1;
      ball->x_position--;
      break;
    default:
      break;
  }
}

void move_ball(ball *ball) {
  set_ball_x(ball);
  set_ball_y(ball);
  usleep(10000);
}


void bounce_ball(ball *ball) {
  ball_direction current = ball->direction;
  ball_direction new = (current + 3) % 8;
  int pick_random = rand() % 3;
  new = (new + pick_random) % 8;
  ball->direction = new;
}

void free_ball(ball *ball) {
  free(ball->vector);
  free(ball);
}
