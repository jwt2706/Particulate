#include "../include/element.h"

// default acts like air: movable, dynamic, and not flammable
Element::Element() 
    : movable(true), dynamic(true), flammable(false), density(1), gravity(0)  {}

Element::Element(bool movable, bool dynamic, bool flammable, int density, int gravity)
    : movable(movable), dynamic(dynamic), flammable(flammable), density(density), gravity(gravity) {}

bool Element::isMovable() const {
    return movable;
}

bool Element::isDynamic() const {
    return dynamic;
}

bool Element::isFlammable() const {
    return flammable;
}
