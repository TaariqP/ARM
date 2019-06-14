//
// Created by Akanksha on 13/06/19.
//
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <memory.h>
#include "utils.h"
#include "ball.h"
#include "sticks.h"

/*the functions in the section below are not intended to be used outside of utils.c*/
/*--------------------------------------------------------*/
/*--------------------------------------------------------*/
/*--------------------------------------------------------*/

void printA(WINDOW *window, int start_x, int start_y) {

  mvwprintw(window, start_y, start_x + 1, "____");
  wrefresh(window);
}

void printB(WINDOW *window, int start_x, int start_y) {
  mvwprintw(window, start_y + 1, start_x, "|");
  mvwprintw(window, start_y + 2, start_x, "|");

  wrefresh(window);
}

void printC(WINDOW *window, int start_x, int start_y) {
  mvwprintw(window, start_y + 3, start_x, "|");
  mvwprintw(window, start_y + 4, start_x, "|");

  wrefresh(window);
}

void printD(WINDOW *window, int start_x, int start_y) {

  mvwprintw(window, start_y + 4, start_x + 1, "____");
  wrefresh(window);
}

void printE(WINDOW *window, int start_x, int start_y) {
  mvwprintw(window, start_y + 3, start_x + 5, "|");
  mvwprintw(window, start_y + 4, start_x + 5, "|");

  wrefresh(window);
}

void printF(WINDOW *window, int start_x, int start_y) {
  mvwprintw(window, start_y + 1, start_x + 5, "|");
  mvwprintw(window, start_y + 2, start_x + 5, "|");

  wrefresh(window);
}

void printG(WINDOW *window, int start_x, int start_y) {

  mvwprintw(window, start_y + 2, start_x + 1, "____");
  wrefresh(window);
}
/*--------------------------------------------------------*/
/*--------------------------------------------------------*/
/*--------------------------------------------------------*/

void (*printers[7])(WINDOW *, int, int) = {printA, printB, printC, printD, printE, printF, printG};


void print_number(WINDOW *window, int score, int start_x, int start_y) {
  int toprint[7];
  switch (score) {
    case 0: {
      int temp[7] = {1, 1, 1, 1, 1, 1, 0};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 1: {
      int temp[7] = {0, 0, 0, 0, 1, 1, 0};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 2: {
      int temp[7] = {1, 0, 1, 1, 0, 1, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 3: {
      int temp[7] = {1, 0, 0, 1, 1, 1, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 4: {
      int temp[7] = {0, 1, 0, 0, 1, 1, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 5: {
      int temp[7] = {1, 1, 0, 1, 1, 0, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 6: {
      int temp[7] = {1, 1, 1, 1, 1, 0, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 7: {
      int temp[7] = {1, 0, 0, 0, 1, 1, 0};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 8: {
      int temp[7] = {1, 1, 1, 1, 1, 1, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 9: {
      int temp[7] = {1, 1, 0, 1, 1, 1, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    default: {
      int temp[7] = {1, 1, 0, 1, 1, 1, 1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
  }

  for (int i = 0; i < 7; i++) {
    if (toprint[i] == 1) {
      printers[i](window, start_x, start_y);
    }
  }
}

void initialise_game(WINDOW *window, ball *ball, int *left_score, int *right_score, int *stickleft, int *stickright) {
  *stickleft = (STICK_WINDOW_HEIGHT / 2 - 1);
  *stickright = (STICK_WINDOW_HEIGHT / 2 - 1);
  *left_score = 0;
  *right_score = 0;
  ball->x_position = BALL_INITIAL_X;
  ball->y_position = BALL_INITIAL_Y;
  ball->direction->x = 1;
  ball->direction->y = 0;
}

void display_game(WINDOW *game_window, ball *ball, int stick_y_l, int stick_y_r) {
  box(game_window, ACS_VLINE, ACS_HLINE);
  display_left_stick(game_window, stick_y_l);
  display_right_stick(game_window, stick_y_r);
  display_ball(game_window, ball);
}

void
display_game_state(WINDOW *game_window, WINDOW *score_window, ball *ball, int stick_y_l, int stick_y_r, int left_score,
                   int right_score) {
  box(score_window, ACS_VLINE, ACS_HLINE);
  for (int i = 1; i < SCORE_HEIGHT - 1; i++) {
    mvwprintw(score_window, i, (STICK_WINDOW_WIDTH / 2) - 10, "|");
  }
  wrefresh(score_window);
  print_number(score_window, left_score, SCORE_WIDTH / 4 - 5, 3);
  print_number(score_window, right_score, (3 * (SCORE_WIDTH / 4)), 3);

  display_game(game_window, ball, stick_y_l, stick_y_r);
}

void print_message_center(WINDOW *window, char *message, int y_pos) {
  int l = (int) strlen(message);
  mvwprintw(window, y_pos, (STICK_WINDOW_WIDTH / 2) - (l / 2), message);
  wrefresh(window);

}

void computer_move(ball *ball, int *stick_y_l) {
  int top_of_paddle = *stick_y_l;
  int bottom_of_paddle = *stick_y_l + HEIGHT_OF_STICK;

  if (STICK_WINDOW_WIDTH/2 > ball->x_position) {
    if (ball->y_position > bottom_of_paddle) {
      *stick_y_l += 2;
    } else if (ball->y_position < top_of_paddle + 1) {
      *stick_y_l -= 2;
    }
  }
}