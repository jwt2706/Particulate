#include "include/Element.h"

// default acts like air: movable, dynamic, and not flammable
Element::Element() 
    : movable(true), dynamic(true), flammable(false) {}

Element::Element(bool movable, bool dynamic, bool flammable)
    : movable(movable), dynamic(dynamic), flammable(flammable) {}

bool Element::isMovable() const {
    return movable;
}

bool Element::isDynamic() const {
    return dynamic;
}

bool Element::isFlammable() const {
    return flammable;
}
