//
// Created by Akanksha on 12/06/19.
//
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include "ball.h"
#include "sticks.h"

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
  if (next_x >= STICK_WINDOW_WIDTH -2 || next_x < 2) {
    return 1;
  }
  return 0;
}

static int illegal_y(ball *ball) {
    //check next y
  int next_y = ball->y_position -= ball->direction->y;
  if (next_y > STICK_WINDOW_HEIGHT -2 || next_y < 1) {
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

static void refresh_screen(WINDOW *window, ball *ball, int stick_left_y, int stick_right_y){
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
    ball->direction->x *=2;
  }
}

void set_steepness_to_1(ball *ball) {
  if (ball->direction->x == 2 || ball->direction->x == -2) {
    ball->direction->x /=2;
  }
}

void (*steepness_functions[2]) (ball *) = {set_steepness_to_1, set_steepness_to_2};


void bounce_ball(WINDOW *window, ball *ball, int stick_left_y, int stick_right_y, int *score_left, int *score_right) {
  while (illegal_x(ball) || illegal_y(ball)) {
    if (illegal_x(ball) && !illegal_y(ball)) {
      if (ball->direction->x == -1) {

        //travelling left
        if (ball->y_position >= stick_left_y && ball->y_position < stick_left_y + HEIGHT_OF_STICK) {
          //will hit left stick
          ball->direction->x *= -1;
          ball->direction->y = getRand();
          break;
        }
        *score_right+=1;
        refresh_screen(window, ball, stick_left_y, stick_right_y);
        break;
      }

      if (ball->y_position >= stick_right_y && ball->y_position < stick_right_y + HEIGHT_OF_STICK) {
        //will hit right stick
        ball->direction->x *= -1;
        ball->direction->y = getRand();
        break;
      }
      *score_left+=1;
      refresh_screen(window, ball, stick_left_y, stick_right_y);
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
