#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <vector>

void saveGame(const std::string& filename);
void loadGame(const std::string& filename);
std::vector<std::string> getSaveFiles();

#endif // SAVE_H