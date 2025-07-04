#include <ncurses.h>
#include <vector>
#include <string>
#include "../include/menu.h"
#include "../include/save.h"
#include "../include/grid.h"
#include "../include/generate.h"

int menu(const char* msg, const std::vector<std::string>& options) {
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
            return selected; // return selected option index
        } else if (key == 27) { // 'q' or ESC to exit
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

bool mainMenu() {
    clear();

    std::vector<std::string> options = {
        "Resume Game",
        "New Game",
        "Save Game",
        "Load Game",
        "Quit Game"
    };
    int selectedOption = menu("Game Menu. Select an option:", options);
            
    if (selectedOption == 0) { // resume game
        ; // return to game
    } else if (selectedOption == 1) { // new game
        newMenu();
        return true;
    } else if (selectedOption == 2) { // save game
        saveMenu();
        return true;
    } else if (selectedOption == 3) { // load game
        loadMenu();
        return true;
    } else if (selectedOption == 4) { // quit game
        return false;
    }
    return true;
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