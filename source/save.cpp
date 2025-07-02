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
    
    std::ifstream file(saveFolder + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for loading: " << filename << std::endl;
        return;
    }

    // read terminal dimensions
    file >> termHeight >> termWidth;

    // free the old grid memory
    freeGrid();

    // create a new grid with the right dimensions
    grid = new Element*[termHeight];
    for (int i = 0; i < termHeight; ++i) {
        grid[i] = new Element[termWidth];
    }

    // read element data onto grid
    for (int y = 0; y < termHeight; ++y) {
        for (int x = 0; x < termWidth; ++x) {
            int id;
            file >> id;
            grid[y][x] = Element::fromId(id); // create element from id
        }
    }

    file.close();
    std::cout << "Game loaded from " << filename << std::endl;
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