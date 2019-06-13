//
// Created by Akanksha on 12/06/19.
//
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include "ball.h"

ball *create_ball() {
  ball *ball = calloc(1, sizeof(ball));
  if (ball == NULL) {
    perror("could not allocate memory to ball");
    exit(EXIT_FAILURE);
  }
  ball->direction = calloc(1, sizeof(direction));
  if (ball->direction == NULL) {
    perror ("could not allocate memory to ball vector");
    free(ball);
    exit(EXIT_FAILURE);
  }
  return ball;
}

void display_ball(WINDOW *window, ball *ball) {
  mvwprintw(window, ball->y_position, ball->x_position, "o");
  wrefresh(window);
}

void move_ball(ball *ball) {
  ball->x_position += ball->direction->x;
  ball->y_position -= ball->direction->y;

  if (ball->direction->y != 0) {
    usleep(60000);
  } else {
    usleep(30000);
  }

}

static int illegal_x(ball *ball) {
  //check next x
  int next_x = ball->x_position + ball->direction->x;
  if (next_x >= STICK_WINDOW_WIDTH -1 || next_x < 1) {
    return 1;
  }
  return 0;
}

static int illegal_y(ball *ball) {
    //check next y
  int next_y = ball->y_position -= ball->direction->y;
  if (next_y > STICK_WINDOW_HEIGHT -1 || next_y < 1) {
    return 1;
  }

  return 0;
}

static int getRand() {
  srand(time(0));
  int options[3] = {-1 ,0, 1};
  int index = rand() % 3;
  return options[index];
}

void bounce_ball(ball *ball) {
  while (illegal_x(ball) || illegal_y(ball)) {
    if (illegal_x(ball) && !illegal_y(ball)) {
      ball->direction->x *= -1;
      ball->direction->y = getRand();
      break;
    }
    if (illegal_y(ball) && !illegal_x(ball)) {
      ball->direction->y *= -1;
      break;
    }
    if (illegal_x(ball) && illegal_y(ball)) {
      ball->direction->y *= -1;
      ball->direction->x *= -1;
      break;
    }
  }
}

void free_ball(ball *ball) {
  free(ball->direction);
  free(ball);
}
