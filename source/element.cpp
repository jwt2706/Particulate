#include "../include/element.h"

// default acts like air: movable, dynamic, and not flammable
Element::Element() 
    : movable(true), dynamic(true), flammable(false), density(1), gravity(0)  {}

Element::Element(bool movable, bool dynamic, bool flammable, int density, int gravity)
    : movable(movable), dynamic(dynamic), flammable(flammable), density(density), gravity(gravity) {}

string Element::getName() const {
    return name;
}

string Element::getAscii() const {
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

int Element:: getDensity() const {
    return density;
}

int Element::getGravity() const {
    return gravity;
}
