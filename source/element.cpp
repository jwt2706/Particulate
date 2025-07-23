#include "../include/element.h"

// element definitions
std::vector<Element> Element::elements = {
    //      id,     name,       ascii,  color,      state,      movable,    dynamic,    flammable,  igniter,    density,    gravity 
    Element(0,      "air",      ' ',    15,         "gas",      true,       true,       false,      false,      1,          1),
    Element(1,      "water",    '~',    39,         "liquid",   true,       false,      false,      false,      3,          2),
    Element(2,      "dirt",     '#',    94,         "solid",    true,       false,      false,      false,      10,         1),
    Element(3,      "sand",     '%',    228,        "solid",    true,       true,       false,      false,      8,          2),
    Element(4,      "fire",     '*',    196,        "gas",      true,       false,      false,      true,       0,          -2),
    Element(5,      "rock",     '#',    8,          "solid",    true,       false,      false,      false,      11,         1),
    Element(6,      "grass",    '_',    34,         "solid",    true,       false,      true,       false,      7,          2),
    Element(7,      "lava",     '@',    208,        "liquid",   true,       true,       false,      true,       12,         3),
    Element(8,      "ice",      '=',    51,         "solid",    true,       false,      false,      false,      5,          0),
    Element(9,      "wood",     '|',    130,        "solid",    false,      false,      true,       false,      6,          1),
    Element(10,     "leaf",     '#',    46,         "solid",    false,      false,      true,       false,      2,          0),
    Element(11,     "steam",    '^',    250,        "gas",      true,       true,       false,      false,      1,          -1),
    Element(12,     "oil",      '~',    53,         "liquid",   true,       true,       true,       false,      4,          2),
    Element(13,     "metal",    '$',    7,          "solid",    false,      false,      false,      false,      15,         1),
    Element(14,     "mud",      '&',    94,         "solid",    true,       false,      false,      false,      9,          1),
    Element(15,     "acid",     '!',    118,        "liquid",   true,       true,       false,      true,       3,          2),
    Element(16,     "cloud",    '~',    252,        "gas",      true,       true,       false,      false,      -1,         -1),
    Element(17,     "glass",    '=',    159,        "solid",    false,      false,      false,      false,      10,         2),
    Element(18,     "coal",     '#',    16,         "solid",    false,      false,      true,       false,      8,          2),
};

// constructors
Element::Element() 
    :   id(0),
        name("air"),
        ascii(' '),
        color(15),
        state("gas"),
        movable(true),
        dynamic(true),
        flammable(false),
        igniter(false),
        density(1),
        gravity(1) {}
        
Element::Element(const int id, const std::string& name, char ascii, int color, const std::string& state, 
                 bool movable, bool dynamic, bool flammable, bool igniter, int density, int gravity)
    :   id(id),
        name(name),
        ascii(ascii),
        color(color),
        state(state),
        movable(movable),
        dynamic(dynamic),
        flammable(flammable),
        igniter(igniter),
        density(density),
        gravity(gravity) {}

// accessor methods
std::string Element::getName() const { return name; }
int Element::getId() const { return id; }
char Element::getAscii() const { return ascii; }
int Element::getColor() const { return color; }
std::string Element::getState() const { return state; }
bool Element::isMovable() const { return movable; }
bool Element::isDynamic() const { return dynamic; }
bool Element::isFlammable() const { return flammable; }
bool Element::isIgniter() const { return igniter; }
int Element::getDensity() const { return density; }
int Element::getGravity() const { return gravity; }

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