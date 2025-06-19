#include "../include/globals.h"
#include "../include/grid.h"

void renderGrid() {
    // draw border
    box(playwin, 0, 0);
    mvwprintw(playwin, 0, 2, "| Particulate v0.0.1 | Resolution: %dpx * %dpx | FPS: %d | Pause (p) |", termHeight, termWidth, fps);
    mvwprintw(playwin, termHeight - 1, 2, "| Selected: (%d, %d) | Elements: (w)ater, (s)and, d(irt), (f)ire, (g)rass, (a)ir, r(ock) |", selectedX, selectedY);

    // render the grid of elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {

            if (x == selectedX && y == selectedY) {
                wattron(playwin, A_REVERSE); // highlight the selected cell
            } else {
                wattroff(playwin, A_REVERSE);
            }

            // init color pair with unique color pair ID (using cantoring pairing function)
            int fg = grid[y][x].getFGColor();
            int bg = grid[y][x].getBGColor();
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;

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

void clearGrid() {
    // clear the grid and reset all elements to air
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            grid[y][x] = Element::air();
        }
    }
    renderGrid();
}

void resizeGrid(int sig) {
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
    renderGrid();
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

    // helper to handle element movement
    auto tryMove = [&](int y, int x, int newY, int newX) {
        if (newY >= BORDER_SIZE && newY < termHeight - BORDER_SIZE &&
            newX >= BORDER_SIZE && newX < termWidth - BORDER_SIZE &&
            grid[newY][newX].getDensity() < grid[y][x].getDensity()) {
            std::swap(newGrid[y][x], newGrid[newY][newX]); // swap elements
            return true;
        }
            return false;
    };

    // Iterate through the grid
    for (int y = termHeight - 2; y >= BORDER_SIZE; --y) { // Bottom to top
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            Element& current = grid[y][x];

            if (current.isMovable()) {
                // handle gravity
                if (current.getGravity()) {
                    if (tryMove(y, x, y + 1, x)) continue; // strait down
                    if (tryMove(y, x, y + 1, x - 1)) continue; // down left
                    if (tryMove(y, x, y + 1, x + 1)) continue; // down right
                } else if (current.getGravity() < 0) {
                    if (tryMove(y, x, y - 1, x)) continue; // strait up
                    if (tryMove(y, x, y - 1, x - 1)) continue; // up left
                    if (tryMove(y, x, y - 1, x + 1)) continue; // up right
                }

                // make water flatten out
                if (current.getName() == "water") {
                    int dir = rand() % 3; // 0 = left, 1 = right, 2 = none
                
                    switch (dir) {
                        case 0: // try to go to the left
                            if (tryMove(y, x, y, x - 1)) continue;
                            break;
                        case 1: // try to go to the right
                            if (tryMove(y, x, y, x + 1)) continue;
                            break;
                        case 2: // dont move
                            break;
                    }
                }

                // handle fire burning
                if (current.getName() == "fire") {
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            int newY = y + dy;
                            int newX = x + dx;
                            if (newY >= BORDER_SIZE && newY < termHeight - BORDER_SIZE &&
                                newX >= BORDER_SIZE && newX < termWidth - BORDER_SIZE &&
                                grid[newY][newX].isFlammable()) {
                                newGrid[newY][newX] = Element::fire(); // Turn flammable element into fire
                            }
                        }
                    }
                }
                
            }
        }
    }

    // Replace the old grid with the updated grid
    freeGrid();
    grid = newGrid;
}