#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
private:
    bool movable;
    bool dynamic;
    bool flammable;
    int density;
    int gravity;

public:
    Element();
    Element(bool movable, bool dynamic, bool flammable, int density, int gravity);

    bool isMovable() const;
    bool isDynamic() const;
    bool isFlammable() const;
    int getDensity() const; // check when you want to see what should 'float' to the surface
    int getGravity() const; // difference in movement direction (water vs gas)
};

#endif // ELEMENT_H
