//
// Created by Satadru on 12/06/19.
//

#include <curses.h>
#include <unistd.h>
#include "sticks.h"
#include "utils.h"

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

void stick_move(ball *ball, int mode, int key, int *end, int *stick_y_l, int *stick_y_r) {
  if (mode == 2) {
    switch (key) {
      case KEY_UP:
        if (*stick_y_r > 0) {
          //next position is valid
          *stick_y_r -= 2;
        }
        //can potentially add scroll feature
        break;
      case KEY_DOWN:
        if (*stick_y_r + HEIGHT_OF_STICK < STICK_WINDOW_HEIGHT) {
          //next position is valid
          *stick_y_r += 2;
        }
        break;
      case 'w':
        if (*stick_y_l > 0) {
          //next position is valid
          *stick_y_l -= 2;
        }
        break;
      case 's':
        if (*stick_y_l + HEIGHT_OF_STICK < STICK_WINDOW_HEIGHT) {
          //next position is valid
          *stick_y_l += 2;
        }
        break;
      case 'q':
        //exit key
        *end = 1;
        break;
      default:
        break;
    }
  } else {
    /*user moves*/
    switch (key) {
      case KEY_UP:
        if (*stick_y_r > 0) {
          //next position is valid
          *stick_y_r -= 2;
        }
        //can potentially add scroll feature
        break;
      case KEY_DOWN:
        if (*stick_y_r + HEIGHT_OF_STICK < STICK_WINDOW_HEIGHT) {
          //next position is valid
          *stick_y_r += 2;
        }
        break;
      case 'q':
        //exit key
        *end = 1;
        break;
      default:
        break;
    }

    /*computer moves*/
    computer_move(ball, stick_y_l);
  }
}


