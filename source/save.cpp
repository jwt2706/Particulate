#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include "../include/globals.h"
#include "../include/element.h"

extern int termHeight, termWidth;
extern Element** grid;
extern void freeGrid();

void saveGame(const std::string& filename) {
    std::filesystem::create_directory(saveFolder); // create save folder if it doesn't exist

    // open the selected save file
    std::ofstream file(saveFolder + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for saving: " << filename << std::endl;
        return;
    }

    file << termHeight << " " << termWidth << "\n"; // save terminal dimensions

    for (int y = 0; y < termHeight; ++y) {
        for (int x = 0; x < termWidth; ++x) {
            file << grid[y][x].getId() << " ";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Game saved to " << filename << std::endl;
}

// load the grid state from a file
void loadGame(const std::string& filename) {
    clear();

    //
}