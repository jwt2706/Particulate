#include <ncurses.h>
#include "include/global.h"
#include "include/element.h"
#include <iostream>
#include <ostream>

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    refresh();
    start_color();

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

    grid[1][1] = Element::water();
    grid[1][2] = Element::dirt();
    grid[1][3] = Element::sand();
    grid[1][4] = Element::fire();
    grid[1][5] = Element::stone();
    grid[1][6] = Element::grass();

    grid[4][1] = Element::dirt();
    grid[3][1] = Element::grass();

    // Render the grid
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            // Determine the color pair based on the element's foreground and background colors

            // init color pair with unique color pair ID (using cantoring pairing function)
            int fg = grid[y][x].getFGColor();
            int bg = grid[y][x].getBGColor();
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;
            init_pair(colorPairID, grid[y][x].getFGColor(), grid[y][x].getBGColor());

            // Apply the color pair and render the ASCII character
            wattron(playwin, COLOR_PAIR(colorPairID));
            mvwaddch(playwin, y, x, grid[y][x].getAscii());
            wattroff(playwin, COLOR_PAIR(colorPairID));
        }
    }

    // Refresh the window to display the changes
    wrefresh(playwin);


    // game loop
    // todo

    getch(); // Wait for user input before exiting
    endwin();
    return 0;
}
