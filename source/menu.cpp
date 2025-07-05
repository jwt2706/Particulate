#include <ncurses.h>
#include <vector>
#include <string>
#include <cstring>
#include "../include/menu.h"
#include "../include/save.h"
#include "../include/grid.h"
#include "../include/globals.h"
#include "../include/generate.h"
#include "../include/color.h"

int menu(const char* msg, const std::vector<std::string>& options) {
    nodelay(stdscr, FALSE);
    int selected = 0;
    int numOptions = options.size();

    while (true) {
        clear();

        attron(A_BOLD);
        mvprintw(0, 0, "%s", msg);
        attroff(A_BOLD);

        for (int i = 0; i < numOptions; ++i) {
            if (i == selected) {
                attron(A_REVERSE); // highlight selected option
            }
            mvprintw(i + 1, 0, "  %s", options[i].c_str());
            if (i == selected) {
                attroff(A_REVERSE); // remove highlight when unselected
            }
        }
        mvprintw(numOptions + 2, 0, "Press ESC to return.");
        refresh();

        int key = getch();
        if (key == KEY_UP) {
            selected = (selected - 1 + numOptions) % numOptions; // go up
        } else if (key == KEY_DOWN) {
            selected = (selected + 1) % numOptions; // go down
        } else if (key == '\n') {
            nodelay(stdscr, TRUE);
            return selected; // return selected option index
        } else if (key == 27) { // 'q' or ESC to exit
            nodelay(stdscr, TRUE);
            return -1; // exit menu
        }
    }
}

bool confirm(const char* question, const char* message) {
    int answer = menu(question, {"Yes", "No"});
    if (answer == 0) { // user confirmed
        nodelay(stdscr, FALSE); // block until user confirms (or doesn't)
        if (message) {
            clear();
            mvprintw(0, 0, "%s Press any key to continue.", message);
            refresh();
            getch(); // wait for user to acknowledge
        }
        nodelay(stdscr, TRUE); // resume non-blocking
        return true;
    }
    return false; // user declined
}

void splashMenu() {
    clear();

    const char* asciiArt[] = {
        "Welcome to                                        ",
        " _____           _   _            _       _       ",
        "|  __ \\         | | (_)          | |     | |      ",
        "| |__) |_ _ _ __| |_ _  ___ _   _| | __ _| |_ ___ ",
        "|  ___/ _` | '__| __| |/ __| | | | |/ _` | __/ _ \\",
        "| |  | (_| | |  | |_| | (__| |_| | | (_| | ||  __/",
        "|_|   \\__,_|_|   \\__|_|\\___|\\__,_|_|\\__,_|\\__\\___|"
    };

    int numLines = sizeof(asciiArt) / sizeof(asciiArt[0]);
    int startHeight = ((termHeight - numLines) / 2) - 3;

    // display ascii art, centered
    attron(A_BOLD);
    for (int i = 0; i < numLines; ++i) {
        // apply the color pair and render the ascii char
        int colorPairID = getColorPairID(i, COLOR_BLACK);
        attron(COLOR_PAIR(colorPairID));
        mvprintw(startHeight + i, (termWidth - strlen(asciiArt[i])) / 2, "%s", asciiArt[i]);
        attroff(COLOR_PAIR(colorPairID));
    }
    attroff(A_BOLD);
    mvprintw(startHeight + numLines, (termWidth - strlen(asciiArt[5])) / 2, "Developed by %s", authors);
    mvprintw(startHeight + numLines, ((termWidth + strlen(asciiArt[5])) / 2) - strlen(version), "%s", version);
    
    int selected = 0;
    std::vector<std::string> options = {"Start Game", "Quit Game"};
    while (true) {
        // render the menu options side by side
        for (int i = 0; i < options.size(); ++i) {
            int optionX = (termWidth - 25) / 2 + i * 12;
            if (i == selected) {
                attron(A_REVERSE); // highlight selected option
            }
            mvprintw(startHeight + numLines + 2, optionX, "%s", options[i].c_str());
            if (i == selected) {
                attroff(A_REVERSE); // remove highlight when unselected
            }
        }
        refresh();
    
        int key = getch();
        if (key == KEY_RIGHT) {
            selected = (selected + 1) % options.size(); // move right
        } else if (key == KEY_LEFT) {
            selected = (selected - 1 + options.size()) % options.size(); // move left
        } else if (key == '\n') {
            if (selected == 0) { // start game
                nodelay(stdscr, TRUE); // resume non-blocking
                mainMenu();
                break;
            } else if (selected == 1) { // quit game
                freeGrid();
                endwin();
                exit(0);
            }
        }
    }
    refresh();

    // wait for user input
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

void mainMenu() {
    clear();

    std::vector<std::string> options = {
        "Resume Game",
        "New Game",
        "Save Game",
        "Load Game",
        "Return Home"
    };
    int selectedOption = menu("Game Menu. Select an option:", options);
            
    if (selectedOption == 0) { // resume game
        ; // return to game
    } else if (selectedOption == 1) { // new game
        newMenu();
    } else if (selectedOption == 2) { // save game
        saveMenu();
    } else if (selectedOption == 3) { // load game
        loadMenu();
    } else if (selectedOption == 4) { // quit game
        splashMenu();
    }
}

void newMenu() {

    std::vector<std::string> options = {
        "Empty Map",
        "Generate Random Map",
    };

    int selectedOption = menu("Select a new game type:", options);

    if (confirm("Are you sure you want to start a new game? Any unsaved progress will be lost.")) {
        if (selectedOption == 0) { // empty map
            clearGrid();
        } else if (selectedOption == 1) { // generate map
            clearGrid();
            generateMap();
        } else {
            mainMenu(); // return to main menu if no valid option selected
        }
    } else {
        mainMenu(); // return to main menu if user declined
    }
}

void saveMenu() {
    clear();

    std::vector<std::string> saveFiles = getSaveFiles();
    saveFiles.push_back("+ Create New Save");
    int selectedFileIndex = menu("Select a save slot:", saveFiles);

    if (selectedFileIndex == -1) {
        mainMenu();
    } else if (selectedFileIndex == saveFiles.size() - 1) {
        // User chose to create a new save
        clear();
        nodelay(stdscr, FALSE); // block until user confirms (or doesn't)
        mvprintw(0, 0, "Enter save slot name (if it already exitst, it will overwrite): ");
        refresh();

        char saveSlot[256];
        echo(); // Enable user input
        getstr(saveSlot); // Get save slot name
        noecho(); // Disable user input

        saveGame(std::string(saveSlot) + ".txt"); // Save to file
        clear();
        mvprintw(0, 0, "Game saved to %s.txt", saveSlot);
        nodelay(stdscr, TRUE); // resume non-blocking
        refresh();
    } else {
        saveGame(saveFiles[selectedFileIndex]); // Save to selected file
        clear();
        mvprintw(0, 0, "Game saved to %s", saveFiles[selectedFileIndex].c_str());
        refresh();
    }
}

void loadMenu() {
    clear();

    std::vector<std::string> saveFiles = getSaveFiles();
    if (saveFiles.empty()) {
        mvprintw(0, 0, "No save files found. Go create one!");
        refresh();
        getch();
        mainMenu();
        return;
    } else {
        int selectedFileIndex = menu("Select a save file to load:", saveFiles);

        if (selectedFileIndex == -1) {
            mainMenu();
            return;
        }

        if (confirm("Are you sure you want to load this game? All unsaved progress will be lost.")) {
            loadGame(saveFiles[selectedFileIndex]);
        }
    }
}