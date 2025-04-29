#include "../include/element.h"

// default element is just air
Element::Element() 
    : Element("air", ' ', COLOR_WHITE, COLOR_WHITE, true, true, false, 1, 0) {}

Element::Element(const std::string& name, char ascii, int fgColor, int bgColor, 
                 bool movable, bool dynamic, bool flammable, int density, int gravity)
    : name(name), ascii(ascii), fgColor(fgColor), bgColor(bgColor), 
      movable(movable), dynamic(dynamic), flammable(flammable), 
      density(density), gravity(gravity) {}

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

int Element:: getDensity() const {
    return density;
}

int Element::getGravity() const {
    return gravity;
}
