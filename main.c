#include <ncurses.h>

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	printw("hello world");
	refresh();

	getch();
	endwin();
	
	return 0;
}
