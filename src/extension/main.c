//
// Created by taariq on 6/11/19.
//

#include <ncurses.h>
#include <unistd.h>

int main(void) {
  initscr(); // Initialize the window
  noecho(); // Don't echo any keypresses
  curs_set(TRUE); // Don't display a cursor
  move(6, 5);
  printw("sats");
  refresh();
  sleep(2);

  endwin();
}