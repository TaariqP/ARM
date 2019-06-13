//
// Created by taariq on 6/11/19.
//

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


/*initialising local variables (co-ordinates)*/
  int stick_y_l = (STICK_WINDOW_HEIGHT / 2 - 1),
    stick_y_r = (STICK_WINDOW_HEIGHT / 2 - 1),
    ball_y = 14,
    ball_x = 50,
    end = 0,
    key = 0,
    direction = 1,
    max_y = 0,
    max_x = 0;

  while (!end) {
    cbreak();
    getmaxyx(stick_window, max_y, max_x);
    werase(stick_window);

    display_left_stick(stick_window, stick_y_l);
    display_right_stick(stick_window, stick_y_r);

    display_ball(stick_window, ball_x, ball_y);

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
        if (stick_y_r + HEIGHT_OF_STICK < max_y) {
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
        if (stick_y_l + HEIGHT_OF_STICK < max_y) {
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


    if ((ball_x + direction) >= max_x || (ball_x + direction) < 0) {
      direction *= -1;
    } else {
      ball_x += direction;
    }

  }
  endwin();
}