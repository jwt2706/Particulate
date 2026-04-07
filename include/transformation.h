#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <functional>
#include <vector>
#include <string>
#include "element.h"

// Forward declaration
struct Transformation;

// Callback type for transformation conditions
using ConditionCheck = std::function<bool(int y, int x)>;
using TransformationAction = std::function<void(int y, int x)>;

class Transformation {
public:
    std::string name;
    ConditionCheck condition;
    TransformationAction action;
    int priority; // higher priority transformations are checked first

    Transformation(const std::string& name, ConditionCheck cond, TransformationAction act, int priority = 0);
};

// Builder class for easy transformation definition
class TransformationBuilder {
private:
    std::string name;
    ConditionCheck conditionCheck;
    TransformationAction transformAction;
    int priority = 0;

public:
    TransformationBuilder(const std::string& name);
    
    // Condition builders - check the cell being evaluated
    TransformationBuilder& requireElement(const std::string& elementName);
    TransformationBuilder& requireElementId(int elementId);
    TransformationBuilder& requireProperty(std::function<bool(const Element&)> prop);
    TransformationBuilder& requireNeighbor(const std::string& elementName, int range = 1);
    TransformationBuilder& requireNeighborProperty(std::function<bool(const Element&)> prop, int range = 1);
    TransformationBuilder& requireNoNeighbor(const std::string& elementName, int range = 1);
    TransformationBuilder& requireAll(std::vector<ConditionCheck> conditions);
    TransformationBuilder& requireAny(std::vector<ConditionCheck> conditions);
    
    // Action builders
    TransformationBuilder& transformToElement(const std::string& elementName);
    TransformationBuilder& transformToElement(int elementId);
    TransformationBuilder& transformNeighbors(const std::string& fromElement, const std::string& toElement, int range = 1);
    TransformationBuilder& customAction(TransformationAction act);
    
    // Priority setter
    TransformationBuilder& setPriority(int p);
    
    // Build the transformation
    Transformation build();
};

// Global transformation registry
class TransformationRegistry {
private:
    static std::vector<Transformation> transformations;

public:
    static void registerTransformation(const Transformation& t);
    static void clearTransformations();
    static std::vector<Transformation>& getTransformations();
    static void sortByPriority();
};

void beginTransformationFrame();
const Element& getFrameElement(int y, int x);
bool applyTransformation(int y, int x);

#endif // TRANSFORMATION_H
