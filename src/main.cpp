#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <signal.h>
#include <ncurses.h>
#include "globals.h"
#include "element.h"
#include "grid.h"
#include "save.h"
#include "menu.h"
#include "color.h"
#include "inventory.h"

void setup() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    set_escdelay(0); // disable escape delay
    keypad(stdscr, TRUE); // enable special keys (like arrow keys)
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // enable mouse events
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
    MEVENT event;
    while (true) {
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
            case 10: { // ENTER key to place element
                grid[selectedY][selectedX] = Element::fromId(hotbar[selectedHotbarIndex]);
                break;
            }
            default: {
                // handle hotbar keys
                if (ch >= '0' && ch <= '9') {
                    // convert number key input to int
                    int index = (ch == '0') ? (hotbar.size() - 1) : (ch - '1');
                    selectedHotbarIndex = index;
                }
                break;
            }
        }    
    
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                selectedX = event.x;
                selectedY = event.y;
                grid[selectedY][selectedX] = Element::fromId(hotbar[selectedHotbarIndex]);
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
