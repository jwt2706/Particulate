#ifndef GRID_H
#define GRID_H

#include <ncurses.h>
#include "element.h"

void renderGrid();
void freeGrid();
void clearGrid();
void resizeGrid(int sig);
void updateGrid();

#endif // GRID_H