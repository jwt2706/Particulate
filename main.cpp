#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <signal.h>
#include <ncurses.h>
#include "include/globals.h"
#include "include/element.h"
#include "include/grid.h"
#include "include/save.h"
#include "include/menu.h"

void initColorPairs() {
    for (int fg = 0; fg < 8; ++fg) {
        for (int bg = 0; bg < 8; ++bg) {
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;
            init_pair(colorPairID, fg, bg);
        }
    }
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak(); // allow instant key input
    keypad(stdscr, TRUE); // enable special keys (like arrow keys)
    nodelay(stdscr, TRUE); // makes getch() non-blocking
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
                clear();

                std::vector<std::string> options = {
                    "Resume Game",
                    "Save Game",
                    "Load Game",
                    "Reset Game",
                    "Quit Game"
                };
                int selectedOption = menu("Game Paused. Select an option:", options);
                        
                if (selectedOption == 0) { // resume game
                    break;

                } else if (selectedOption == 1) { // save game
                    clear();

                    std::vector<std::string> saveFiles = getSaveFiles();
                    saveFiles.push_back("+ Create New Save");

                    int selectedFileIndex = menu("Select a save slot:", saveFiles);

                    if (selectedFileIndex == saveFiles.size() - 1) {
                        // User chose to create a new save
                        clear();
                        mvprintw(0, 0, "Enter save slot name (if it already exitst, it will overwrite): ");
                        refresh();

                        char saveSlot[256];
                        echo(); // Enable user input
                        getstr(saveSlot); // Get save slot name
                        noecho(); // Disable user input

                        saveGame(std::string(saveSlot) + ".txt"); // Save to file
                        clear();
                        mvprintw(0, 0, "Game saved to %s.txt", saveSlot);
                        refresh();
                    } else {
                        saveGame(saveFiles[selectedFileIndex]); // Save to selected file
                        clear();
                        mvprintw(0, 0, "Game saved to %s", saveFiles[selectedFileIndex].c_str());
                        refresh();
                    }
                    break;

                } else if (selectedOption == 2) { // load game
                    clear();

                    std::vector<std::string> saveFiles = getSaveFiles();
                    if (saveFiles.empty()) {
                        mvprintw(0, 0, "No save files found. Go create one!");
                        refresh();
                        getch();
                    } else {
                        int selectedFileIndex = menu("Select a save file to load:", saveFiles);

                        if (confirm("Are you sure you want to load this game? All unsaved progress will be lost.")) {
                            loadGame(saveFiles[selectedFileIndex]);
                        }
                    }
                    break;

                } else if (selectedOption == 3) { // reset game
                    // confirm before resetting the game, if confirmed then reset the grid to air
                    if (confirm("Are you sure you want to reset the game? Unsaved progress will be lost.")) {
                        clearGrid();
                    }
                    break;

                } else if (selectedOption == 4) { // quit game
                    running = false;
                    break;

                }
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
