//
// Created by taariq on 6/11/19.
//

#include <ncurses.h>
#include <unistd.h>

int main(void){
    initscr(); // Initialize the window
    noecho(); // Don't echo any keypresses
    curs_set(FALSE); // Don't display a cursor

    sleep(2);

    endwin();
}