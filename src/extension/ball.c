//
// Created by Akanksha on 12/06/19.
//
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include "ball.h"
#include "sticks.h"
#include "utils.h"

ball *create_ball() {
  ball *ball = calloc(1, sizeof(ball));
  if (ball == NULL) {
    perror("could not allocate memory to ball");
    exit(EXIT_FAILURE);
  }
  ball->direction = calloc(1, sizeof(direction));
  if (ball->direction == NULL) {
    perror("could not allocate memory to ball vector");
    free(ball);
    exit(EXIT_FAILURE);
  }
  return ball;
}

void display_ball(WINDOW *window, ball *ball) {
  mvwprintw(window, ball->y_position, ball->x_position, "0");
  wrefresh(window);
}

void move_ball(ball *ball) {
  ball->x_position += ball->direction->x;
  ball->y_position -= ball->direction->y;

  if (ball->direction->y != 0) {
    usleep(120000);
  } else {
    usleep(60000);
  }

}


static int illegal_x(ball *ball) {
  //check next x
  int next_x = ball->x_position + ball->direction->x;
  if (ball->direction->x == 2 || ball->direction->x == -2) {
    if (next_x >= STICK_WINDOW_WIDTH - 3 || next_x < 3) {
      ball->direction->x /=2;
      return illegal_x(ball);
    }
  } else {
    if (next_x >= STICK_WINDOW_WIDTH - 2 || next_x < 2) {
      return 1;
    }
  }


  return 0;
}

static int illegal_y(ball *ball) {
  //check next y
  int next_y = ball->y_position -= ball->direction->y;
  if (next_y > STICK_WINDOW_HEIGHT - 2 || next_y < 1) {
    return 1;
  }

  return 0;
}

static int random_index(int length_of_list) {
  srand(time(0));
  return rand() % length_of_list;
}

static int get_random_magintude() {
  int options[3] = {-1, 0, 1};
  int index = random_index(3);
  return options[index];
}

static void refresh_screen(WINDOW *window, ball *ball, int stick_left_y, int stick_right_y) {
  sleep(1);
  ball->x_position = BALL_INITIAL_X;
  ball->y_position = BALL_INITIAL_Y;
  ball->direction->x *= -1;
  werase(window);
  display_left_stick(window, stick_left_y);
  display_right_stick(window, stick_right_y);
  display_ball(window, ball);
  wrefresh(window);
  sleep(1);
}


void set_steepness_to_2(ball *ball) {
  if (ball->direction->x == 1 || ball->direction->x == -1) {
    ball->direction->x *= 2;
  }
}

void set_steepness_to_1(ball *ball) {
  if (ball->direction->x == 2 || ball->direction->x == -2) {
    ball->direction->x /= 2;
  }
}


void (*steepness_functions[2])(ball *) = {set_steepness_to_1, set_steepness_to_2};

static void
bounce_behaviour(WINDOW *window, ball *ball, int stick_left_y, int stick_right_y, int *score_left, int *score_right) {
  if (ball->direction->x == -1) {

    //travelling left
    if (ball->y_position >= stick_left_y && ball->y_position < stick_left_y + HEIGHT_OF_STICK) {
      //will hit left stick
      ball->direction->x *= -1;
      int index = random_index(2);
      steepness_functions[index](ball);
      ball->direction->y = get_random_magintude();
      return;
    }
    *score_right += 1;
    refresh_screen(window, ball, stick_left_y, stick_right_y);
    return;
  }

  if (ball->y_position >= stick_right_y && ball->y_position < stick_right_y + HEIGHT_OF_STICK) {
    //will hit right stick
    ball->direction->x *= -1;
    int index = random_index(2);
    steepness_functions[index](ball);
    ball->direction->y = get_random_magintude();
    return;
  }
  *score_left += 1;
  refresh_screen(window, ball, stick_left_y, stick_right_y);
}

void bounce_ball(WINDOW *window, ball *ball, int stick_left_y, int stick_right_y, int *score_left, int *score_right) {
  while (illegal_x(ball) || illegal_y(ball)) {
    //slow_if_edge_case
    if (illegal_x(ball) && !illegal_y(ball)) {
      bounce_behaviour(window, ball, stick_left_y, stick_right_y, score_left, score_right);
      break;
    }

    if (illegal_y(ball) && !illegal_x(ball)) {
      ball->direction->y *= -1;
      int index = random_index(2);
      steepness_functions[index](ball);
      break;
    }
    if (illegal_x(ball) && illegal_y(ball)) {

      ball->direction->y *= -1;
      bounce_behaviour(window, ball, stick_left_y, stick_right_y, score_left, score_right);
      break;
    }
  }
}

void free_ball(ball *ball) {
  free(ball->direction);
  free(ball);
}
