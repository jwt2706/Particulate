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

    // Initialize the grid with default elements
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = Element(); // use use default, since were filling it with air
        }
    }

    // game loop
    bool running = true;
    while (running) {
        // render the grid
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (grid[y][x].isMovable()) {
                    mvwaddch(playwin, y, x, ' '); // empty space
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
                int px = WIDTH / 2;
                int py = HEIGHT / 2;
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
