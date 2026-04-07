#include "rules.h"
#include "transformation.h"
#include "globals.h"
#include <cstdlib>

void initializeRules() {
    // clear any existing transformations
    TransformationRegistry::clearTransformations();

    // MOVEMENT INTERACTIONS
    // move movable elements using the same gravity/piling/diffusion behavior as before
    TransformationRegistry::registerTransformation(
        TransformationBuilder("movable_physics")
            .requireAll({
                [](int y, int x) {
                    return getFrameElement(y, x).isMovable();
                },
                [](int y, int x) {
                    // if another transformation already changed this cell this frame, skip movement
                    return grid[y][x].getId() == getFrameElement(y, x).getId();
                }
            })
            .customAction([](int y, int x) {
                Element current = getFrameElement(y, x);

                auto tryMove = [&](int newY, int newX) -> bool {
                    if (newY < BORDER_SIZE || newY >= termHeight - BORDER_SIZE ||
                        newX < BORDER_SIZE || newX >= termWidth - BORDER_SIZE) {
                        return false;
                    }

                    // Only move into a destination that has not already changed this frame.
                    if (grid[newY][newX].getId() != getFrameElement(newY, newX).getId()) {
                        return false;
                    }

                    if (getFrameElement(newY, newX).getDensity() < current.getDensity()) {
                        std::swap(grid[y][x], grid[newY][newX]);
                        return true;
                    }
                    return false;
                };

                if (tryMove(y + 1, x)) return;

                if (current.isPiles()) {
                    if (tryMove(y + 1, x - 1)) return;
                    if (tryMove(y + 1, x + 1)) return;
                }

                if (current.isDiffusable()) {
                    int dir = rand() % 3; // 0 = left, 1 = right, 2 = none
                    if (dir == 0) {
                        tryMove(y, x - 1);
                    } else if (dir == 1) {
                        tryMove(y, x + 1);
                    }
                }
            })
            .setFrameInterval(1)
            .setPriority(1)
            .build()
    );

    // FIRE INTERACTIONS
    // burn like the original updateGrid logic: any igniter spreads to all 8
    // flammable neighbors, and burnout igniters turn into air afterward
    TransformationRegistry::registerTransformation(
        TransformationBuilder("igniter_spreads_and_burns")
            .requireAll({
                [](int y, int x) {
                    return getFrameElement(y, x).isIgniter();
                },
                [](int y, int x) {
                    const int dirs[8][2] = {
                        {-1, -1}, {-1, 0}, {-1, 1},
                        {0, -1},             {0, 1},
                        {1, -1},  {1, 0},  {1, 1}
                    };
                    for (int i = 0; i < 8; ++i) {
                        int ny = y + dirs[i][0];
                        int nx = x + dirs[i][1];
                        if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                            nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE &&
                            getFrameElement(ny, nx).isFlammable()) {
                            return true;
                        }
                    }
                    return false;
                }
            })
            .customAction([](int y, int x) {
                // spread ignition to all flammable neighbors from the frame snapshot
                int flammableNeighbors[8][2];
                int count = 0;
                const int dirs[8][2] = {
                    {-1, -1}, {-1, 0}, {-1, 1},
                    {0, -1},             {0, 1},
                    {1, -1},  {1, 0},  {1, 1}
                };

                for (int i = 0; i < 8; ++i) {
                    int ny = y + dirs[i][0];
                    int nx = x + dirs[i][1];
                    if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                        nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE &&
                        getFrameElement(ny, nx).isFlammable()) {
                        flammableNeighbors[count][0] = ny;
                        flammableNeighbors[count][1] = nx;
                        ++count;
                    }
                }

                if (count > 0) {
                    int ignitedCount = 0;
                    for (int i = 0; i < count; ++i) {
                        int ny = flammableNeighbors[i][0];
                        int nx = flammableNeighbors[i][1];

                        // Irregular fire front: random spread with slight upward bias.
                        int chance = 45;
                        if (ny < y) {
                            chance += 25; // prefer rising flames
                        } else if (ny > y) {
                            chance -= 15; // less likely to spread downward
                        }
                        if (nx != x) {
                            chance -= 5; // diagonals ignite a bit less often
                        }

                        if (chance < 10) chance = 10;
                        if (chance > 90) chance = 90;

                        if (rand() % 100 < chance) {
                            grid[ny][nx] = Element::fromName("fire");
                            ++ignitedCount;
                        }
                    }

                    // Keep burn propagation alive when all random rolls fail.
                    if (ignitedCount == 0) {
                        int fallback = rand() % count;
                        int ny = flammableNeighbors[fallback][0];
                        int nx = flammableNeighbors[fallback][1];
                        grid[ny][nx] = Element::fromName("fire");
                    }
                }

                // burnout igniters become air after spreading
                if (getFrameElement(y, x).isBurnOut()) {
                    grid[y][x] = Element::fromName("air");
                }
            })
            .setFrameInterval(1)
            .setPriority(10)
            .build()
    );

    // igniter without fuel burns out
    TransformationRegistry::registerTransformation(
        TransformationBuilder("igniter_burns_out")
            .requireAll({
                [](int y, int x) {
                    return getFrameElement(y, x).isIgniter();
                },
                [](int y, int x) {
                    return getFrameElement(y, x).isBurnOut();
                }
            })
            .customAction([](int y, int x) {
                // check if there are any flammable neighbors
                bool hasFuel = false;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dy == 0 && dx == 0) continue;
                        int ny = y + dy;
                        int nx = x + dx;
                        if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                            nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE) {
                            if (getFrameElement(ny, nx).isFlammable()) {
                                hasFuel = true;
                                break;
                            }
                        }
                    }
                    if (hasFuel) break;
                }
                
                // if there is no fuel nearby, the flame may continue upward a little or burn out
                if (!hasFuel) {
                    int ny = y - 1;
                    int nx = x;

                    if (ny >= BORDER_SIZE && ny < termHeight - BORDER_SIZE &&
                        nx >= BORDER_SIZE && nx < termWidth - BORDER_SIZE &&
                        getFrameElement(ny, nx).getName() == "air" &&
                        grid[ny][nx].getId() == getFrameElement(ny, nx).getId() &&
                        rand() % 2 == 0) {
                        grid[ny][nx] = Element::fromName("fire");
                    }

                    grid[y][x] = Element::fromName("air");
                }
            })
            .setFrameInterval(1)
            .setPriority(5)
            .build()
    );

    TransformationRegistry::sortByPriority();
}
