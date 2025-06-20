#include "../include/element.h"

// default element is just air
Element::Element() 
    : Element(0, "air", ' ', COLOR_WHITE, COLOR_WHITE, true, true, false, 1, 1) {}

Element::Element(const int id, const std::string& name, char ascii, int fgColor, int bgColor, 
                 bool movable, bool dynamic, bool flammable, int density, int gravity)
    : id(id), name(name), ascii(ascii), fgColor(fgColor), bgColor(bgColor), 
      movable(movable), dynamic(dynamic), flammable(flammable), 
      density(density), gravity(gravity) {}

int Element::getId() const {
    return id;
}

std::string Element::getName() const {
    return name;
}

char Element::getAscii() const {
    return ascii;
}

int Element::getFGColor() const {
    return fgColor;
}

int Element::getBGColor() const {
    return bgColor;
}

bool Element::isMovable() const {
    return movable;
}

bool Element::isDynamic() const {
    return dynamic;
}

bool Element::isFlammable() const {
    return flammable;
}

int Element::getDensity() const {
    return density;
}

int Element::getGravity() const {
    return gravity;
}

Element Element::fromId(int id) {
    switch (id) {
        case 0: return Element::air();
        case 1: return Element::water();
        case 2: return Element::dirt();
        case 3: return Element::sand();
        case 4: return Element::fire();
        case 5: return Element::rock();
        case 6: return Element::grass();
        default: return Element(); // default to air if unknown id
    }
}