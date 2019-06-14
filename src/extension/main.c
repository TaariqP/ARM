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
  init_pair(1,COLOR_CYAN, COLOR_BLUE);
  init_pair(2,COLOR_CYAN,COLOR_GREEN);
  wattron(game_window,COLOR_PAIR(1));
  wattron(score_window,COLOR_PAIR(2));

/*initialising local variables*/
  int stick_y_l,
    stick_y_r,
    has_just_quit = 0,
    end = 0,
    cont = 1,
    left_score,
    right_score,
    key = 0;

  ball *ball = create_ball();

  initialise_game(game_window, ball, &left_score, &right_score, &stick_y_l, &stick_y_r);



  print_message_center(game_window, "Are you ready to play pong? (Y/N)", 10);
  print_message_center(game_window, "(For controls, press C)", 12);

  /*starter windows*/
  while (key != 'y') {
    display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);

    key = wgetch(game_window);
    if (key == 'n') {
      end = 1;
      cont = 0;
      break;
    } else if (key == 'c') {
      werase(game_window);
      display_game_state(game_window, score_window, ball, stick_y_l, stick_y_r, left_score, right_score);
      print_message_center(game_window, "Player 1 (right) uses up and down arrow keys to move paddle", 5);
      print_message_center(game_window, "Player 2 (left) uses W and S keys to move paddle", 7);
      print_message_center(game_window, "Press Y to begin game, or N to quit", 9);
      print_message_center(game_window, "Once you have begun the game, press Q to quit", 11);
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

//    display_ball(stick_window, ball_x, ball_y);

      /*behaviour of valid key presses*/
      nodelay(game_window, TRUE);
      key = wgetch(game_window);
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

    print_message_center(game_window, "Press R for rematch or Q to confirm quit", 10);
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
      sleep(1);
    } else if (key == 'q') {
      cont = 0;
      break;
    }
  }
  wattroff(score_window,COLOR_PAIR(2));
  wattroff(game_window,COLOR_PAIR(1));
  free_ball(ball);
  wattroff(win, A_BOLD);
  endwin();
}
