#include "../include/globals.h"

int BORDER_SIZE = 1;
WINDOW* playwin = nullptr;
int fps = 15;
int termHeight = 0, termWidth = 0;
int selectedX = 1, selectedY = 1;
Element** grid = nullptr;