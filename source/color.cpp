#include <ncurses.h>

bool isColorSupported() {
    return COLORS >= 256;
}

// inits all color pairs for easy use, and assigns each with a unique ID
// using the cantor paring function
void initColorPairs() {
    
    
    // Initialize pairs 1-256: each foreground color with black background
    for (int fg = 0; fg < COLORS; ++fg) {
        if (fg + 1 < COLOR_PAIRS) {
            init_pair(fg + 1, fg, COLOR_BLACK); // Color on black
        }
    }
    
    // Initialize pairs 257-512: each foreground color with white background
    for (int fg = 0; fg < COLORS; ++fg) {
        if (COLORS + fg + 1 < COLOR_PAIRS) {
            init_pair(COLORS + fg + 1, fg, COLOR_WHITE); // Color on white
        }
    }
    
    /*
    for (int fg = 0; fg < COLORS; ++fg) {
        for (int bg = 0; bg < COLORS; ++bg) {
            int colorPairID = (fg + fg)*(fg + bg + 1)/2 + bg;
            init_pair(colorPairID, fg, bg);
        }
    }
    */
}

int getColorPairID(int fg, int bg) {
    if (bg == COLOR_BLACK || bg == 0) {
        // Foreground color on black background
        return fg + 1;
    } else if (bg == COLOR_WHITE || bg == 15 || bg == 7) {
        // Foreground color on white background
        return COLORS + fg + 1;
    } else {
        // Fallback to black background
        return fg + 1;
    }
    //return fg + 1;
    //return (fg + fg)*(fg + bg + 1)/2 + bg;
}