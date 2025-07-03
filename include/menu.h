#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

int menu(const char* msg, const std::vector<std::string>& options);
bool confirm(const char* question, const char* message = nullptr);
bool mainMenu();
void newMenu();
void saveMenu();
void loadMenu();

#endif // MENU_H