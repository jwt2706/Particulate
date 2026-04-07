#include "element.h"

// element definitions
std::vector<Element> Element::elements = {
    //      id,     name,       ascii,  color,      density,    movable,    flammable,  igniter,    burnOut,    piles,  diffusable
    Element(0,      "air",      ' ',    15,         1,          true,       false,      false,      false,      false,  false),
    Element(1,      "water",    '~',    39,         3,          true,       false,      false,      false,      false,  true),
    Element(2,      "dirt",     '#',    94,         10,         true,       false,      false,      false,      false,  false),
    Element(3,      "sand",     '%',    228,        8,          true,       false,      false,      false,      true,   false),
    Element(4,      "fire",     '*',    196,        0,          true,       false,      true,       true,       false,  false),
    Element(5,      "rock",     '#',    8,          11,         true,       false,      false,      false,      false,  false),
    Element(6,      "grass",    '_',    34,         7,          true,       true,       false,      false,      true,   false),
    Element(7,      "lava",     '@',    208,        12,         true,       false,      true,       false,      false,  true),
    Element(8,      "ice",      '=',    51,         5,          true,       false,      false,      false,      true,   false),
    Element(9,      "wood",     '|',    130,        6,          false,      true,       false,      false,      false,  false),
    Element(10,     "leaf",     '#',    46,         2,          false,      true,       false,      false,      false,  false),
    Element(11,     "steam",    '^',    250,        0,          true,       false,      false,      false,      false,  true),
    Element(12,     "oil",      '~',    53,         4,          true,       true,       false,      false,      false,  true),
    Element(13,     "metal",    '$',    7,          15,         false,      false,      false,      false,      false,  false),
    Element(14,     "mud",      '&',    94,         9,          true,       false,      false,      false,      true,   false),
    Element(15,     "acid",     '!',    118,        3,          true,       false,      true,       false,      false,  true),
    Element(16,     "cloud",    '~',    252,        -1,         true,       false,      false,      false,      false,  true),
    Element(17,     "glass",    '=',    159,        10,         false,      false,      false,      false,      false,  false),
    Element(18,     "coal",     '#',    240,        8,          false,      true,       false,      false,      true,   false),
    Element(19,     "gravel",   '%',    246,        9,          true,       false,      false,      false,      true,   false),
    Element(20,     "obsidian", 'O',    237,        16,         false,      false,      false,      false,      false,  false),
    Element(21,     "smoke",    '\'',   244,        0,          true,       false,      false,      false,      false,  true),
    Element(22,     "clay",     ';',    173,        10,         true,       false,      false,      false,      true,   false),
};

// constructors
Element::Element() 
    :   id(0),
        name("air"),
        ascii(' '),
        color(15),
        density(1), 
        movable(true),
        flammable(false),
        igniter(false),
        burnOut(false),
        piles(false),
        diffusable(false){}
        
Element::Element(const int id, const std::string& name, char ascii, int color, int density,
            bool movable, bool flammable, bool igniter, bool burnOut, bool piles, bool diffusable)
    :   id(id),
        name(name),
        ascii(ascii),
        color(color),
        density(density),
        movable(movable),
        flammable(flammable),
        igniter(igniter),
        burnOut(burnOut),
        piles(piles),
        diffusable(diffusable) {}

// accessor methods
std::string Element::getName() const { return name; }
int Element::getId() const { return id; }
char Element::getAscii() const { return ascii; }
int Element::getColor() const { return color; }
int Element::getDensity() const { return density; }
bool Element::isMovable() const { return movable; }
bool Element::isFlammable() const { return flammable; }
bool Element::isIgniter() const { return igniter; }
bool Element::isBurnOut() const { return burnOut; }
bool Element::isPiles() const { return piles; }
bool Element::isDiffusable() const { return diffusable; }

// static method to get all elements
const std::vector<Element>& Element::getAllElements() {
    return elements;
}

// static method to get an element by ID
Element Element::fromId(int id) {
    for (const Element& element : elements) {
        if (element.getId() == id) {
            return element;
        }
    }
    return elements[0]; // default to air if ID is not found
}

Element Element::fromName(const std::string& name) {
    for (const Element& element : elements) {
        if (element.getName() == name) {
            return element;
        }
    }
    return elements[0]; // default to air if name is not found
}
