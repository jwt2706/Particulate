#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20
#define WATER '~'
#define EMPTY ' '

class Simulator {
  private:
    std::vector<std::vector<char>> grid;

  public:
    Simulator() : grid(HEIGHT, std::vector<char>(WIDTH, EMPTY)) {}

    // init empty grid
    void init_grid() {
      for (auto &row : grid) {
        std::fill(row.begin(), row.end(), EMPTY);
      }
    }

    // generate water at specified coordinates
    void generate_water(int x, int y) {
      if (x > 0 && WIDTH - 1 && y > 0 && y < HEIGHT - 1) {
        grid[y][x] = WATER;
      }
    }

    // update water state
    void update_water() {
      for (int y = HEIGHT - 2; y >= 0; y--) {
        for (int x = 1; x < WIDTH - 1; x++) {
          for (grid[y][x] == WATER) {
            // if there's nothing below the water, just keep falling down
            if (grid[y + 1][x] == EMPTY) {
              grid[y + 1][x] = WATER;
              grid[y][x] = EMPTY;
            }

            // if there's something below the water, then try to move diagonally
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

    // render grid with ncurses
    void render() {
      clear();
      for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
          mvaddch(y, x, grid[y][x]);
        }
      }
      refresh();
    }
};

int main() {
  initscr();              // init ncurses
  noecho();               // disable key echoing
  curs_set(0);            // hide cursor
  nodelay(stdscr, TRUE);  // non blocking input

  Simulator sim;
  sim.init_grid();

  bool running = true;
  while (running) {
    // get user input
    int ch = getch();
    if (ch == 'q') running = false;

    sim.generate_water(WIDTH / 2, 1);
    sim.update_water();
    sim.render();

    usleep(500000);
  }

  endwin(); // exit ncurses
  return 0;
}
