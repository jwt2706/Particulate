#include "../include/element.h"

// element definitions
std::vector<Element> Element::elements = {
    Element(0, "air", ' ', COLOR_BLACK, COLOR_BLACK, true, true, false, 1, 1),
    Element(1, "water", '~', COLOR_BLUE, COLOR_BLACK, true, false, false, 3, 2),
    Element(2, "dirt", '#', COLOR_YELLOW, COLOR_BLACK, true, false, false, 10, 1),
    Element(3, "sand", '%', COLOR_YELLOW, COLOR_BLACK, true, true, false, 8, 2),
    Element(4, "fire", '*', COLOR_RED, COLOR_BLACK, true, false, false, 0, -2),
    Element(5, "rock", '#', COLOR_WHITE, COLOR_BLACK, true, false, false, 11, 1),
    Element(6, "grass", '_', COLOR_GREEN, COLOR_BLACK, true, false, true, 7, 2),
    Element(7, "lava", '@', COLOR_RED, COLOR_BLACK, true, true, false, 12, 3),
    Element(8, "ice", '=', COLOR_CYAN, COLOR_BLACK, true, false, false, 5, 0),
    Element(9, "wood", '|', COLOR_YELLOW, COLOR_BLACK, false, false, true, 6, 0),
    Element(10, "leaf", '#', COLOR_GREEN, COLOR_BLACK, false, false, true, 2, 0),
    Element(11, "steam", '^', COLOR_WHITE, COLOR_BLACK, true, true, false, 1, -1),
    Element(12, "oil", '~', COLOR_MAGENTA, COLOR_BLACK, true, true, true, 4, 2),
    Element(13, "metal", '$', COLOR_WHITE, COLOR_BLACK, false, false, false, 15, 0)
};

// constructors
Element::Element() 
    : id(0), name("air"), ascii(' '), fgColor(COLOR_BLACK), bgColor(COLOR_BLACK), 
      movable(true), dynamic(true), flammable(false), density(1), gravity(1) {}
Element::Element(const int id, const std::string& name, char ascii, int fgColor, int bgColor, 
                 bool movable, bool dynamic, bool flammable, int density, int gravity)
    : id(id), name(name), ascii(ascii), fgColor(fgColor), bgColor(bgColor), 
      movable(movable), dynamic(dynamic), flammable(flammable), 
      density(density), gravity(gravity) {}

// accessor methods
std::string Element::getName() const { return name; }
int Element::getId() const { return id; }
char Element::getAscii() const { return ascii; }
int Element::getFGColor() const { return fgColor; }
int Element::getBGColor() const { return bgColor; }
bool Element::isMovable() const { return movable; }
bool Element::isDynamic() const { return dynamic; }
bool Element::isFlammable() const { return flammable; }
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