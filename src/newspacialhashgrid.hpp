#pragma once
#include <sc2api/sc2_api.h>

#include <map>
#include "map2d.hpp"

#include "constants.h"
#include "unit.hpp"
#include "debugging.hpp"

struct Circle {
    Point2D pos;
    float radius;
};

using Circles = vector<Circle>;

namespace NewSpacialHash {

constexpr int8_t cellSize = 3;
constexpr int16_t cellCapacity = 210;

struct gridCell {
    uint8_t size;
    UnitWrapper* wrappers[cellCapacity] = { 0 };

    gridCell() {
        memset(wrappers, 0, cellCapacity * sizeof(UnitWrapper*));
        size = 0;
    }

    void add(UnitWrapper* newWrapper) {
        wrappers[size++] = newWrapper;
    }

    UnitWrapper* at(int16_t index) {
        if (index >= size || index < 0) {
            throw std::out_of_range("gridCell out of range");
            return nullptr;
        }
        return wrappers[index];
    }

    void clear() {
        //memset(wrappers, 0, cellCapacity * sizeof(UnitWrapper*));
        size = 0;
    }
};

map2d<gridCell> *grid;
map2d<gridCell> *gridEnemy;
map2d<int8_t>* gridModify;

struct Bounds {
    int xmin;
    int xmax;
    int ymin;
    int ymax;

    Bounds operator+(const Bounds& b) {
        Bounds nu = { xmin,xmax,ymin,ymax };
        if (b.xmin < nu.xmin) {
            nu.xmin = b.xmin;
        }
        if (b.xmax > nu.xmax) {
            nu.xmax = b.xmax;
        }
        if (b.ymin < nu.ymin) {
            nu.ymin = b.ymin;
        }
        if (b.ymax > nu.ymax) {
            nu.ymax = b.ymax;
        }
        return nu;
    }

    void operator+=(const Bounds& b) {
        if (b.xmin < xmin) {
            xmin = b.xmin;
        }
        if (b.xmax > xmax) {
            xmax = b.xmax;
        }
        if (b.ymin < ymin) {
            ymin = b.ymin;
        }
        if (b.ymax > ymax) {
            ymax = b.ymax;
        }
    }
};

Bounds fillSpacialMapRadius(Point2D pos, float radius, std::function<void(int, int)> action) {
    int x = (int)((pos.x - radius) / cellSize);
    int y = (int)((pos.y - radius) / cellSize);
    int xmax = (int)((pos.x + radius) / cellSize) + 1;
    int ymax = (int)((pos.y + radius) / cellSize) + 1;

    //action(int(pos.x / cellSize), int(pos.y / cellSize));

    //DebugLine(agent, AP3D((pos)), AP3D((pos)) + Point3D{ 0,0,2 });
    //DebugText(agent, strprintf("%.1f", radius / cellSize), AP3D(pos) + Point3D{ 0,0,2 });

    for (int i = x; i < xmax; i++) {
        for (int j = y; j < ymax; j++) {
            Point2D blockCenter = Point2D{ (float)i + 0.5F,(float)j + 0.5F };
            float f = Distance2D(pos / cellSize, blockCenter) - 1.414 / 2; //sqrt 2 / 2 because its only half of the square 

            //DebugLine(agent, AP3D((blockCenter * cellSize)), AP3D((blockCenter * cellSize)) + Point3D{ 0,0,2 });
            //DebugText(agent, strprintf("%.1f", f), AP3D((blockCenter * cellSize)) + Point3D{ 0,0,2 });

            if (i > 1 && i < gridModify->width() && j > 1 && j < gridModify->height() && f < (radius / cellSize)) {
                action(i, j);
            }
        }
    }
    return { x, xmax, y, ymax };
}

Bounds fillSpacialModify(Point2D pos, float radius, Agent* agent) {
    return fillSpacialMapRadius(pos, radius, [](int i, int j) {imRef(gridModify, i, j) = 1;});
}

void resetGridModify() {
    //for (int i = 0; i < grid->width(); i++) {
    //    for (int j = 0; j < grid->height(); j++) {
    //        imRef(gridModify, i, j) = 0;
    //    }
    //}
    gridModify->clear();
}

void initGridSelf() {
    //for (int i = 0; i < grid->width(); i++) {
    //    for (int j = 0; j < grid->height(); j++) {
    //        imRef(grid, i, j).clear();
    //    }
    //}
    grid->clear();
}

void resetGridSelf() {
    for (int i = 0; i < grid->width(); i++) {
        for (int j = 0; j < grid->height(); j++) {
            imRef(grid, i, j).clear();
        }
    }
    //grid->clear();
}

void updateGridSelf(Agent *agent) {
    resetGridSelf();
    for (auto it = UnitManager::units.begin(); it != UnitManager::units.end(); it++) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            Bounds b = fillSpacialMapRadius((*it2)->pos(agent), (*it2)->radius, [it2](int i, int j) {imRef(grid, i, j).add(*it2);});
            DebugSphere(agent,(*it2)->pos3D(agent), (*it2)->radius);
        }
    }
}

static UnitWrappers findInRadiusSelf_INTERNAL(Bounds b, Agent *agent) {
    UnitWrappers found;
    for (int i = b.xmin; i < b.xmax; i++) {
        for (int j = b.ymin; j < b.ymax; j++) {
            if (!Aux::isWithin(i, j)) {
                continue;
            }
            if (imRef(gridModify, i, j) == 1) {
                for (int u = 0; u < imRef(grid, i, j).size; u++) {
                    if (std::find(found.begin(), found.end(), imRef(grid, i, j).at(u)) == found.end()) {
                        found.push_back(imRef(grid, i, j).at(u));
                    }
                }
            }
        }
    }
    return found;
}

UnitWrappers findInRadiusSelf(Point2D pos, float radius, Agent* agent) {
    resetGridModify();
    Bounds b = fillSpacialModify(pos, radius, agent);

    return findInRadiusSelf_INTERNAL(b, agent);
}

UnitWrappers findInRadiusSelf(Circle c, Agent* agent) {
    return findInRadiusSelf(c.pos, c.radius, agent);
}

UnitWrappers findInRadiiSelf(Circles c, Agent* agent) {
    if (c.size() == 0) {
        return UnitWrappers();
    }
    resetGridModify();

    Bounds bound = fillSpacialModify(c[0].pos, c[0].radius, agent);
    for (int i = 1; i < c.size(); i++) {
        bound += fillSpacialModify(c[i].pos, c[i].radius, agent);
    }

    return findInRadiusSelf_INTERNAL(bound, agent);
}

void initGridEnemy() {
    //for (int i = 0; i < gridEnemy->width(); i++) {
    //    for (int j = 0; j < gridEnemy->height(); j++) {
    //        imRef(gridEnemy, i, j).clear();
    //    }
    //}
    gridEnemy->clear();
}

void resetGridEnemy() {
    for (int i = 0; i < gridEnemy->width(); i++) {
        for (int j = 0; j < gridEnemy->height(); j++) {
            imRef(gridEnemy, i, j).clear();
        }
    }
    //gridEnemy->clear();
}

void updateGridEnemy(Agent* agent) {
    resetGridEnemy();
    for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            Bounds b = fillSpacialMapRadius((*it2)->pos(agent), (*it2)->radius, [it2](int i, int j) {imRef(gridEnemy, i, j).add(*it2);});
            DebugSphere(agent, (*it2)->pos3D(agent), (*it2)->radius);
        }
    }
}

static UnitWrappers findInRadiusEnemy_INTERNAL(Bounds b, Agent* agent) {
    UnitWrappers found;
    for (int i = b.xmin; i < b.xmax; i++) {
        for (int j = b.ymin; j < b.ymax; j++) {
            if (!Aux::isWithin(i, j)) {
                continue;
            }
            if (imRef(gridModify, i, j) == 1) {
                for (int u = 0; u < imRef(gridEnemy, i, j).size; u++) {
                    if (std::find(found.begin(), found.end(), imRef(gridEnemy, i, j).at(u)) == found.end()) {
                        found.push_back(imRef(gridEnemy, i, j).at(u));
                    }
                }
            }
        }
    }
    return found;
}

UnitWrappers findInRadiusEnemy(Point2D pos, float radius, Agent* agent) {
    resetGridModify();
    Bounds b = fillSpacialModify(pos, radius, agent);

    return findInRadiusEnemy_INTERNAL(b, agent);
}

UnitWrappers findInRadiusEnemy(Circle c, Agent* agent) {
    return findInRadiusEnemy(c.pos, c.radius, agent);
}

UnitWrappers findInRadiiEnemy(Circles c, Agent* agent) {
    if (c.size() == 0) {
        return UnitWrappers();
    }
    resetGridModify();

    Bounds bound = fillSpacialModify(c[0].pos, c[0].radius, agent);
    for (int i = 1; i < c.size(); i++) {
        bound += fillSpacialModify(c[i].pos, c[i].radius, agent);
    }

    return findInRadiusEnemy_INTERNAL(bound, agent);
}
}