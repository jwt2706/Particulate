#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
public:
    Element();
    Element(bool movable, bool dynamic, bool flammable, int density, int gravity);

    bool isMovable() const;
    bool isDynamic() const;
    bool isFlammable() const;
    int density() const; // check when you want to see what should 'float' to the surface
    int gravity() const; // difference in movement direction (water vs gas)


private:
    bool movable;
    bool dynamic;
    bool flammable;
};

#endif // ELEMENT_H
