#ifndef SELECTION_H
#define SELECTION_H

#include <algorithm>
#include "globals.h"

inline int clampValue(int value, int minValue, int maxValue) {
    return std::max(minValue, std::min(value, maxValue));
}

inline void clampSelectionToGrid() {
    selectedX = clampValue(selectedX, BORDER_SIZE, termWidth - BORDER_SIZE - 1);
    selectedY = clampValue(selectedY, BORDER_SIZE, termHeight - BORDER_SIZE - 1);
}

#endif // SELECTION_H