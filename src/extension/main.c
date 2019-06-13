//
// Created by taariq on 6/11/19.
//

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "sticks.h"
#include "ball.h"

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 30

int main(void) {
  /*initialising game space*/
  initscr(); // Initialize the window
  WINDOW *win = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 5, 10);
  WINDOW *stick_window = subwin(win, STICK_WINDOW_HEIGHT, STICK_WINDOW_WIDTH, 6, 11);
  cbreak();

  noecho(); // Don't echo any keypresses
  curs_set(FALSE); // Don't display a cursor
  keypad(stick_window, TRUE); //recognises key inputs


  box(win, '|', '-');
  wrefresh(win);


/*initialising local variables*/
  int stick_y_l = (STICK_WINDOW_HEIGHT / 2 - 1),
    stick_y_r = (STICK_WINDOW_HEIGHT / 2 - 1),
    end = 0,
    key = 0;


  ball *ball = create_ball();
  ball->x_position = 50;
  ball->y_position = 14;
  ball->direction->x = 1;
  ball->direction->y = 0;



  /*the actual game execution*/
  while (!end) {
    cbreak();
    werase(stick_window);

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

    bounce_ball(ball);
    move_ball(ball);
  }
  free_ball(ball);
  endwin();
}