#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <ncurses.h> // nclude ncurses for color constants

class Element {
private:
    // element ids
    int id;           // unique int identifier for the element
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
    Element(const int id, const std::string& name, char ascii, int fgColor, int bgColor, 
            bool movable, bool dynamic, bool flammable, int density, int gravity);

    // static factory methods to generate specific elements
    static Element air() {
        return Element(0, "air", ' ', COLOR_BLACK, COLOR_BLACK, true, true, false, 1, 1);
    }

    static Element water() {
        return Element(1, "water", '~', COLOR_BLUE, COLOR_BLACK, true, false, true, 3, 1);
    }

    static Element dirt() {
        return Element(2, "dirt", '#', COLOR_YELLOW, COLOR_BLACK, true, false, false, 10, 1);
    }

    static Element sand() {
        return Element(3, "sand", '%', COLOR_YELLOW, COLOR_BLACK, true, true, false, 8, 1);
    }

    static Element fire() {
        return Element(4, "fire", '*', COLOR_RED, COLOR_BLACK, true, false, true, 0, 1);
    }

    static Element rock() {
        return Element(5, "rock", '#', COLOR_WHITE, COLOR_BLACK, true, false, false, 11, 1);
    }

    static Element grass() {
        return Element(6, "grass", '_', COLOR_GREEN, COLOR_BLACK, true, false, false, 7, 1);
    }

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
};

#endif