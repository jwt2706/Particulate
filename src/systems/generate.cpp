#include <algorithm>
#include <vector>
#include <random>
#include <utility>
#include "grid.h"
#include "globals.h"
#include "element.h"

void generateMap() {
    clearGrid();

    const int xStart = BORDER_SIZE;
    const int xEnd = termWidth - BORDER_SIZE;
    const int yStart = BORDER_SIZE;
    const int yEnd = termHeight - BORDER_SIZE;
    const int worldW = xEnd - xStart;
    const int worldH = yEnd - yStart;

    if (worldW <= 4 || worldH <= 4) {
        renderGrid();
        return;
    }

    std::mt19937 rng(std::random_device{}());
    auto randInt = [&](int lo, int hi) {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(rng);
    };
    auto chance = [&](int percent) {
        std::uniform_int_distribution<int> dist(1, 100);
        return dist(rng) <= percent;
    };

    // 1) Build a rolling surface with a constrained random walk.
    std::vector<int> surface(worldW);
    int currentSurface = yStart + std::max(3, worldH / 4);
    const int minSurface = yStart + 3;
    const int maxSurface = yStart + std::max(6, worldH / 2);
    for (int i = 0; i < worldW; ++i) {
        currentSurface += randInt(-1, 1);
        currentSurface = std::clamp(currentSurface, minSurface, maxSurface);
        surface[i] = currentSurface;
    }

    // 2) Fill terrain layers (grass/dirt/gravel/rock/coal).
    for (int x = xStart; x < xEnd; ++x) {
        int sx = surface[x - xStart];
        for (int y = yStart; y < yEnd; ++y) {
            if (y < sx) {
                grid[y][x] = Element::fromName("air");
                continue;
            }

            int depth = y - sx;
            if (depth == 0) {
                grid[y][x] = Element::fromName("grass");
            } else if (depth < 3) {
                grid[y][x] = Element::fromName("dirt");
            } else if (depth < 7) {
                grid[y][x] = chance(30) ? Element::fromName("gravel") : Element::fromName("rock");
            } else {
                grid[y][x] = chance(8) ? Element::fromName("coal") : Element::fromName("rock");
            }
        }
    }

    // 3) Carve winding cave tunnels.
    int tunnelCount = std::max(2, worldW / 20);
    for (int t = 0; t < tunnelCount; ++t) {
        int cx = randInt(xStart + 2, xEnd - 3);
        int cy = randInt(yStart + worldH / 3, yEnd - 3);
        int steps = randInt(worldW / 2, worldW);

        for (int s = 0; s < steps; ++s) {
            int radius = chance(25) ? 2 : 1;
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    int ny = cy + dy;
                    int nx = cx + dx;
                    if (ny >= yStart && ny < yEnd && nx >= xStart && nx < xEnd) {
                        grid[ny][nx] = Element::fromName("air");
                    }
                }
            }

            cx += randInt(-1, 1);
            cy += randInt(-1, 1);
            cx = std::clamp(cx, xStart + 1, xEnd - 2);
            cy = std::clamp(cy, yStart + worldH / 4, yEnd - 2);
        }
    }

    // 4) Add surface lakes.
    int lakeCount = std::max(1, worldW / 35);
    for (int i = 0; i < lakeCount; ++i) {
        int centerX = randInt(xStart + 4, xEnd - 5);
        int halfW = randInt(3, 7);
        for (int x = centerX - halfW; x <= centerX + halfW; ++x) {
            if (x < xStart || x >= xEnd) continue;
            int sx = surface[x - xStart];
            int waterDepth = randInt(2, 4);
            for (int y = sx; y < std::min(yEnd, sx + waterDepth); ++y) {
                grid[y][x] = Element::fromName("water");
            }
            if (sx < yEnd) {
                grid[sx][x] = Element::fromName("sand");
            }
        }
    }

    // 5) Add deep lava pockets and obsidian shells.
    int lavaPockets = std::max(1, worldW / 45);
    for (int i = 0; i < lavaPockets; ++i) {
        int cx = randInt(xStart + 3, xEnd - 4);
        int cy = randInt(yStart + (worldH * 2) / 3, yEnd - 3);
        int radius = randInt(2, 4);
        std::vector<std::pair<int, int>> lavaCells;

        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int ny = cy + dy;
                int nx = cx + dx;
                if (ny < yStart || ny >= yEnd || nx < xStart || nx >= xEnd) continue;
                int d2 = dx * dx + dy * dy;
                if (d2 <= radius * radius) {
                    grid[ny][nx] = Element::fromName("lava");
                    lavaCells.push_back({ny, nx});
                } else if (d2 <= (radius + 1) * (radius + 1)) {
                    grid[ny][nx] = Element::fromName("obsidian");
                }
            }
        }

        // Ensure every lava tile is fully enclosed so pockets do not leak on spawn.
        for (const auto& cell : lavaCells) {
            int ly = cell.first;
            int lx = cell.second;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dy == 0 && dx == 0) continue;
                    int ny = ly + dy;
                    int nx = lx + dx;
                    if (ny < yStart || ny >= yEnd || nx < xStart || nx >= xEnd) continue;
                    if (grid[ny][nx].getName() != "lava") {
                        grid[ny][nx] = Element::fromName("obsidian");
                    }
                }
            }
        }
    }

    // 6) Sprinkle extra coal veins in deep rock.
    int veinCount = std::max(2, worldW / 18);
    for (int v = 0; v < veinCount; ++v) {
        int x = randInt(xStart + 1, xEnd - 2);
        int y = randInt(yStart + worldH / 2, yEnd - 2);
        int len = randInt(4, 10);
        for (int i = 0; i < len; ++i) {
            if (x >= xStart && x < xEnd && y >= yStart && y < yEnd && grid[y][x].getName() == "rock") {
                grid[y][x] = Element::fromName("coal");
            }
            x += randInt(-1, 1);
            y += randInt(-1, 1);
            x = std::clamp(x, xStart + 1, xEnd - 2);
            y = std::clamp(y, yStart + worldH / 2, yEnd - 2);
        }
    }

    // 7) Place simple trees on recalculated, stable surface tiles.
    for (int x = xStart + 2; x < xEnd - 2; ++x) {
        if (!chance(8)) continue;

        int sx = -1;
        for (int y = yStart; y < yEnd; ++y) {
            if (grid[y][x].getName() != "air") {
                sx = y;
                break;
            }
        }

        if (sx == -1) continue;
        if (sx <= yStart + 2 || sx >= yEnd - 6) continue;
        if (grid[sx][x].getName() != "grass") continue;

        // Skip unstable columns (near cave ceilings / collapsing slopes).
        bool stableColumn = true;
        for (int d = 1; d <= 4; ++d) {
            int sy = sx + d;
            if (sy >= yEnd || grid[sy][x].getName() == "air") {
                stableColumn = false;
                break;
            }
        }
        if (!stableColumn) continue;

        int trunkH = randInt(2, 4);
        for (int h = 1; h <= trunkH; ++h) {
            int ty = sx - h;
            if (ty >= yStart) {
                grid[ty][x] = Element::fromName("wood");
            }
        }

        int crownY = sx - trunkH;
        for (int dy = -2; dy <= 1; ++dy) {
            for (int dx = -2; dx <= 2; ++dx) {
                int ny = crownY + dy;
                int nx = x + dx;
                if (ny >= yStart && ny < yEnd && nx >= xStart && nx < xEnd) {
                    if ((dx * dx + dy * dy <= 5) && chance(80) && grid[ny][nx].getName() == "air") {
                        grid[ny][nx] = Element::fromName("leaf");
                    }
                }
            }
        }
    }

    renderGrid();
}
