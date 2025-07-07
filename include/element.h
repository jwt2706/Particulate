#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <vector>
#include <ncurses.h>

class Element {
private:
    // element properties
    int id;           // unique identifier for the element
    std::string name; // name of the element
    char ascii;       // ascii character to represent the element
    int fgColor;      // foreground color
    int bgColor;      // background color
    bool movable;     // whether the element can move
    bool dynamic;     // whether the element changes over time
    bool flammable;   // whether the element can catch fire
    int density;      // density of the element (affects floating/sinking)
    int gravity;      // gravity effect (affects movement direction)

    // static list of all elements
    static std::vector<Element> elements;

public:
    Element();
    Element(const int id, const std::string& name, char ascii, int fgColor, int bgColor, 
            bool movable, bool dynamic, bool flammable, int density, int gravity);

    // accessor methods
    std::string getName() const;
    int getId() const;
    char getAscii() const;
    int getFGColor() const;
    int getBGColor() const;
    bool isMovable() const;
    bool isDynamic() const;
    bool isFlammable() const;
    int getDensity() const;
    int getGravity() const;

    // static methods to access elements
    static const std::vector<Element>& getAllElements();
    static Element fromId(const int id);
    static Element fromName(const std::string& name);
};

#endif