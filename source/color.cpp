#include <ncurses.h>

// inits all color pairs for easy use, and assigns each with a unique ID
// using the cantor paring function
void initColorPairs() {
    for (int fg = 0; fg < 8; ++fg) {
        for (int bg = 0; bg < 8; ++bg) {
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;
            init_pair(colorPairID, fg, bg);
        }
    }
}

int getColorPairID(int fg, int bg) {
    return (fg + fg)*(fg + bg + 1)/2 + bg;
}