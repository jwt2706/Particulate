#include <vector>
#include <stack>
#include <utility>
#include <random>
#include "../include/grid.h"
#include "../include/globals.h"
#include "../include/element.h"

void generateMap() {
    clearGrid(); // clear the grid before generating a new map

    // Simple map generation logic: fill with random elements
    for (int y = BORDER_SIZE; y < termHeight - BORDER_SIZE; ++y) {
        for (int x = BORDER_SIZE; x < termWidth - BORDER_SIZE; ++x) {
            int randElement = rand() % 5; // Randomly choose an element type
            switch (randElement) {
                case 0:
                    grid[y][x] = Element::air();
                    break;
                case 1:
                    grid[y][x] = Element::water();
                    break;
                case 2:
                    grid[y][x] = Element::fire();
                    break;
                case 3:
                    grid[y][x] = Element::sand();
                    break;
                case 4:
                    grid[y][x] = Element::rock();
                    break;
            }
        }
    }

    renderGrid(); // render the newly generated map
}
