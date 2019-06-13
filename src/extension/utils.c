//
// Created by Akanksha on 13/06/19.
//
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <memory.h>
#include "utils.h"


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
      int temp[7] = {1,0,1,1,0,1,1};
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
      int temp[7] = {1,1,0,1,1,0,1};
      memcpy(toprint, temp, (sizeof(int) * 7));
    }
      break;
    case 6: {
      int temp[7] = {1,1,1,1,1,0,1};
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
      printers[i] (window, start_x, start_y);
    }
  }
}