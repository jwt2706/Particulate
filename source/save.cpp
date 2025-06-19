#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include "../include/element.h"

extern int termHeight, termWidth;
extern Element** grid;
extern void freeGrid();

void saveGame(const std::string& filename) {
    // creates saves folder if it doesn't exist
    std::filesystem::create_directory("saves");

    // Open the file in the /saves/ folder
    std::ofstream file("saves/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for saving: " << filename << std::endl;
        return;
    }

    file << termHeight << " " << termWidth << "\n"; // Save dimensions
    for (int y = 0; y < termHeight; ++y) {
        for (int x = 0; x < termWidth; ++x) {
            const Element& elem = grid[y][x];
            file << elem.getName() << " " << elem.getAscii() << " "
                 << elem.getFGColor() << " " << elem.getBGColor() << " "
                 << elem.isMovable() << " " << elem.isDynamic() << " "
                 << elem.isFlammable() << " " << elem.getDensity() << " "
                 << elem.getGravity() << "\n";
        }
    }
    file.close();
    std::cout << "Game saved to saves/" << filename << std::endl;
}

// load the grid state from a file
void loadGame(const std::string& filename) {
    std::ifstream file("saves/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for loading: " << filename << std::endl;
        return;
    }

    int savedHeight, savedWidth;
    file >> savedHeight >> savedWidth;

    // Resize the game grid if necessary
    if (savedHeight != termHeight || savedWidth != termWidth) {
        freeGrid();
        termHeight = savedHeight;
        termWidth = savedWidth;
        grid = new Element*[termHeight];
        for (int i = 0; i < termHeight; ++i) {
            grid[i] = new Element[termWidth];
        }
    }

    // Load elements into the grid
    for (int y = 0; y < termHeight; ++y) {
        for (int x = 0; x < termWidth; ++x) {
            std::string name;
            char ascii;
            int fgColor, bgColor, movable, dynamic, flammable, density, gravity;

            file >> name >> ascii >> fgColor >> bgColor
                 >> movable >> dynamic >> flammable
                 >> density >> gravity;

            grid[y][x] = Element(name, ascii, fgColor, bgColor,
                                 static_cast<bool>(movable),
                                 static_cast<bool>(dynamic),
                                 static_cast<bool>(flammable),
                                 density, gravity);
        }
    }
    file.close();
    std::cout << "Game loaded from saves/" << filename << std::endl;
}