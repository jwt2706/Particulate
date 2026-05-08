#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "globals.h"
#include "element.h"

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
bool loadGame(const std::string& filename) {
    clear();
    
    std::ifstream file(saveFolder + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for loading: " << filename << std::endl;
        return false;
    }

    // read terminal dimensions
    int savedHeight = 0;
    int savedWidth = 0;
    if (!(file >> savedHeight >> savedWidth)) {
        std::cerr << "Error reading save metadata: " << filename << std::endl;
        return false;
    }

    if (savedHeight != termHeight || savedWidth != termWidth) {
        std::cerr << "Save file dimensions do not match the current terminal size: " << filename << std::endl;
        return false;
    }

    Element** loadedGrid = new Element*[savedHeight];
    for (int i = 0; i < savedHeight; ++i) {
        loadedGrid[i] = new Element[savedWidth];
    }

    // read element data onto grid
    for (int y = 0; y < savedHeight; ++y) {
        for (int x = 0; x < savedWidth; ++x) {
            int id;
            if (!(file >> id)) {
                for (int i = 0; i < savedHeight; ++i) {
                    delete[] loadedGrid[i];
                }
                delete[] loadedGrid;
                std::cerr << "Save file ended early while reading grid data: " << filename << std::endl;
                return false;
            }
            loadedGrid[y][x] = Element::fromId(id); // create element from id
        }
    }

    // free the old grid memory only after the new state has been validated
    freeGrid();
    grid = loadedGrid;

    file.close();
    std::cout << "Game loaded from " << filename << std::endl;
    return true;
}

std::vector<std::string> getSaveFiles() {
    std::vector<std::string> saveFiles;
    const std::string saveFolder = "saves";

    // check if the folder exists
    if (!std::filesystem::exists(saveFolder)) {
        std::cerr << "Save folder does not exist." << std::endl;
        return saveFiles;
    }

    // iterate through the files in the saves folder and return the names
    for (const auto& entry : std::filesystem::directory_iterator(saveFolder)) {
        if (entry.is_regular_file()) {
            saveFiles.push_back(entry.path().filename().string());
        }
    }

    return saveFiles;
}
