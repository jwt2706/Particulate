#include "transformation.h"
#include "globals.h"
#include <algorithm>

// « Rien ne se perd, rien ne se crée, tout se transforme »
// - Antoine Lavoisier

std::vector<Transformation> TransformationRegistry::transformations;
static unsigned long long transformationFrameTick = 0;
static std::vector<Element> transformationFrameSnapshot;
static int transformationSnapshotHeight = 0;
static int transformationSnapshotWidth = 0;

// Transformation constructor
Transformation::Transformation(const std::string& name, ConditionCheck cond, TransformationAction act, int priority)
    : name(name), condition(cond), action(act), priority(priority), frame_interval(0) {}

// TransformationBuilder implementation
TransformationBuilder::TransformationBuilder(const std::string& name) 
    : name(name), conditionCheck(nullptr) {}

TransformationBuilder& TransformationBuilder::requireElement(const std::string& elementName) {
    auto prevCondition = conditionCheck;
    conditionCheck = [elementName, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        return grid[y][x].getName() == elementName;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireElementId(int elementId) {
    auto prevCondition = conditionCheck;
    conditionCheck = [elementId, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        return grid[y][x].getId() == elementId;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireProperty(std::function<bool(const Element&)> prop) {
    auto prevCondition = conditionCheck;
    conditionCheck = [prop, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        return prop(grid[y][x]);
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireNeighbor(const std::string& elementName, int range) {
    auto prevCondition = conditionCheck;
    conditionCheck = [elementName, range, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        
        for (int dy = -range; dy <= range; ++dy) {
            for (int dx = -range; dx <= range; ++dx) {
                if (dy == 0 && dx == 0) continue;
                int ny = y + dy;
                int nx = x + dx;
                if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                    nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE) {
                    if (grid[ny][nx].getName() == elementName) {
                        return true;
                    }
                }
            }
        }
        return false;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireNeighborProperty(std::function<bool(const Element&)> prop, int range) {
    auto prevCondition = conditionCheck;
    conditionCheck = [prop, range, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        
        for (int dy = -range; dy <= range; ++dy) {
            for (int dx = -range; dx <= range; ++dx) {
                if (dy == 0 && dx == 0) continue;
                int ny = y + dy;
                int nx = x + dx;
                if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                    nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE) {
                    if (prop(grid[ny][nx])) {
                        return true;
                    }
                }
            }
        }
        return false;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireNoNeighbor(const std::string& elementName, int range) {
    auto prevCondition = conditionCheck;
    conditionCheck = [elementName, range, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        
        for (int dy = -range; dy <= range; ++dy) {
            for (int dx = -range; dx <= range; ++dx) {
                if (dy == 0 && dx == 0) continue;
                int ny = y + dy;
                int nx = x + dx;
                if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                    nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE) {
                    if (grid[ny][nx].getName() == elementName) {
                        return false;
                    }
                }
            }
        }
        return true;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireAll(std::vector<ConditionCheck> conditions) {
    auto prevCondition = conditionCheck;
    conditionCheck = [conditions, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        for (const auto& cond : conditions) {
            if (!cond(y, x)) return false;
        }
        return true;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::requireAny(std::vector<ConditionCheck> conditions) {
    auto prevCondition = conditionCheck;
    conditionCheck = [conditions, prevCondition](int y, int x) {
        if (prevCondition && !prevCondition(y, x)) return false;
        for (const auto& cond : conditions) {
            if (cond(y, x)) return true;
        }
        return false;
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::transformToElement(const std::string& elementName) {
    transformAction = [elementName](int y, int x) {
        grid[y][x] = Element::fromName(elementName);
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::transformToElement(int elementId) {
    transformAction = [elementId](int y, int x) {
        grid[y][x] = Element::fromId(elementId);
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::transformNeighbors(const std::string& fromElement, const std::string& toElement, int range) {
    transformAction = [fromElement, toElement, range](int y, int x) {
        for (int dy = -range; dy <= range; ++dy) {
            for (int dx = -range; dx <= range; ++dx) {
                int ny = y + dy;
                int nx = x + dx;
                if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                    nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE) {
                    if (grid[ny][nx].getName() == fromElement) {
                        grid[ny][nx] = Element::fromName(toElement);
                    }
                }
            }
        }
    };
    return *this;
}

TransformationBuilder& TransformationBuilder::customAction(TransformationAction act) {
    transformAction = act;
    return *this;
}

TransformationBuilder& TransformationBuilder::setPriority(int p) {
    priority = p;
    return *this;
}

TransformationBuilder& TransformationBuilder::setFrameInterval(int interval) {
    frame_interval = interval;
    return *this;
}

Transformation TransformationBuilder::build() {
    // ensure we have a valid condition
    if (!conditionCheck) {
        conditionCheck = [](int y, int x) { return true; };
    }
    if (!transformAction) {
        transformAction = [](int y, int x) {};
    }
    Transformation t(name, conditionCheck, transformAction, priority);
    t.frame_interval = frame_interval;
    return t;
}

// TransformationRegistry implementation
void TransformationRegistry::registerTransformation(const Transformation& t) {
    transformations.push_back(t);
}

void TransformationRegistry::clearTransformations() {
    transformations.clear();
}

std::vector<Transformation>& TransformationRegistry::getTransformations() {
    return transformations;
}

void TransformationRegistry::sortByPriority() {
    std::sort(transformations.begin(), transformations.end(), 
              [](const Transformation& a, const Transformation& b) {
                  return a.priority > b.priority;
              });
}

void beginTransformationFrame() {
    ++transformationFrameTick;

    transformationSnapshotHeight = termHeight;
    transformationSnapshotWidth = termWidth;
    transformationFrameSnapshot.resize(static_cast<size_t>(termHeight * termWidth));

    for (int y = 0; y < termHeight; ++y) {
        for (int x = 0; x < termWidth; ++x) {
            transformationFrameSnapshot[static_cast<size_t>(y * termWidth + x)] = grid[y][x];
        }
    }
}

const Element& getFrameElement(int y, int x) {
    if (transformationSnapshotHeight > 0 && transformationSnapshotWidth > 0 &&
        y >= 0 && y < transformationSnapshotHeight && x >= 0 && x < transformationSnapshotWidth) {
        return transformationFrameSnapshot[static_cast<size_t>(y * transformationSnapshotWidth + x)];
    }

    if (y >= 0 && y < termHeight && x >= 0 && x < termWidth) {
        return grid[y][x];
    }

    static Element fallbackAir = Element::fromName("air");
    return fallbackAir;
}

// apply the first matching transformation at a position
bool applyTransformation(int y, int x) {
    auto& transformations = TransformationRegistry::getTransformations();
    
    for (auto& transformation : transformations) {
        // check if this transformation is scheduled for the current frame.
        if (transformation.frame_interval > 0 &&
            transformationFrameTick % static_cast<unsigned long long>(transformation.frame_interval) != 0) {
            continue;  // skip this transformation, not time yet
        }
        
        // check condition and apply transformation
        if (transformation.condition(y, x)) {
            transformation.action(y, x);
            return true; // Only apply one transformation per cell per frame
        }
    }
    return false;
}
