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
            grid[y][x] = Element(); // use use default, since were filling it with air
        }
    }

    // game loop
    bool running = true;
    while (running) {
        // render the grid
        for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
            for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
                if (grid[y][x].isMovable()) {
                    mvwaddch(playwin, y, x, '.'); // empty space
                } else {
                    mvwaddch(playwin, y, x, '#'); // solid block
                }
            }
        }
        wrefresh(playwin);

        // handle user input
        int ch = getch();
        switch (ch) {
            case 'q': // quit
                running = false;
                break;
            case 'p': {
                int px = termWidth / 2;
                int py = termHeight / 2;
                grid[py][px] = Element(true, true, false, 2, 1); // create sand or something
                break;
            }
        }

        // TODO: add logic to update the grid (simulate it all)
    }

    getch(); // Wait for user input before exiting
    endwin();
    return 0;
}
