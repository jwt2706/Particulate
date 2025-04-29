#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <ncurses.h> // nclude ncurses for color constants

class Element {
private:
    // element ids
    std::string name; // name of the element
    char ascii;       // ascii character to represent the element

    // Colors
    int fgColor;      // foreground color
    int bgColor;      // background color

    // element properties
    bool movable;     // whether the element can move
    bool dynamic;     // whether the element changes over time
    bool flammable;   // whether the element can catch fire
    int density;      // density of the element (affects floating/sinking)
    int gravity;      // gravity effect (affects movement direction)

public:
    Element();
    Element(const std::string& name, char ascii, int fgColor, int bgColor, 
            bool movable, bool dynamic, bool flammable, int density, int gravity);

    // static factory methods to generate specific elements
    static Element air() {
        return Element("air", ' ', COLOR_WHITE, COLOR_WHITE, true, true, false, 1, 0);
    }

    static Element water() {
        return Element("water", '~', COLOR_BLUE, COLOR_BLACK, true, false, true, 1, 1);
    }

    static Element dirt() {
        return Element("dirt", '#', COLOR_YELLOW, COLOR_BLACK, false, false, false, 3, 0);
    }

    static Element sand() {
        return Element("sand", '%', COLOR_YELLOW, COLOR_BLACK, true, true, false, 2, 1);
    }

    static Element fire() {
        return Element("fire", '*', COLOR_RED, COLOR_BLACK, false, false, true, 1, 0);
    }

    static Element stone() {
        return Element("stone", '#', COLOR_WHITE, COLOR_BLACK, false, false, false, 5, 0);
    }

    // accessor methods
    std::string getName() const;
    char getAscii() const;
    int getFGColor() const;
    int getBGColor() const;
    bool isMovable() const;
    bool isDynamic() const;
    bool isFlammable() const;
    int getDensity() const;
    int getGravity() const;
};

#endif