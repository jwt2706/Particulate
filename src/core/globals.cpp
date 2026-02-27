#include "globals.h"

const char* version = "v0.0.1";
const char* authors = "jwt2706 (https://jthome.net)";
int BORDER_SIZE = 1;
WINDOW* playwin = nullptr;
int fps = 15;
int termHeight = 0, termWidth = 0;
int selectedX = 1, selectedY = 1;
Element** grid = nullptr;
std::string saveFolder = "saves/";
std::vector<int> hotbar = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int selectedHotbarIndex = 0;
