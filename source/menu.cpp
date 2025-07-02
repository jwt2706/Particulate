#include <ncurses.h>
#include <vector>
#include <string>
#include "../include/menu.h"

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
        refresh();

        int key = getch();
        if (key == KEY_UP) {
            selected = (selected - 1 + numOptions) % numOptions; // go up
        } else if (key == KEY_DOWN) {
            selected = (selected + 1) % numOptions; // go down
        } else if (key == '\n') {
            return selected; // return selected option index
        }
    }
}

bool confirm(const char* question, const char* message) {
    nodelay(stdscr, FALSE); // block until user confirms (or doesn't)

    clear();
    mvprintw(0, 0, "%s (y/n) ", question);
    refresh();
    
    int ch = getch();

    if (ch == 'y' || ch == 'Y') {
        if (message) {
            clear();
            mvprintw(0, 0, "%s Press any key to continue.", message);
            refresh();
            getch(); // wait for user to acknowledge
        }
        nodelay(stdscr, TRUE); // resume non-blocking

        return true;
    }
    nodelay(stdscr, TRUE); // resume non-blocking

    return false;

}