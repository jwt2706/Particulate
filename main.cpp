#include <time.h>
#include <signal.h>
#include <ncurses.h>
#include "include/globals.h"
#include "include/element.h"
#include "include/grid.h"
#include "include/save.h"

void initColorPairs() {
    for (int fg = 0; fg < 8; ++fg) {
        for (int bg = 0; bg < 8; ++bg) {
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;
            init_pair(colorPairID, fg, bg);
        }
    }
}

bool confirm(const char* message) {
    clear();
    mvprintw(0, 0, "%s (y/n) ", message);
    refresh();
    int ch = getch();
    return (ch == 'y' || ch == 'Y');
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    keypad(stdscr, TRUE); // enable special keys (like arrow keys)
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

    initColorPairs(); // initialize color pairs
    playwin = newwin(termHeight, termWidth, 0, 0); // create window for user
    renderGrid();

    signal(SIGWINCH, resizeGrid); // handle window resize dynamically

    // game loop
    bool running = true;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000000 / fps; // set the frame rate
    nodelay(stdscr, TRUE); // makes getch() non-blocking
    while (running) {
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
            case 'w': {
                grid[selectedY][selectedX] = Element::water();
                break;
            }
            case 's': {
                grid[selectedY][selectedX] = Element::sand();
                break;
            }
            case 'd': {
                grid[selectedY][selectedX] = Element::dirt();
                break;
            }
            case 'f': {
                grid[selectedY][selectedX] = Element::fire();
                break;
            }
            case 'g': {
                grid[selectedY][selectedX] = Element::grass();
                break;
            }
            case 'a': {
                grid[selectedY][selectedX] = Element::air();
                break;
            }
            case 'r': {
                grid[selectedY][selectedX] = Element::rock();
                break;
            }
            case 'p': {
                nodelay(stdscr, FALSE); // block until user input
                clear();

                const char* options[] = {
                    "Resume Game",
                    "Save Game",
                    "Load Game",
                    "Reset Game",
                    "Quit Game"
                };
                const int numOptions = sizeof(options) / sizeof(options[0]);
                int selectedOption = 0;

                while (true) {
                    clear();
                    mvprintw(0, 0, "Game Paused! Use arrow keys to selected what you want to do.");
                    for (int i = 0; i < numOptions; ++i) {
                        if (i == selectedOption) {
                            attron(A_REVERSE); // highlight selected option
                        }
                        mvprintw(i + 1, 0, "%s", options[i]);
                        if (i == selectedOption) {
                            attroff(A_REVERSE); // remove highlight
                        }
                    }
                    refresh();

                    int key = getch();
                    if (key == KEY_UP) {
                        selectedOption = (selectedOption - 1 + numOptions) % numOptions; // go up
                    } else if (key == KEY_DOWN) {
                        selectedOption = (selectedOption + 1) % numOptions; // go down
                    } else if (key == '\n') {
                        if (selectedOption == 0) {
                            break; // resume game
                        } else if (selectedOption == 1) {
                            clear();
                            mvprintw(0, 0, "Enter save slot name (e.g., save1): ");
                            refresh();

                            char saveSlot[256];
                            echo(); // Enable user input
                            getstr(saveSlot); // Get save slot name
                            noecho(); // Disable user input

                            saveGame(std::string(saveSlot) + ".txt"); // Save to file
                            clear();
                            mvprintw(0, 0, "Game saved to %s.txt", saveSlot);
                            refresh();
                            getch();
                            break;

                        } else if (selectedOption == 2) {
                            clear();
                            mvprintw(0, 0, "Enter load slot name (e.g., load1): ");
                            refresh();

                            char loadSlot[256];
                            echo(); // Enable user input
                            getstr(loadSlot); // Get load slot name
                            noecho(); // Disable user input

                            loadGame(std::string(loadSlot) + ".txt"); // Load from file
                            clear();
                            mvprintw(0, 0, "Game loaded from %s.txt", loadSlot);  
                            refresh();
                            getch();
                            break;

                        } else if (selectedOption == 3) {
                            // confirm before resetting the game, if confirmed then reset the grid to air
                            if (confirm("Are you sure you want to reset the game? Unsaved progress will be lost.")) {
                                clearGrid();
                            }
                            break;
                        } else if (selectedOption == 4) {
                            running = false; // quit game
                            break;
                        }
                    }
                }

                nodelay(stdscr, TRUE); // resume non-blocking input
                break;

            }
            default:
                break;
        }

        updateGrid();
        renderGrid();
        nanosleep(&ts, NULL); // sleep for the specified time to control the frame rate
    }

    freeGrid();
    endwin();
    return 0;
}
