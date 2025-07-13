#include <ncurses.h>
#include <algorithm>
#include <vector>
#include "../include/element.h"
#include "../include/color.h"
#include "../include/globals.h"
#include "../include/menu.h"

void inventory() {
    nodelay(stdscr, FALSE);

    const std::vector<Element>& elements = Element::getAllElements();
    int numElements = elements.size();
    int selected = 0;

    while (true) {
        clear();
        mvhline(4, 0, ACS_HLINE, termWidth);
        box(stdscr, 0, 0);
        mvprintw(0, 2, "| Inventory Menu | Back To Game (i) | Pause Menu (p) |");
        mvprintw(1, 2, "Hotbar:");
        for (size_t i = 0; i < hotbar.size(); ++i) {
            int elementId = hotbar[i];
            const Element& hotbarElement = elements[elementId];
            int colorPairID = getColorPairID(hotbarElement.getFGColor(), hotbarElement.getBGColor());
            attron(A_BOLD);
            mvprintw(2, 2 + i * 6, "(%zu)", (i + 1) % 10);
            attroff(A_BOLD);
            wattron(stdscr, COLOR_PAIR(colorPairID));
            mvprintw(2, 6 + i * 6, "%c", hotbarElement.getAscii());
            wattroff(stdscr, COLOR_PAIR(colorPairID));
        }

        attron(A_BOLD);
        mvwprintw(stdscr, 5, 2, "Elements:");
        attroff(A_BOLD);
        
        // Display elements
        for (int i = 0; i < numElements; ++i) {
            int y = 7 + i;
        
            // Check if the element is in the hotbar
            bool isInHotbar = std::find(hotbar.begin(), hotbar.end(), elements[i].getId()) != hotbar.end();
        
            if (i == selected) {
                wattron(stdscr, A_REVERSE); // Highlight selected element
            }
        
            if (isInHotbar) {
                attron(A_BOLD); // Make bold if the element is in the hotbar
            }
        
            mvprintw(y, 2, "%i. %s", elements[i].getId(), elements[i].getName().c_str());
            int colorPairID = getColorPairID(elements[i].getFGColor(), elements[i].getBGColor());
            wattron(stdscr, COLOR_PAIR(colorPairID));
            mvprintw(y, 12, "%c", elements[i].getAscii());
            wattroff(stdscr, COLOR_PAIR(colorPairID));
        
            if (isInHotbar) {
                attroff(A_BOLD); // Remove bold attribute after rendering
            }
        
            if (i == selected) {
                wattroff(stdscr, A_REVERSE); // Remove highlight
            }
        }

        // display properties
        const Element& selectedElement = elements[selected];
        attron(A_BOLD);
        mvwprintw(stdscr, 5, termWidth / 2, "Properties:");
        attroff(A_BOLD);
        
        // align property names and data
        int propertyNameCol = termWidth / 2; // column for property names
        int propertyDataCol = termWidth / 2 + 12; // column for property data

        mvprintw(7, propertyNameCol, "%s", selectedElement.getName().c_str());
        mvprintw(9, propertyNameCol, "Movable:");
        mvprintw(9, propertyDataCol, "%s", selectedElement.isMovable() ? "Yes" : "No");
        mvprintw(10, propertyNameCol, "Dynamic:");
        mvprintw(10, propertyDataCol, "%s", selectedElement.isDynamic() ? "Yes" : "No");
        mvprintw(11, propertyNameCol, "Flammable:");
        mvprintw(11, propertyDataCol, "%s", selectedElement.isFlammable() ? "Yes" : "No");
        mvprintw(12, propertyNameCol, "Density:");
        mvprintw(12, propertyDataCol, "%d", selectedElement.getDensity());
        mvprintw(13, propertyNameCol, "Gravity:");
        mvprintw(13, propertyDataCol, "%d", selectedElement.getGravity());

        
        // Define dimensions for the box
        int boxHeight = 7; // Height of the box (including padding)
        int boxWidth = 14;  // Width of the box (including padding)
        int startY = 15;   // Starting row for the box
        int startX = propertyNameCol; // Center the box horizontally
        
        // Draw the box border
        for (int i = 0; i < boxHeight; ++i) {
            for (int j = 0; j < boxWidth; ++j) {
                if (i == 0 || i == boxHeight - 1) {
                    mvaddch(startY + i, startX + j, ACS_HLINE); // Top and bottom borders
                } else if (j == 0 || j == boxWidth - 1) {
                    mvaddch(startY + i, startX + j, ACS_VLINE); // Left and right borders
                }
            }
        }
                
        mvaddch(startY, startX, ACS_ULCORNER); // Top-left corner
        mvaddch(startY, startX + boxWidth - 1, ACS_URCORNER); // Top-right corner
        mvaddch(startY + boxHeight - 1, startX, ACS_LLCORNER); // Bottom-left corner
        mvaddch(startY + boxHeight - 1, startX + boxWidth - 1, ACS_LRCORNER); // Bottom-right corner
        
        int colorPairID = getColorPairID(selectedElement.getFGColor(), selectedElement.getBGColor());
        wattron(stdscr, COLOR_PAIR(colorPairID));

        // Display the selected element in the center of the box
        char asciiChar = selectedElement.getAscii();
        int centerY = startY + boxHeight / 2; // Center row
        int centerX = startX + boxWidth / 2; // Center column
        mvprintw(centerY, centerX, "%c", asciiChar); // Place the ASCII character in the center

        
        wattroff(stdscr, COLOR_PAIR(colorPairID));

        int key = getch();
        if (key == KEY_UP) {
            selected = (selected - 1 + numElements) % numElements; // move up
        } else if (key == KEY_DOWN) {
            selected = (selected + 1) % numElements; // move down
        } else if (key >= '1' && key <= '9') {
            int hotbarSlot = key - '1';
            hotbar[hotbarSlot] = selectedElement.getId(); // set hotbar slot to selected element ID
        } else if (key == '0') {
            hotbar[9] = selectedElement.getId(); // set hotbar slot 10 to selected element ID
        } else if (key == 'p') {
            nodelay(stdscr, TRUE);
            mainMenu();
            break;
        } else if (key == 27 || key == 'i') { // exit
            nodelay(stdscr, TRUE);
            break;
        }
    }
}