//
// Created by Akanksha on 12/06/19.
//

#include <ncurses.h>
#include <unistd.h>

void display_ball(WINDOW *window, int x_position, int y_position) {
  mvwprintw(window, y_position, x_position, "o");
  wrefresh(window);
  usleep(30000);
}

