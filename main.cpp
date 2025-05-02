#include <time.h>
#include <signal.h>
#include <ncurses.h>
#include "include/element.h"

int BORDER_SIZE = 1;
WINDOW* playwin; // global window pointer
int fps = 15; // frames per second
int termHeight, termWidth; // global terminal dimensions
int selectedX, selectedY; // selected coordinates by mouse
Element** grid; // global grid pointer

void renderFullGrid() {
    // draw border
    box(playwin, 0, 0);
    mvwprintw(playwin, 0, 2, "| Particulate v0.0.1 | Resolution: %dpx * %dpx | FPS: %d | Pause (p) |", termHeight, termWidth, fps);
    mvwprintw(playwin, termHeight - 1, 2, "| Selected: (%d, %d) | Elements: (w)ater, (s)and, d(irt), (f)ire, (g)rass, (a)ir, r(ock) ", selectedX, selectedY);

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

void freeGrid() {
    // free the grid memory
    for (int i = 0; i < termHeight; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

void resizeHandler(int sig) {
    endwin();
    refresh();
    resize_term(0, 0);
    int newHeight, newWidth;
    getmaxyx(stdscr, newHeight, newWidth); // get new terminal dimensions

    // Create a new grid with updated dimensions
    Element** newGrid = new Element*[newHeight];
    for (int i = 0; i < newHeight; ++i) {
        newGrid[i] = new Element[newWidth];
    }

    // Copy existing elements to the new grid
    for (int y = BORDER_SIZE; y < std::min(termHeight, newHeight) - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < std::min(termWidth, newWidth) - BORDER_SIZE; ++x) {
            newGrid[y][x] = grid[y][x];
        }
    }

    // Initialize new cells with air
    for (int y = BORDER_SIZE; y < newHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < newWidth - BORDER_SIZE; ++x) {
            if (y >= termHeight || x >= termWidth) {
                newGrid[y][x] = Element::air();
            }
        }
    }

    // Free the old grid memory
    freeGrid();

    // Update global variables and grid pointer
    termHeight = newHeight;
    termWidth = newWidth;
    grid = newGrid;


    // recreate the window
    playwin = newwin(termHeight, termWidth, 0, 0);
    wclear(playwin);
    wresize(playwin, termHeight, termWidth);
    renderFullGrid();
}

void updateGrid() {
    // Create a temporary grid to store updates
    Element** newGrid = new Element*[termHeight];
    for (int i = 0; i < termHeight; ++i) {
        newGrid[i] = new Element[termWidth];
        for (int j = 0; j < termWidth; ++j) {
            newGrid[i][j] = grid[i][j]; // Copy the current grid
        }
    }

    // Iterate through the grid
    for (int y = termHeight - 2; y >= BORDER_SIZE; --y) { // Bottom to top
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            Element& current = grid[y][x];

            // Skip immovable elements
            if (!current.isMovable()) {
                continue;
            }

            // Gravity-based movement (falling down)
            if (current.getGravity() > 0) {
                // Check if the cell below is empty or less dense
                if (y + 1 < termHeight - BORDER_SIZE && grid[y + 1][x].getDensity() < current.getDensity()) {
                    newGrid[y + 1][x] = current;
                    newGrid[y][x] = Element::air();
                    continue;
                }

                // Check for diagonal movement if blocked below
                if (y + 1 < termHeight - BORDER_SIZE) {
                    if (x - 1 >= BORDER_SIZE && grid[y + 1][x - 1].getDensity() < current.getDensity()) {
                        newGrid[y + 1][x - 1] = current;
                        newGrid[y][x] = Element::air();
                        continue;
                    }
                    if (x + 1 < termWidth - BORDER_SIZE && grid[y + 1][x + 1].getDensity() < current.getDensity()) {
                        newGrid[y + 1][x + 1] = current;
                        newGrid[y][x] = Element::air();
                        continue;
                    }
                }
            }

            // Fire behavior (burning flammable elements)
            if (current.isFlammable()) {
                if (y + 1 < termHeight - BORDER_SIZE && grid[y + 1][x].isFlammable()) {
                    newGrid[y + 1][x] = Element::fire();
                }
                if (x - 1 >= BORDER_SIZE && grid[y][x - 1].isFlammable()) {
                    newGrid[y][x - 1] = Element::fire();
                }
                if (x + 1 < termWidth - BORDER_SIZE && grid[y][x + 1].isFlammable()) {
                    newGrid[y][x + 1] = Element::fire();
                }
                // Fire disappears after burning
                newGrid[y][x] = Element::air();
            }
        }
    }

    // Replace the old grid with the updated grid
    freeGrid();
    grid = newGrid;
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // enable mouse events
    start_color();

	// get screen demensions
	getmaxyx(stdscr, termHeight, termWidth);

    grid = new Element*[termHeight];
    for (int i = 0; i < termHeight; ++i) {
        grid[i] = new Element[termWidth];
    }

    // initialize the grid with default elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            grid[y][x] = Element::air(); // fill grid with air
        }
    }

    // add some random elelments
    grid[1][1] = Element::water();


    // create window for input
    playwin = newwin(termHeight, termWidth, 0, 0);
    renderFullGrid();

    signal(SIGWINCH, resizeHandler); // handle window resize dynamically

    // game loop
    bool running = true;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000000 / fps; // set the frame rate
    nodelay(stdscr, TRUE); // makes getch() non-blocking
    while (running) {
        int ch = getch(); // user input

        switch (ch) {
            case KEY_MOUSE: {
                MEVENT event;
                if (getmouse(&event) == OK) {
                    selectedX = event.x;
                    selectedY = event.y;
                }
            }
            case 'p': {
                nodelay(stdscr, FALSE); // block until user input
                clear();
                mvprintw(0, 0, "Game Paused!");
                mvprintw(1, 0, "Resume (r) or Quit (q)?");
                refresh();

                int confirm = getch();
                if (confirm == 'q' || confirm == 'Q') {
                    running = false;
                } else if (confirm == 'r' || confirm == 'R') {
                    renderFullGrid();
                }
                nodelay(stdscr, TRUE); // resume non-blocking input
                break;

            }
            case 'w': {
                grid[1][1] = Element::water();
                break;
            }
            case 's': {
                grid[1][1] = Element::sand();
                break;
            }
            case 'd': {
                grid[1][1] = Element::dirt();
                break;
            }
            case 'f': {
                grid[1][1] = Element::fire();
                break;
            }
            case 'g': {
                grid[1][1] = Element::grass();
                break;
            }
            case 'a': {
                grid[1][1] = Element::air();
                break;
            }
            case 'r': {
                grid[1][1] = Element::rock();
                break;
            }
            default:
                break;
        }

        updateGrid();
        renderFullGrid();
        nanosleep(&ts, NULL); // sleep for the specified time to control the frame rate
    }

    freeGrid();
    endwin();
    return 0;
}
