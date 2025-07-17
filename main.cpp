#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <signal.h>
#include <ncurses.h>
#include "include/globals.h"
#include "include/element.h"
#include "include/grid.h"
#include "include/save.h"
#include "include/menu.h"
#include "include/color.h"
#include "include/inventory.h"

void setup() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    set_escdelay(0); // disable escape delay
    keypad(stdscr, TRUE); // enable special keys (like arrow keys)
    nodelay(stdscr, TRUE); // makes getch() non-blocking
    start_color();

    if (!isColorSupported()) {
        nodelay(stdscr, FALSE);
        mvprintw(0, 0, "Your terminal does not support 256 color. It's required for Particulate to run properly. Press any key to exit.");
        getch();
        endwin();
        exit(EXIT_FAILURE);
    }

    initGrid();
    initColorPairs();
    playwin = newwin(termHeight, termWidth, 0, 0); // create window for user
    signal(SIGWINCH, resizeGrid); // handle window resize dynamically
}

int main() {
    setup(); // setup ncurses and game
    splashMenu(); // start the game on the splash screen

    // game loop
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000000 / fps; // set the frame rate
    while (true) {
        MEVENT event;
        int ch = getch(); // user input

        switch (ch) {
            case KEY_UP: {
                if (selectedY > BORDER_SIZE) {
                    selectedY--;
                }
                break;
            }
            case KEY_DOWN: {
                if (selectedY < termHeight - BORDER_SIZE - 1) {
                    selectedY++;
                }
                break;
            }
            case KEY_LEFT: {
                if (selectedX > BORDER_SIZE) {
                    selectedX--;
                }
                break;
            }
            case KEY_RIGHT: {
                if (selectedX < termWidth - BORDER_SIZE - 1) {
                    selectedX++;
                }
                break;
            }
            case 'i': {
                inventory();
                break;
            }
            case 'p' :
            case 27: { // 'p' or ESC to pause the game
                mainMenu();
                break;
            }
            default: {
                // handle hotbar keys
                if (ch >= '1' && ch <= '9') {
                    int index = ch - '1'; // convert char to index
                    grid[selectedY][selectedX] = Element::fromId(hotbar[index]);
                } else if (ch == '0') {
                    grid[selectedY][selectedX] = Element::fromId(hotbar[hotbar.size() - 1]);
                }
                break;
            }
        }

        updateGrid();
        renderGrid();
        nanosleep(&ts, NULL); // sleep for the specified time to control the frame rate
    }

    freeGrid();
    endwin();
    return 0;
}
