#include "globals.h"
#include "grid.h"
#include "color.h"
#include "transformation.h"

void initGrid() {
    // get screen demensions
	getmaxyx(stdscr, termHeight, termWidth);

    grid = new Element*[termHeight];
    for (int i = 0; i < termHeight; ++i) {
        grid[i] = new Element[termWidth];
    }

    // initialize the grid with default elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            grid[y][x] = Element::fromName("air"); // fill grid with air
        }
    }
}

void renderGrid() {
    // draw border
    box(playwin, 0, 0);
    mvwprintw(playwin, 0, 2, "| Particulate %s | Resolution: %dpx * %dpx | FPS: %d | Pause (p) | Inventory (i) |", version, termHeight, termWidth, fps);
    
    // print hotbar contents
    mvwprintw(playwin, termHeight - 1, 2, "| ");
    for (size_t i = 0; i < hotbar.size(); ++i) {
        // get hotbar element info
        int elementId = hotbar[i];
        const Element& hotbarElement = Element::getAllElements()[elementId];
        int color = hotbarElement.getColor();

        // if the hotbar element is currently selected, highlight it
        if (i == selectedHotbarIndex) {
            wattron(playwin, A_REVERSE); // highlight selected hotbar element
        } else {
            wattroff(playwin, A_REVERSE);
        }

        // render hotbar element
        wattron(playwin, COLOR_PAIR(color));
        mvwprintw(playwin, termHeight - 1, 4 + i * 6, "(%zu) %c ", (i + 1) % 10, hotbarElement.getAscii());
        wattroff(playwin, COLOR_PAIR(color));
    }
    mvwprintw(playwin, termHeight - 1, 4 + hotbar.size() * 6, "| Selected: (%d, %d) |", selectedX, selectedY);

    // render the grid of elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {

            if (x == selectedX && y == selectedY) {
                wattron(playwin, A_REVERSE); // highlight the selected cell
            } else {
                wattroff(playwin, A_REVERSE);
            }

            // apply the color pair and render the ascii char
            int color = grid[y][x].getColor();
            wattron(playwin, COLOR_PAIR(color));
            mvwaddch(playwin, y, x, grid[y][x].getAscii());
            wattroff(playwin, COLOR_PAIR(color));
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
            grid[y][x] = Element::fromName("air");
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
                newGrid[y][x] = Element::fromName("air");
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
    // apply all simulation logic through transformation rules once per frame.
    beginTransformationFrame();
    for (int y = termHeight - BORDER_SIZE - 1; y >= BORDER_SIZE; --y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            applyTransformation(y, x);
        }
    }
}
