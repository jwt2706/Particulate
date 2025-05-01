#include <signal.h>
#include <ncurses.h>
#include "include/global.h"
#include "include/element.h"

WINDOW* playwin; // global window pointer
int termHeight, termWidth; // global terminal dimensions
Element** grid; // global grid pointer

void renderFullGrid(Element** grid) {
    // draw border
    box(playwin, 0, 0);
    mvwprintw(playwin, 0, 2, " Powder Game - Press 'q' to quit ");

    // render the grid of elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            // init color pair with unique color pair ID (using cantoring pairing function)
            int fg = grid[y][x].getFGColor();
            int bg = grid[y][x].getBGColor();
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;
            init_pair(colorPairID, grid[y][x].getFGColor(), grid[y][x].getBGColor());

            // apply the color pair and render the ascii char
            wattron(playwin, COLOR_PAIR(colorPairID));
            mvwaddch(playwin, y, x, grid[y][x].getAscii());
            wattroff(playwin, COLOR_PAIR(colorPairID));
        }
    }

    // refresh the window to display the changes
    wrefresh(playwin);
}

void resizeHandler(int sig) {
    // this is mostly temporary, and should keep the existing elements and react to the termial resizing
    // instead of getting deleted and replaced


    // get new dimentions
    getmaxyx(stdscr, termHeight, termWidth);

    // free old grid
    for (i = 0, i < termHeight; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    // create new grid with updated dimensions
    grid = new Element*[termHeight];
    for (int i = 0; i < termHeight; ++i) {
        grid[i] = new Element[termWidth];
    }

    // initialize the grid with air
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            grid[y][x] = Element::air(); // fill grid with air
        }
    }

    // recreate the window
    wclear(playwin);
    wresize(playwin, termHeight, termWidth);
    renderFullGrid(grid);
}

void updateGrid() {
    return;
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    refresh();
    start_color();

	// get screen demensions
	getmaxyx(stdscr, termHeight, termWidth);

	// create a window for inputs
	playwin = newwin(termHeight, termWidth, 0, 0);
    
    // this grid will hold the elements for the simulation
    Element grid[termHeight][termWidth];

    // initialize the grid with default elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            grid[y][x] = Element::air(); // fill grid with air
        }
    }

    // add some random elelments
    grid[1][1] = Element::water();
    grid[1][2] = Element::dirt();
    grid[1][3] = Element::sand();
    grid[1][4] = Element::fire();
    grid[1][5] = Element::stone();
    grid[1][6] = Element::grass();

    grid[4][1] = Element::dirt();
    grid[3][1] = Element::grass();

    
    
    renderFullGrid(grid);

    signal(SIGWINCH, resizeHandler); // handle window resize dynamically

    // game loop
    bool running = true;
    while (running) {
        int ch = getch(); // user input

        switch (ch) {
            case 'q': {
                clear();
                mvprintw(0, 0, "Do you really want to quit? (y/n)");
                refresh();

                int confirm = getch();
                if (confirm == 'y' || confirm == 'Y') {
                    running = false;
                } else if (confirm == 'n' || confirm == 'N') {
                    renderFullGrid(grid);
                }
                break;

            }
            default:
                break;
        }
    }

    endwin();
    return 0;
}
