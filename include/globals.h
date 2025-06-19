#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>
#include "element.h"

// global variables
extern int BORDER_SIZE;
extern WINDOW* playwin;
extern int fps;
extern int termHeight, termWidth;
extern int selectedX, selectedY;
extern Element** grid;

#endif // GLOBALS_H