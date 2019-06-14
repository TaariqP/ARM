//
// Created by taariq on 6/11/19.
//

#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include "sticks.h"
#include "ball.h"
#include "utils.h"


int main(void) {
  /*initialising game space*/
  initscr(); // Initialize the window
  WINDOW *win = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 5, 10);
  wattron(win, A_BOLD);
  WINDOW *game_window = subwin(win, STICK_WINDOW_HEIGHT, STICK_WINDOW_WIDTH, 6, 11);
  WINDOW *score_window = subwin(win, SCORE_HEIGHT, SCORE_WIDTH, 6 + STICK_WINDOW_HEIGHT + 2, 21);

  cbreak();
  scrollok(game_window, TRUE);
  scrollok(score_window, TRUE);

  noecho(); // Don't echo any keypresses
  curs_set(FALSE); // Don't display a cursor
  keypad(game_window, TRUE); //recognises key inputs

  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLUE);
  init_pair(2, COLOR_CYAN, COLOR_GREEN);
  wattron(game_window, COLOR_PAIR(1));
  wattron(score_window, COLOR_PAIR(2));

/*initialising local variables*/
  int stick_y_l,
    stick_y_r,
    has_just_quit = 0,
    end = 0,
    mode = 0,
    cont = 1,
    left_score,
    right_score,
    key = 0;

  ball *ball = create_ball();

  initialise_game(game_window, ball, &left_score, &right_score, &stick_y_l, &stick_y_r);

  print_message_center(game_window, "Are you ready to play NEON PONG? (Y/N)", 8);
  print_message_center(game_window, "First one to five wins!", 10);
  print_message_center(game_window, "(For controls, press C)", 12);

  /*starter windows*/
  while (key != 'y') {
    display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);

    key = wgetch(game_window);
    if (key == 'n') {
      end = 1;
      cont = 0;
      mode = 3;
      break;
    } else if (key == 'c') {
      werase(game_window);
      display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);
      print_instructions(game_window);
    }
  }

  werase(game_window);
  /*select mode, will have different game cycles*/
  while (mode == 0) {
    display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);
    print_message_center(game_window, "Press 1 for single player mode, or 2 for two players ", 7);
    print_message_center(game_window, "In single player mode, you will be right player (use arrow keys)", 9);

    key = wgetch(game_window);
    if (key == '2') {
      mode = 2;
    }
    if (key == '1') {
      mode = 1;
    }
  }

  while (cont) {
    /*the actual game execution*/
    while (!end && (max(left_score, right_score)) < MAXSCORE) {
      cbreak();
      werase(score_window);
      werase(game_window);
      display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);

      display_left_stick(game_window, stick_y_l);
      display_right_stick(game_window, stick_y_r);

      display_ball(game_window, ball);

      /*behaviour of valid key presses, mode dependent*/
      nodelay(game_window, TRUE);
      key = wgetch(game_window);
      stick_move(ball, mode, key, &end, &stick_y_l, &stick_y_r);

      bounce_ball(game_window, ball, stick_y_l, stick_y_r, &left_score, &right_score);
      move_ball(ball);
      if ((max(left_score, right_score)) < MAXSCORE) {
        has_just_quit = 1;
      }
    }

    if (has_just_quit) {
      werase(game_window);
      werase(score_window);
      has_just_quit = 0;
    }

    win_player(game_window, left_score, right_score);

    print_message_center(game_window, "Press R for rematch or Q to confirm quit", 12);
    display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);

    nodelay(game_window, TRUE);
    key = wgetch(game_window);
    if (key == 'r') {
      werase(game_window);
      werase(score_window);
      cont = 1;
      end = 0;
      initialise_game(game_window, ball, &left_score, &right_score, &stick_y_l, &stick_y_r);
      display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);
      print_message_center(game_window, "Press 1 for single player mode, or 2 for two players ", 7);
      print_message_center(game_window, "In single player mode, you will be right player (use arrow keys)", 9);

      mode = 0;
      while (mode == 0) {
        key = wgetch(game_window);
        if (key == '2') {
          mode = 2;
        }
        if (key == '1') {
          mode = 1;
        }
      }
      werase(game_window);
      display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);
      sleep(1);
    } else if (key == 'q') {
      cont = 0;
      break;
    }
  }
  wattroff(score_window, COLOR_PAIR(2));
  wattroff(game_window, COLOR_PAIR(1));
  wattroff(win, A_BOLD);
  free_ball(ball);

  endwin();
}
