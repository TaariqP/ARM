//
// Created by Satadru on 12/06/19.
//

#include <ncurses.h>
#include <unistd.h>
#include "sticks.h"


/*prints a stick to a window at a set position*/
void display_left_stick(WINDOW *window, int y_position) {
  mvwprintw(window, y_position, 0, "|");
  mvwprintw(window, y_position+1, 0, "|");
  wrefresh(window);
}

void display_right_stick(WINDOW *window, int y_position) {

  int end_x_position = getmaxx(window) - 1;

  mvwprintw(window, y_position, end_x_position, "|");
  mvwprintw(window, y_position+1, end_x_position, "|");
  wrefresh(window);
}

