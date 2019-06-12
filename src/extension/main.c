//
// Created by taariq on 6/11/19.
//

#include <ncurses.h>
#include <unistd.h>
#include "sticks.h"

int main(void) {
  /*initialising game space*/
  initscr(); // Initialize the window
  WINDOW *win = newwin(30, 100, 5, 10);
  WINDOW *stick_window = subwin(win, 28, 98, 6, 11);
  noecho(); // Don't echo any keypresses
  curs_set(FALSE); // Don't display a cursor
  keypad(stdscr, TRUE); //recognises key inputs


/*initialising local variables (co-ordinates)*/
  int stick_y_l = 13,
      stick_y_r = 13,
      end = 0,
      key = 0,
      max_y = 0,
      max_x = 0;



  while (!end) {
    box(win, '+', '-');
    wrefresh(win);
    getmaxyx(stick_window, max_y, max_x);
    werase(stick_window);

    display_left_stick(stick_window, stick_y_l);
    display_right_stick(stick_window, stick_y_r);

    /*behaviour of valid key presses*/
    nodelay(stick_window, TRUE);
    key = getch();
    switch (key){
      case KEY_UP:
        stick_y_r--;
        wrefresh(stick_window);
        break;
      case KEY_DOWN:
        stick_y_r++;
        break;
      case 'w':
        stick_y_l--;
        wrefresh(stick_window);
        break;
      case 's':
        stick_y_l++;
        break;
      case 'q':
        end = 1;
        break;
      default:
        break;
    }

  }
  endwin();
}