#include <filesystem>
#include <algorithm>
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
#include "rules.h"
#include "selection.h"

namespace {
int clampHotbarIndex(int index) {
    if (hotbar.empty()) {
        return 0;
    }
    return clampValue(index, 0, static_cast<int>(hotbar.size()) - 1);
}
}

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
    initializeRules(); // load transformation rules
}

int main() {
    setup(); // setup ncurses and game
    splashMenu(); // start the game on the splash screen

    // game loop
    MEVENT event;
    while (true) {
        // recompute sleep per-frame to reflect dynamic fps changes
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1000000000 / std::max(1, fps); // set the frame rate, avoid div by zero
        int moveX = 0;
        int moveY = 0;
        bool placeRequested = false;
        bool openInventory = false;
        bool openMenu = false;

        // drain all pending input this frame so held keys do not create delayed backlog
        int ch = ERR;
        while ((ch = getch()) != ERR) {
            switch (ch) {
                case KEY_UP:
                    moveY--;
                    break;
                case KEY_DOWN:
                    moveY++;
                    break;
                case KEY_LEFT:
                    moveX--;
                    break;
                case KEY_RIGHT:
                    moveX++;
                    break;
                case 'i':
                    openInventory = true;
                    break;
                case 'p':
                case 27: // 'p' or ESC to pause the game
                    openMenu = true;
                    break;
                case 10: // ENTER key to place element
                    placeRequested = true;
                    break;
                case '+': // increase simulation speed
                case '=':
                    fps = std::min(240, fps + 1);
                    break;
                case '-': // decrease simulation speed
                    fps = std::max(1, fps - 1);
                    break;
                case ']': // increase brush size
                    brushSize = std::min(10, brushSize + 1);
                    break;
                case '[': // decrease brush size
                    brushSize = std::max(0, brushSize - 1);
                    break;
                case KEY_MOUSE:
                    if (getmouse(&event) == OK) {
                        selectedX = event.x;
                        selectedY = event.y;
                        clampSelectionToGrid();

                        // spawn from mouse only once per frame even if many mouse events queue up
                        if (event.bstate & (BUTTON1_PRESSED | BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED | BUTTON1_TRIPLE_CLICKED)) {
                            placeRequested = true;
                        }
                    }
                    break;
                default:
                    // handle hotbar keys
                    if (ch >= '0' && ch <= '9') {
                        int index = (ch == '0') ? (hotbar.size() - 1) : (ch - '1');
                        selectedHotbarIndex = clampHotbarIndex(index);
                    }
                    break;
            }
        }

        if (openMenu) {
            mainMenu();
        } else if (openInventory) {
            inventory();
        }

        selectedX += moveX;
        selectedY += moveY;
        clampSelectionToGrid();

        if (placeRequested) {
            selectedHotbarIndex = clampHotbarIndex(selectedHotbarIndex);
            int elemId = hotbar[selectedHotbarIndex];
            // apply brush: radius = brushSize (0 = single cell)
            for (int dy = -brushSize; dy <= brushSize; ++dy) {
                for (int dx = -brushSize; dx <= brushSize; ++dx) {
                    int tx = selectedX + dx;
                    int ty = selectedY + dy;
                    // clamp target to playable area
                    tx = clampValue(tx, BORDER_SIZE, termWidth - BORDER_SIZE - 1);
                    ty = clampValue(ty, BORDER_SIZE, termHeight - BORDER_SIZE - 1);
                    grid[ty][tx] = Element::fromId(elemId);
                }
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
