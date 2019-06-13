//
// Created by taariq on 6/11/19.
//

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <memory.h>
#include "sticks.h"
#include "ball.h"
#include "utils.h"


#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 45
#define SCORE_HEIGHT 13
#define SCORE_WIDTH (STICK_WINDOW_WIDTH - 20)
#define MAXSCORE 5
#define max(x,y) ((x) >= (y)) ? (x) : (y)

int main(void) {
  /*initialising game space*/
  initscr(); // Initialize the window
  WINDOW *win = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 5, 10);
  WINDOW *stick_window = subwin(win, STICK_WINDOW_HEIGHT, STICK_WINDOW_WIDTH, 6, 11);
  WINDOW *score_window = subwin(win, SCORE_HEIGHT, SCORE_WIDTH, 6 + STICK_WINDOW_HEIGHT + 2, 21);

  cbreak();

  noecho(); // Don't echo any keypresses
  curs_set(FALSE); // Don't display a cursor
  keypad(stick_window, TRUE); //recognises key inputs


/*initialising local variables*/
  int stick_y_l = (STICK_WINDOW_HEIGHT / 2 - 1),
    stick_y_r = (STICK_WINDOW_HEIGHT / 2 - 1),
    started = 0,
    end = 0,
    left_score = 0,
    right_score = 0,
    key = 0;

  ball *ball = create_ball();
  ball->x_position = BALL_INITIAL_X;
  ball->y_position = BALL_INITIAL_Y;
  ball->direction->x = 1;
  ball->direction->y = 0;

  char *message = "Are you ready to play pong? (Y/N)";
  int len = (int) strlen(message);
  mvwprintw(stick_window, 10, (STICK_WINDOW_WIDTH / 2) - (len / 2), message);
  wrefresh(stick_window);

  for (int i = 1; i < SCORE_HEIGHT - 1; i++) {
    mvwprintw(score_window, i, (STICK_WINDOW_WIDTH / 2) - 10, "|");
  }
  wrefresh(score_window);

  /*starter windows*/
  while (true) {
    box(score_window, ACS_VLINE, ACS_HLINE);


    print_number(score_window, left_score, 10, 3);
    print_number(score_window, right_score, SCORE_WIDTH - 20, 3);
    box(stick_window, ACS_VLINE, ACS_HLINE);
    wrefresh(stick_window);


    display_left_stick(stick_window, stick_y_l);
    display_right_stick(stick_window, stick_y_r);

    display_ball(stick_window, ball);

    nodelay(stick_window, TRUE);
    key = wgetch(stick_window);

    if (key == 'y') {
      started = 1;
      break;
    } else if (key == 'n') {
      end = 1;
      break;
    }
  }


  /*the actual game execution*/
  while (!end && (max(left_score, right_score)) < MAXSCORE) {
    cbreak();
    werase(score_window);
    box(score_window, ACS_VLINE, ACS_HLINE);

    for (int i = 1; i < SCORE_HEIGHT - 1; i++) {
      mvwprintw(score_window, i, (STICK_WINDOW_WIDTH / 2) - 10, "|");
    }
    wrefresh(score_window);

    print_number(score_window, left_score, 10, 3);
    print_number(score_window, right_score, SCORE_WIDTH - 20, 3);
    werase(stick_window);
    box(stick_window, ACS_VLINE, ACS_HLINE);
    wrefresh(stick_window);


    display_left_stick(stick_window, stick_y_l);
    display_right_stick(stick_window, stick_y_r);

    display_ball(stick_window, ball);

//    display_ball(stick_window, ball_x, ball_y);

    /*behaviour of valid key presses*/
    nodelay(stick_window, TRUE);
    key = wgetch(stick_window);
    switch (key) {
      case KEY_UP:
        if (stick_y_r > 0) {
          //next position is valid
          stick_y_r--;
        }
        //can potentially add scroll feature
        break;
      case KEY_DOWN:
        if (stick_y_r + HEIGHT_OF_STICK < STICK_WINDOW_HEIGHT) {
          //next position is valid
          stick_y_r++;
        }
        break;
      case 'w':
        if (stick_y_l > 0) {
          //next position is valid
          stick_y_l--;
        }
        break;
      case 's':
        if (stick_y_l + HEIGHT_OF_STICK < STICK_WINDOW_HEIGHT) {
          //next position is valid
          stick_y_l++;
        }
        break;
      case 'q':
        //exit key
        end = 1;
        break;
      default:
        break;
    }

    bounce_ball(stick_window, ball, stick_y_l, stick_y_r, &left_score, &right_score);
    move_ball(ball);
  }
  free_ball(ball);
  endwin();
}
