#include <ncurses.h>

bool isColorSupported() {
    return COLORS >= 256;
}

// inits all 256 color with a black background, for easy use
void initColorPairs() {    
    for (int c = 0; c < COLORS; ++c) {
        init_pair(c, c, COLOR_BLACK);
    }

}
