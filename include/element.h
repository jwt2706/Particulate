#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <vector>
#include <ncurses.h>

class Element {
private:
    // element properties
    int id;             // unique identifier for the element
    std::string name;   // name of the element
    char ascii;         // ascii character to represent the element
    int color;          // color
    int density;        // density of the element (affects floating/sinking)
    bool movable;       // whether the element can move
    bool flammable;     // whether the element can catch fire
    bool igniter;       // whether the element can start a fire on contact
    bool burnOut;       // whether the element should burn out after igniting
    bool piles;         // whether the element can form piles
    bool diffusable;    // whether the element can diffuse into surrounding elements

    // static list of all elements
    static std::vector<Element> elements;

public:
    Element();
    Element(const int id, const std::string& name, char ascii, int color, int density,
            bool movable, bool flammable, bool igniter, bool burnOut, bool piles, bool diffusable);

    // accessor methods
    std::string getName() const;
    int getId() const;
    char getAscii() const;
    int getColor() const;
    int getDensity() const;
    bool isMovable() const;
    bool isFlammable() const;
    bool isIgniter() const;
    bool isBurnOut() const;
    bool isPiles() const;
    bool isDiffusable() const;

    // static methods to access elements
    static const std::vector<Element>& getAllElements();
    static Element fromId(const int id);
    static Element fromName(const std::string& name);
};

#endif