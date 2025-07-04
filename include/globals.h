#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <ncurses.h>
#include "element.h"

// global variables
extern const char* version;
extern const char* authors;
extern int BORDER_SIZE;
extern WINDOW* playwin;
extern int fps;
extern int termHeight, termWidth;
extern int selectedX, selectedY;
extern Element** grid;
extern std::string saveFolder;

#endif // GLOBALS_H