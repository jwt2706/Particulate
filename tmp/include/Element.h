#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
public:
    Element();
    Element(bool movable, bool dynamic, bool flammable);

    bool isMovable() const;
    bool isDynamic() const;
    bool isFlammable() const;

private:
    bool movable;
    bool dynamic;
    bool flammable;
};

#endif // ELEMENT_H
