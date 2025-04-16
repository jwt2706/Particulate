#include <ncurses.h>
#include "include/global.h"
#include "include/element.h"

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // Allow instant key input
    refresh(); // Refresh main screen
    start_color(); // Enable color

	// get screen demensions
	int xMax, yMax;
	getmaxyx(stdscr, yMax, xMax);
	// create a window for our input
	WINDOW * playwin = newwin(HEIGHT, WIDTH, (yMax/2)-10, 10);
	box(playwin, 0, 0);
	wrefresh(playwin);

    // Establish coordinates within window
    Element grid[HEIGHT][WIDTH];

    // Start of game loop
    // (should be here)

    getch(); // Wait for user input before exiting
    endwin();
    return 0;
}
