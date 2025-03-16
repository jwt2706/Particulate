#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 40
#define HEIGHT 20
#define WATER '~'
#define EMPTY ' '

char grid[HEIGHT][WIDTH];

// start with an empty grid
void init_grid() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			grid[y][x] = EMPTY;
		}
	}
}

// generate water at specified position
void add_water(int x, int y) {
	if (x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT -1) {
		grid[y][x] = WATER;
	}
}

// update water state
void update_water() {
	for (int y = HEIGHT - 2; y >= 0; y--) {
		for (int x = 1; x < WIDTH - 1; x++) {
			if (grid[y][x] == WATER) {
				// if theres nothing below, fall down
				if (grid[y + 1][x] == EMPTY) {
					grid[y + 1][x] = WATER;
					grid[y][x] = EMPTY;
				}

				// if there is something below, try to move diagonally
				else if (grid[y + 1][x - 1] == EMPTY) {
					grid[y + 1][x - 1] = WATER;
					grid[y][x] = EMPTY;
				}
				else if (grid[y + 1][x + 1] == EMPTY) {
					grid[y + 1][x + 1] = WATER;
					grid[y][x] = EMPTY;
				}
			}
		}
	}
}

// render grid in ncurses
void render() {
	clear();
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			mvaddch(y, x, grid[y][x]);
		}
	}
	refresh();
}

int main() {
	initscr();		// init ncurses
	noecho();		// disable key echoing
	curs_set(0);		// hide cursor
	nodelay(stdscr, TRUE);	// non blocking input
	
	init_grid();

	int running = 1;
	while (running) {
		// get user input
		int ch = getch();
		if (ch == 'q') running = 0;

		add_water(WIDTH / 2, 1);
		update_water();
		render();

		usleep(50000);
	}

	endwin(); // exist ncurses
	return 0;
}
