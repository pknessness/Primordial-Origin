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

namespace SpacialHash {

map2d<UnitWrappers> *grid;
map2d<UnitWrappers> *gridEnemy;
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

void initGrid() {
    for (int i = 0; i < grid->width(); i++) {
        for (int j = 0; j < grid->height(); j++) {
            imRef(grid, i, j) = UnitWrappers();
        }
    }
}

void resetGrid() {
    for (int i = 0; i < grid->width(); i++) {
        for (int j = 0; j < grid->height(); j++) {
            imRef(grid, i, j).clear();
        }
    }
}

void updateGrid(Agent *agent) {
    resetGrid();
    for (auto it = UnitManager::units.begin(); it != UnitManager::units.end(); it++) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            Point2D p = (*it2)->pos(agent);
            float size = (*it2)->radius;
            float x = p.x - size;
            float y = p.y - size;
            for (int i = 0; i < size * 2 + 1; i++) {
                for (int j = 0; j < size * 2 + 1; j++) {
                    bool activate = false;
                    for (float a = 0; a <= 1; a += 0.25) {
                        for (float b = 0; b <= 1; b += 0.25) {
                            if (a == 0 || a == 1 || b == 0 || b == 1 || (a == 0.5 && b == 0.5)) {
                                Point2D temp{float((int)(i + x) + a), float((int)(j + y) + b)};
                                if (Distance2D(p, temp) < size ||
                                    ((int)(i + x) == (int)(p.x) && std::abs(j + y - p.y) < size) ||
                                    ((int)(j + y) == (int)(p.y) && std::abs(i + x - p.x) < size)) {
                                    activate = true;
                                    break;
                                }
                            }
                            
                        }
                        if (activate) {
                            break;
                        }
                    }
                    if (activate) {
                        //imRef(grid, (int)(i + x), (int)(j + y)) = UnitWrappers();
                        if ((int)(i + x) > 0 && (int)(i + x) < grid->width() && (int)(j + y) > 0 && (int)(j + y) < grid->height()) {
                            imRef(grid, (int)(i + x), (int)(j + y)).push_back(*it2);
                        }


                    }
                }
            }
            DebugSphere(agent,(*it2)->pos3D(agent), size);
        }
    }
}

void fillGridModify2(Point2D pos, float radius, Agent *agent) {
    float x = pos.x - radius;
    float y = pos.y - radius;
    for (int i = 0; i < radius * 2 + 1; i++) {
        for (int j = 0; j < radius * 2 + 1; j++) {
            if (!Aux::isWithin((int)(i + x), (int)(j + y), agent)) {
                continue;
            }
            bool activate = false;
            for (float a = 0; a <= 1; a += 0.25) {
                for (float b = 0; b <= 1; b += 0.25) {
                    if (a == 0 || a == 1 || b == 0 || b == 1 || (a == 0.5 && b == 0.5)) {
                        Point2D temp{ float((int)(i + x) + a), float((int)(j + y) + b) };
                        if (Distance2D(pos, temp) < radius ||
                            ((int)(i + x) == (int)(pos.x) && std::abs(j + y - pos.y) < radius) ||
                            ((int)(j + y) == (int)(pos.y) && std::abs(i + x - pos.x) < radius)) {
                            activate = true;
                            break;
                        }
                    }
                }
                if (activate) {
                    break;
                }
            }
            if (activate) {
                imRef(gridModify, (int)(i + x), (int)(j + y)) = 1;
            }
        }
    }
}

Bounds fillGridModify(Point2D pos, float radius, Agent* agent) {
    int x = (int)(pos.x - radius);
    int y = (int)(pos.y - radius);
    int xmax = (int)(pos.x + radius + 1);
    int ymax = (int)(pos.y + radius + 1);
    if (radius < 1) {
        fillGridModify2(pos, radius, agent);
        return { x, xmax, y, ymax };
    }
    //printf("%d - %d, %d - %d\n", x, xmax, y, ymax);
    imRef(gridModify, int(pos.x), int(pos.y)) = 1;
    for (int i = x; i < xmax; i++) {
        for (int j = y; j < ymax; j++) {
            float f = Distance2D(pos, Point2D{ (float)i,(float)j });
            if (i > 1 && i < gridModify->width() && j > 1 && j < gridModify->height() && f < radius) {
                imRef(gridModify, i, j) = 1;
                imRef(gridModify, i, j - 1) = 1;
                imRef(gridModify, i - 1, j) = 1;
                imRef(gridModify, i - 1, j - 1) = 1;
            }
        }
    }
    return {x, xmax, y, ymax};
}

static UnitWrappers findInRadius_INTERNAL(Bounds b, Agent *agent) {
    UnitWrappers found;
    for (int i = b.xmin; i < b.xmax; i++) {
        for (int j = b.ymin; j < b.ymax; j++) {
            if (!Aux::isWithin(i, j, agent)) {
                continue;
            }
            if (imRef(gridModify, i, j) == 1) {
                UnitWrappers cell = imRef(grid, i, j);
                for (auto it = cell.begin(); it != cell.end(); it++) {
                    if (std::find(found.begin(), found.end(), *it) == found.end()) {
                        found.push_back(*it);
                    }
                }
                //found.insert(found.end(), cell.begin(), cell.end());
                //if (find(found.begin(), found.end()))
            }
        }
    }
    return found;
}

UnitWrappers findInRadius(Point2D pos, float radius, Agent* agent) {
    gridModify->clear();
    Bounds b = fillGridModify(pos, radius, agent);

    return findInRadius_INTERNAL(b, agent);
}

UnitWrappers findInRadius(Circle c, Agent* agent) {
    return findInRadius(c.pos, c.radius, agent);
}

UnitWrappers findInRadii(Circles c, Agent* agent) {
    if (c.size() == 0) {
        return UnitWrappers();
    }
    gridModify->clear();

    Bounds bound = fillGridModify(c[0].pos, c[0].radius, agent);

    for (int i = 1; i < c.size(); i++) {
        bound += fillGridModify(c[i].pos, c[i].radius, agent);
    }

    return findInRadius_INTERNAL(bound, agent);
}

void initGridEnemy() {
    for (int i = 0; i < gridEnemy->width(); i++) {
        for (int j = 0; j < gridEnemy->height(); j++) {
            imRef(gridEnemy, i, j) = UnitWrappers();
        }
    }
}

void resetGridEnemy() {
    for (int i = 0; i < gridEnemy->width(); i++) {
        for (int j = 0; j < gridEnemy->height(); j++) {
            imRef(gridEnemy, i, j).clear();
        }
    }
}

void updateGridEnemy(Agent *agent) {
    resetGridEnemy();
    for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            Point2D p = (*it2)->pos(agent);
            float size = (*it2)->radius;
            float x = p.x - size;
            float y = p.y - size;
            for (int i = 0; i < size * 2 + 1; i++) {
                for (int j = 0; j < size * 2 + 1; j++) {
                    int intX = (int)(i + x);
                    int intY = (int)(j + y);
                    if (intX < 0 || intX >= gridEnemy->width() || intY < 0 || intY >= gridEnemy->height()) {
                        continue;
                    }
                    bool activate = false;
                    for (float a = 0; a <= 1; a += 0.25) {
                        for (float b = 0; b <= 1; b += 0.25) {
                            if (a == 0 || a == 1 || b == 0 || b == 1 || (a == 0.5 && b == 0.5)) {
                                Point2D temp{float(intX + a), float(intY + b)};
                                if (Distance2D(p, temp) < size ||
                                    (intX == (int)(p.x) && std::abs(j + y - p.y) < size) ||
                                    (intY == (int)(p.y) && std::abs(i + x - p.x) < size)) {
                                    activate = true;
                                    break;
                                }
                            }
                        }
                        if (activate) {
                            break;
                        }
                    }
                    if (activate && (*it2)->pos(agent) != Point2D{0,0}) {
                        //imRef(gridEnemy, (int)(i + x), (int)(j + y)) = UnitWrappers();
                        imRef(gridEnemy, intX, intY).push_back(*it2);
                    }
                }
            }
            DebugSphere(agent,(*it2)->pos3D(agent), size);
        }
    }
}

static UnitWrappers findInRadiusEnemy_INTERNAL(Bounds b, Agent* agent, bool excludeHallucination = true) {
    UnitWrappers found;
    for (int i = b.xmin; i < b.xmax; i++) {
        for (int j = b.ymin; j < b.ymax; j++) {
            if (!Aux::isWithin(i, j, agent)) {
                continue;
            }
            if (imRef(gridModify, i, j) == 1) {
                UnitWrappers cell = imRef(gridEnemy, i, j);
                for (auto it = cell.begin(); it != cell.end(); it++) {
                    if (std::find(found.begin(), found.end(), *it) == found.end() && (!excludeHallucination || (*it)->hallucination)) {
                        found.push_back(*it);
                    }
                }
                //found.insert(found.end(), cell.begin(), cell.end());
                //if (find(found.begin(), found.end()))
            }
        }
    }
    return found;
}

UnitWrappers findInRadiusEnemy(Point2D pos, float radius, Agent* agent) {
    gridModify->clear();
    Bounds b = fillGridModify(pos, radius, agent);

    return findInRadiusEnemy_INTERNAL(b, agent);
}

UnitWrappers findInRadiusEnemy(Circle c, Agent* agent) {
    return findInRadiusEnemy(c.pos, c.radius, agent);
}

UnitWrappers findInRadiiEnemy(Circles c, Agent* agent, bool excludeHallucination = true) {
    if (c.size() == 0) {
        return UnitWrappers();
    }
    gridModify->clear();

    Bounds bound = fillGridModify(c[0].pos, c[0].radius, agent);

    for (int i = 1; i < c.size(); i++) {
        bound += fillGridModify(c[i].pos, c[i].radius, agent);
    }

    return findInRadiusEnemy_INTERNAL(bound, agent, excludeHallucination);
}

}