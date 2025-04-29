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
	int termHeight, termWidth;
	getmaxyx(stdscr, termHeight, termWidth);
	// create a window for our input
	WINDOW * playwin = newwin(termHeight, termWidth, 0, 0);
	box(playwin, 0, 0);
    mvwprintw(playwin, 0, 2, " Powder Game - Press 'q' to quit ");
	wrefresh(playwin);

    // Establish coordinates within window
    Element grid[termHeight][termWidth];

    // Initialize the grid with default elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            grid[y][x] = Element::air(); // fill grid with air
        }
    }

    // game loop
    // todo

    getch(); // Wait for user input before exiting
    endwin();
    return 0;
}
