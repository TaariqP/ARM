//
// Created by Satadru on 12/06/19.
//

#include <curses.h>
#include <unistd.h>
#include "sticks.h"

/*prints a stick to a window at a set position*/
void display_left_stick(WINDOW *window, int y_position) {
  for (int i = 0; i < 4; i++) {
    mvwprintw(window, y_position + i, 1, "|");
    mvwprintw(window, y_position + i, 2, "|");
  }
  wrefresh(window);
}

void display_right_stick(WINDOW *window, int y_position) {

  int end_x_position = getmaxx(window) - 1;
  for (int i = 0; i < 4; i++) {
    mvwprintw(window, y_position + i, end_x_position - 1, "|");
    mvwprintw(window, y_position + i, end_x_position - 2, "|");
  }
  wrefresh(window);
}


