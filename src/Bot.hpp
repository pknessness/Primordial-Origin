// Anshal Jain

#include <sc2api/sc2_api.h>
#include <sc2lib/sc2_lib.h>

#include <iostream>
#include "jps.hpp"
#include "macro.hpp"
#include "grid.hpp"
#include "tools.hpp"
#include "constants.h"
#include "primordialstar.hpp"
#include "unitmanager.hpp"
#include "dist_transform.hpp"
#include "zhangSuen.hpp"
#include "spacialhashgrid.hpp"
#include "BoudaoudSiderTariThinning.hpp"
#include <sc2api/sc2_gametypes.h>
#include <sc2utils/sc2_arg_parser.h>
#include "debugging.hpp"
#include "bmp.hpp"
#include "sc2api/sc2_client.h"
#include "armyControl.hpp"
#include "strategy.hpp"

#define DISPLAY_AIR 0
#define MICRO_TEST 0
#define START_ME Point2D{40,50}
#define START_OP Point2D{60,50}

#define MICRO_TEST_2 0

using namespace sc2;

class Bot : public Agent {
public:
    //std::vector<Location> path;
    std::vector < std::vector<Location>> expansionPaths;
    std::vector<Point2DI> path;

    std::vector<Point3D> expansions;
    std::vector<double> expansionDistance;
    std::vector<double> rankedExpansionDistance;

    //Point2DI staging_location;
    Point2D rally_point;
    //Point2D enemy;

    map2d<int8_t>* path_zhang_suen;

    clock_t last_time;

    string fileName = "";

    Point3D P3D(const Point2D& p) {
        return Point3D(p.x, p.y, Observation()->TerrainHeight(p));
    }

    void initializeStartings() {
        GameInfo game_info = Observation()->GetGameInfo();
        if (Observation()->GetStartLocation().x > game_info.width / 2) {
            Aux::staging_location.x = Observation()->GetStartLocation().x - 6;
        } else {
            Aux::staging_location.x = Observation()->GetStartLocation().x + 6;
        }

        if (Observation()->GetStartLocation().y > game_info.height / 2) {
            Aux::staging_location.y = Observation()->GetStartLocation().y - 6;
        } else {
            Aux::staging_location.y = Observation()->GetStartLocation().y + 6;
        }
        Aux::startLoc = Observation()->GetStartLocation();
        Aux::enemyLoc = Observation()->GetGameInfo().enemy_start_locations[0];
    }

    void initializeExpansions() {
        // staging_location = Point2DI(startLocation.x + ;
        expansions = sc2::search::CalculateExpansionLocations(Observation(), Query());
        for (auto point : expansions) {
            if (point.x == 0 && point.y == 0)
                continue;
            // auto path = generator.findPath(staging_location, (Point2DI)point);
            // expansionDistance.push_back(path.size());
            //auto came_from = jps(gridmap, staging_location, {int(point.x),int(point.y)}, Tool::euclidean, this);
            //auto pathToExpansion = Tool::reconstruct_path(staging_location, {int(point.x), int(point.y)}, came_from);

            //double length = fullDist(pathToExpansion);
            //expansionDistance.push_back(length);

            //for (Location l : pathToExpansion) {
            //    printf("[%d,%d]", l.x, l.y);
            //}
            //printf("{%.1f}\n\n", length);

            float length = PrimordialStar::getPathLength(P2D(Aux::staging_location), P2D(point), 0.2, this);

            constexpr int numsteps = 6;
            Units neut = Observation()->GetUnits(Unit::Alliance::Neutral, Aux::isMineral);
            Units vesp = Observation()->GetUnits(Unit::Alliance::Neutral, Aux::isVespene);
            neut.insert(neut.end(), vesp.begin(), vesp.end());
            for (int i = 0; i < neut.size(); i++) {
                if (Distance2D(point, neut[i]->pos) < 12) {
                    Point2D dir = neut[i]->pos - point;
                    for (int p = 0; p < numsteps; p++) {
                        Point3D loc = point + p * (neut[i]->pos - point) / ((float)numsteps);
                        Aux::addPlacement(loc, 2);
                        //DebugSphere(this,loc, 2);
                    }
                }
            }
            Aux::removePlacement(point, 5);

            if (Aux::rankedExpansions.size() == 0) {
                Aux::rankedExpansions.push_back(point);
                rankedExpansionDistance.push_back(length);
            }
            for (int i = 0; i < Aux::rankedExpansions.size(); i ++) {
                if (rankedExpansionDistance[i] > length) {
                    Aux::rankedExpansions.insert(Aux::rankedExpansions.begin() + i, point);
                    rankedExpansionDistance.insert(rankedExpansionDistance.begin() + i, length);
                    break;
                }
            }
            
        }
        //Debug()->SendDebug();
    }

    void grid(bool nearbyOnly = true) {
        GameInfo game_info = Observation()->GetGameInfo();

        int mapWidth = game_info.width;
        int mapHeight = game_info.height - 1;

        Point2D center = Observation()->GetCameraPos();
        int wS = int(center.x) - 10;
        if (wS < 0 || !nearbyOnly)
            wS = 0;
        int hS = int(center.y) - 5;
        if (hS < 0 || !nearbyOnly)
            hS = 0;
        int wE = int(center.x) + 11;
        if (wE > mapWidth || !nearbyOnly)
            wE = mapWidth;
        int hE = int(center.y) + 14;
        if (hE > mapHeight || !nearbyOnly)
            hE = mapHeight;

        int fontSize = 8;

        #define BOX_BORDER 0.02

        for (int w = wS; w < wE; w++) {
            for (int h = hS; h < hE; h++) {
                Point2DI point = Point2DI(w, h);
                float boxHeight = 0;
                Color c(255,255,255);

                //for (auto loc : path) {
                //    if (loc.x == w && loc.y == h) {
                //        c = Color(120, 23, 90);
                //        break;
                //    }
                //}

                //if (imRef(path_zhang_suen, w, h)) {
                //    c = {250, 200, 210};
                //}
                //else if (imRef(Aux::buildingBlocked, w, h) != 0) {
                //    //boxHeight = 1;
                //    c = {123,50,10};
                //} else if (imRef(Aux::influenceMap, w, h) != 0) {
                //    c = {44, 50, 210};
                //} else if (imRef(Aux::influenceMapEnemy, w, h) != 0) {
                //    c = {210,50, 44};
                //}
                if (Aux::checkPathable(w, h, this)) {
                    c = { 250, 200, 210 };
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255) || boxHeight != 0) {
                    float height = (Observation()->TerrainHeight(Point2D{ float(w), float(h)}) + Observation()->TerrainHeight(Point2D{ w + 1.0F, h + 1.0F }))/2;
                    //float height = Observation()->TerrainHeight(Point2D{ w + 0.5F, h + 0.5F });
                    

                    DebugBox(this,Point3D(w + BOX_BORDER, h + BOX_BORDER, height + 0.01F),
                                         Point3D(w + 1 - BOX_BORDER, h + 1 - BOX_BORDER, height + boxHeight), c);
                    //#if MICRO_TEST
                    //    DebugText(this,strprintf("%d, %d", w, h),
                    //                          Point3D(w + BOX_BORDER, h + 0.2 + BOX_BORDER, height + 0.1),
                    //                          Color(200, 90, 15), 4);
                    //#endif

                    DebugText(this, strprintf("%d, %d", w, h),
                        Point3D(w + BOX_BORDER, h + 0.2 + BOX_BORDER, height + 0.1),
                        Color(200, 90, 15), 8);

                    /*std::string cs = imRef(display, w, h);
                    float disp = cs.length() * 0.0667 * fontSize / 15;
                    DebugText(this,cs, Point3D(w + 0.5 - disp, h + 0.5, height + 0.1 + displace),
                                          Color(200, 190, 115), fontSize);*/
                }
            }
        }
    }

    void displayMaxDistanceGrid(float maximal, bool nearbyOnly = true) {
        GameInfo game_info = Observation()->GetGameInfo();

        int mapWidth = game_info.width;
        int mapHeight = game_info.height - 1;

        Point2D center = Observation()->GetCameraPos();
        int wS = int(center.x) - 10;
        if (wS < 0 || !nearbyOnly)
            wS = 0;
        int hS = int(center.y) - 5;
        if (hS < 0 || !nearbyOnly)
            hS = 0;
        int wE = int(center.x) + 11;
        if (wE > mapWidth || !nearbyOnly)
            wE = mapWidth;
        int hE = int(center.y) + 14;
        if (hE > mapHeight || !nearbyOnly)
            hE = mapHeight;

        int fontSize = 8;

        #define BOX_BORDER 0.02

        for (int w = wS; w < wE; w++) {
            for (int h = hS; h < hE; h++) {
                Point2DI point = Point2DI(w, h);
                float boxHeight = 0;
                Color c(255, 255, 255);

                if (Aux::checkPathable(w, h, this)) {
                    //c = { uint8_t(imRef(PrimordialStar::maxDistanceGrid,w,h).distance/maximal * 255.0), 0, 0};
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255) || boxHeight != 0) {
                    float height = (Observation()->TerrainHeight(Point2D{ float(w), float(h) }) + Observation()->TerrainHeight(Point2D{ w + 1.0F, h + 1.0F })) / 2;
                    //float height = Observation()->TerrainHeight(Point2D{ w + 0.5F, h + 0.5F });


                    DebugBox(this, Point3D(w + BOX_BORDER, h + BOX_BORDER, height + 0.01F),
                        Point3D(w + 1 - BOX_BORDER, h + 1 - BOX_BORDER, height + boxHeight), c);
                    //#if MICRO_TEST
                    //    DebugText(this,strprintf("%d, %d", w, h),
                    //                          Point3D(w + BOX_BORDER, h + 0.2 + BOX_BORDER, height + 0.1),
                    //                          Color(200, 90, 15), 4);
                    //#endif

                    DebugText(this, strprintf("%d, %d", w, h),
                        Point3D(w + BOX_BORDER, h + 0.2 + BOX_BORDER, height + 0.1),
                        Color(200, 90, 15), 8);

                    /*std::string cs = imRef(display, w, h);
                    float disp = cs.length() * 0.0667 * fontSize / 15;
                    DebugText(this,cs, Point3D(w + 0.5 - disp, h + 0.5, height + 0.1 + displace),
                                          Color(200, 190, 115), fontSize);*/
                }
            }
        }
    }

    void displaySpacialHashGrid() {
        GameInfo game_info = Observation()->GetGameInfo();

        int mapWidth = game_info.width;
        int mapHeight = game_info.height;

        Point2D center = Observation()->GetCameraPos();
        int wS = int(center.x) - 10;
        if (wS < 0)
            wS = 0;
        int hS = int(center.y) - 5;
        if (hS < 0)
            hS = 0;
        int wE = int(center.x) + 11;
        if (wE > mapWidth)
            wE = mapWidth;
        int hE = int(center.y) + 14;
        if (hE > mapHeight)
            hE = mapHeight;

        int fontSize = 8;

        #define BOX_BORDER 0.02

        for (int w = wS; w < wE; w++) {
            for (int h = hS; h < hE; h++) {
                Point2DI point = Point2DI(w, h);
                float boxHeight = 0;
                Color c(255, 255, 255);

                // for (auto loc : path) {
                //     if (loc.x == w && loc.y == h) {
                //         c = Color(120, 23, 90);
                //         break;
                //     }
                // }

                if (imRef(SpacialHash::grid, w, h).size() != 0) {
                        c = {20, 200, 210};
                } else if (imRef(SpacialHash::gridEnemy, w, h).size() != 0) {
                        c = {210, 200, 20};
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255)) {
                    float height = Observation()->TerrainHeight(Point2D{ w + 0.5F, h + 0.5F });

                    DebugBox(this,Point3D(w + BOX_BORDER, h + BOX_BORDER, height + 0.01),
                                            Point3D(w + 1 - BOX_BORDER, h + 1 - BOX_BORDER, height + boxHeight), c);
                    #if MICRO_TEST
                        DebugText(this,strprintf("%d, %d", w, h),
                                            Point3D(w + BOX_BORDER, h + 0.2 + BOX_BORDER, height + 0.1),
                                            Color(200, 90, 15), 4);
                    #endif
                    /*std::string cs = imRef(display, w, h);
                    float disp = cs.length() * 0.0667 * fontSize / 15;
                    DebugText(this,cs, Point3D(w + 0.5 - disp, h + 0.5, height + 0.1 + displace),
                                            Color(200, 190, 115), fontSize);*/
                }
            }
        }
    }

    void displaySpacialHashGridTEST() {
        GameInfo game_info = Observation()->GetGameInfo();

        int mapWidth = game_info.width;
        int mapHeight = game_info.height;

        Point2D center = Observation()->GetCameraPos();
        int wS = int(center.x) - 10;
        if (wS < 0)
            wS = 0;
        int hS = int(center.y) - 5;
        if (hS < 0)
            hS = 0;
        int wE = int(center.x) + 11;
        if (wE > mapWidth)
            wE = mapWidth;
        int hE = int(center.y) + 14;
        if (hE > mapHeight)
            hE = mapHeight;

        int fontSize = 8;

        #define BOX_BORDER 0.02

        for (int w = wS; w < wE; w++) {
            for (int h = hS; h < hE; h++) {
                Point2DI point = Point2DI(w, h);
                float boxHeight = 0;
                Color c(255, 255, 255);

                // for (auto loc : path) {
                //     if (loc.x == w && loc.y == h) {
                //         c = Color(120, 23, 90);
                //         break;
                //     }
                // }

                if (imRef(SpacialHash::gridModify, w, h) != 0) {
                    c = { 20, 200, 210 };
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255)) {
                    float height = Observation()->TerrainHeight(Point2D{ w + 0.5F, h + 0.5F });

                    DebugBox(this, Point3D(w + BOX_BORDER, h + BOX_BORDER, height + 0.01),
                        Point3D(w + 1 - BOX_BORDER, h + 1 - BOX_BORDER, height + boxHeight), c);
                #if MICRO_TEST
                    DebugText(this, strprintf("%d, %d", w, h),
                        Point3D(w + BOX_BORDER, h + 0.2 + BOX_BORDER, height + 0.1),
                        Color(200, 90, 15), 4);
                #endif
                    /*std::string cs = imRef(display, w, h);
                    float disp = cs.length() * 0.0667 * fontSize / 15;
                    DebugText(this,cs, Point3D(w + 0.5 - disp, h + 0.5, height + 0.1 + displace),
                                            Color(200, 190, 115), fontSize);*/
                }
            }
        }
    }

    void displayEnemyDamageGrid2() {
        GameInfo game_info = Observation()->GetGameInfo();

        int mapWidth = game_info.width;
        int mapHeight = game_info.height;

        Point2D center = Observation()->GetCameraPos();
        int wS = int(center.x) - 10;
        if (wS < 0)
            wS = 0;
        int hS = int(center.y) - 5;
        if (hS < 0)
            hS = 0;
        int wE = int(center.x) + 11;
        if (wE > mapWidth)
            wE = mapWidth;
        int hE = int(center.y) + 14;
        if (hE > mapHeight)
            hE = mapHeight;

        int fontSize = 8;

        #define BOX_BORDER_S 0.002

        for (float w = wS; w < wE; w += blockSize) {
            for (float h = hS; h < hE; h += blockSize) {
                Point2D p(w, h);
                float boxHeight = 0;
                Color c(255, 255, 255);

                // for (auto loc : path) {
                //     if (loc.x == w && loc.y == h) {
                //         c = Color(120, 23, 90);
                //         break;
                //     }
                // }

                if (UnitManager::damageNetEnemy(p).ground.normal != 0 || UnitManager::damageNetEnemy(p).air.normal != 0) {
                    uint8_t r = (UnitManager::damageNetEnemy(p).ground.normal * 2);
                    uint8_t b = (UnitManager::damageNetEnemy(p).air.normal * 2);
                    //printf("col %d %d\n", r, b);
                    c = {r, 0, b};
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255)) {
                    float height = Observation()->TerrainHeight(Point2D{w+0.5F, h+0.5F});

                    DebugBox(this,Point3D(w + BOX_BORDER_S, h + BOX_BORDER_S, height + 0.01),
                        Point3D(w + blockSize - BOX_BORDER_S, h + blockSize - BOX_BORDER_S, height - 0.01), c);
                #if MICRO_TEST
                    DebugText(this,strprintf("%d, %d", w, h),
                        Point3D(w + BOX_BORDER_S, h + 0.2 + BOX_BORDER_S, height + 0.1),
                        Color(200, 90, 15), 4);
                #endif
                    /*std::string cs = imRef(display, w, h);
                    float disp = cs.length() * 0.0667 * fontSize / 15;
                    DebugText(this,cs, Point3D(w + 0.5 - disp, h + 0.5, height + 0.1 + displace),
                                            Color(200, 190, 115), fontSize);*/
                }
            }
        }
    }

    void displayEnemyDamageGrid() {
        GameInfo game_info = Observation()->GetGameInfo();

        int mapWidth = game_info.width * UnitManager::damageNetPrecision;
        int mapHeight = game_info.height * UnitManager::damageNetPrecision;

        Point2D center = Observation()->GetCameraPos() * UnitManager::damageNetPrecision;
        int wS = int(center.x) - 10 * UnitManager::damageNetPrecision;
        if (wS < 0)
            wS = 0;
        int hS = int(center.y) - 5 * UnitManager::damageNetPrecision;
        if (hS < 0)
            hS = 0;
        int wE = int(center.x) + 11 * UnitManager::damageNetPrecision;
        if (wE > mapWidth)
            wE = mapWidth;
        int hE = int(center.y) + 14 * UnitManager::damageNetPrecision;
        if (hE > mapHeight)
            hE = mapHeight;

        int fontSize = 8;

        #define BOX_BORDER_SD 0.002

        float damageMult = 255.0 / 10000;

        for (int w = wS; w < wE; w += 1) {
            for (int h = hS; h < hE; h += 1) {
                Point2D p(w * blockSize, h * blockSize);
                float boxHeight = 0;
                Color c(255, 255, 255);
                float height = Observation()->TerrainHeight(Point2D{ (w + 0.5F) * blockSize, (h + 0.5F) * blockSize });

                if (UnitManager::damageNetEnemy(p).ground.normal != 0) {
                    uint8_t r = (uint8_t)(UnitManager::damageNetEnemy(p).ground.normal * damageMult);
                    uint8_t g = (uint8_t)(UnitManager::damageNetEnemy(p).ground.light * damageMult);
                    uint8_t b = (uint8_t)(UnitManager::damageNetEnemy(p).ground.armored * damageMult);
                    //printf("col %d %d\n", r, b);
                    c = { r, g, b };
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255)) {
                    
                    Point3D corner1{ w * blockSize, h * blockSize, height + 0.01F };
                    Point3D corner2{ (w + 1) * blockSize, (h + 1) * blockSize, height + 0.01F };
                    DebugBox(this,corner1,corner2, c);
                }

                #if DISPLAY_AIR

                c = { 255, 255, 255 };

                if (damageNetEnemy(p).air.normal != 0) {
                    uint8_t r = (uint8_t)(damageNetEnemy(p).air.normal * damageMult);
                    uint8_t g = (uint8_t)(damageNetEnemy(p).air.light * damageMult);
                    uint8_t b = (uint8_t)(damageNetEnemy(p).air.armored * damageMult);
                    c = { r, g, b };
                }

                if (0 || !(c.r == 255 && c.g == 255 && c.b == 255)) {
                    Point3D corner1{ w * blockSize, h * blockSize, height + 5.01F };
                    Point3D corner2{ (w + 1) * blockSize, (h + 1) * blockSize, height + 5.01F };
                    DebugBox(this,corner1, corner2, c);
                }
                #endif

                float damage = imRef(UnitManager::enemyDamageNetTemp, w, h);

                if (damage != 0.0) {
                    DebugText(this,strprintf("%d", int(damage)),
                        Point3D(w * blockSize, h * blockSize + 0.1F, height + 0.2F),
                        Color(200, 90, 15), 8);
                }
            }
        }
    }

    void displayPrimordialStarNodes() {
        for (int i = 0; i < PrimordialStar::basePathNodes.size(); i++) {
            PrimordialStar::PathNode* node = PrimordialStar::basePathNodes[i];
            if (Distance2D(node->rawPos(), Observation()->GetCameraPos()) > 15) {
                continue;
            }
            //for (int c = 0; c < node->connected.size(); c++) {
            //    PrimordialStar::PathNode* node2 = PrimordialStar::basePathNodes[node->connected[c]];
            //    DebugLine(this, P3D(node->rawPos()) + Point3D{ 0,0,3 }, P3D(node2->rawPos()) + Point3D{ 0,0,3 }, Colors::Blue);
            //}
            DebugSphere(this, P3D(node->rawPos()), 0.5);
        }
        DebugSphere(this, P3D(Observation()->GetCameraPos()), 0.5);
        //if (PrimordialStar::checkLinearPath(rally_point, Observation()->GetCameraPos(), this)) {
        //    DebugLine(this, P3D(rally_point) + Point3D{ 0,0,1 }, P3D(Observation()->GetCameraPos()) + Point3D{ 0,0,1 }, Colors::Green);
        //}
        //else {
        //    DebugLine(this, P3D(rally_point) + Point3D{ 0,0,1 }, P3D(Observation()->GetCameraPos()) + Point3D{ 0,0,1 }, Colors::Red);
        //}
        Profiler* p = new Profiler("pathfind");
        vector<Point2D> path = PrimordialStar::getPath(Observation()->GetCameraPos(), rally_point, 0.5, this);
        delete p;

        DebugText(this, strprintf("%d", path.size()), P3D(Observation()->GetCameraPos()) + Point3D{ 0,0,1 });
        if (path.size() != 0) {
            float l2 = 0;
            for (int i = 0; i < path.size() - 1; i++) {
                l2 += Distance2D(path[i], path[i + 1]);
                DebugLine(this, P3D(path[i]) + Point3D{ 0,0,1 }, P3D(path[i + 1]) + Point3D{ 0,0,1 }, Colors::Green);
            }
            vector<Point2D> subpoints = PrimordialStar::stepPointsAlongPath(path, 1.0F);
            for (Point2D p : subpoints) {
                DebugSphere(this, P3D(p), 0.5, { 61,102,220 });
            }
            //DebugSphere(this, P3D(PrimordialStar::distanceAlongPath(path, 5.5)), 0.5, {21,42,120});
            //printf("len: %.1f len2: %.1f\n", PrimordialStar::getPathLength(Observation()->GetCameraPos(), rally_point, 0.5, this), l2);
        }
        else {
            DebugLine(this, P3D(rally_point) + Point3D{ 0,0,1 }, P3D(Observation()->GetCameraPos()) + Point3D{ 0,0,1 }, Colors::Red);
        }

    }

    void pathVerification() {
        #define NUM_PTS_RT 40
        vector<Point2D> pts;
        pts.reserve(NUM_PTS_RT);

        for (int asd = 0; asd < NUM_PTS_RT; asd++) {
            pts.push_back( Aux::getRandomPathable(this));
        }

        vector<float> differenceInDistance;
        differenceInDistance.reserve(NUM_PTS_RT * NUM_PTS_RT);

        timeus startTime = std::chrono::steady_clock::now();
        for (int a = 0; a < NUM_PTS_RT; a++) {
            Point2D from = pts[a];
            DebugSphere(this, P3D(from), 0.25, { 61,102,220 });
            for (int b = 0; b < NUM_PTS_RT; b++) {
                Point2D to = pts[b];
                auto path = PrimordialStar::getPathDijkstra(from, to, 0, this);
                float dist = PrimordialStar::getPathLength(path);
                float sc2dist = Query()->PathingDistance(from, to);
                float diff = dist - sc2dist;
                if (0 && diff < 0) {
                    printf("S{%.1f,%.1f} E{%.1f,%.1f} D[%.1f]\n", from.x, from.y, to.x, to.y, diff);
                }
                if (diff > 2 || diff < -2) {
                    Color c = Aux::randomColor();
                    if (diff < 0) {
                        c.r = 0;
                    }
                    else if (diff > 0) {
                        c.g = 0;
                    }
                    float z = std::rand() * 2.0F / RAND_MAX;
                    if (path.size() > 0) {
                        for (int i = 0; i < path.size() - 1; i++) {
                            DebugLine(this, P3D(path[i]) + Point3D{ 0,0,1.5F + z }, P3D(path[i + 1]) + Point3D{ 0,0,1.5F + z }, c);
                        }
                    }
                    else {
                        DebugLine(this, P3D(from) + Point3D{ 0,0,1.5F + z }, P3D(to) + Point3D{ 0,0,1.5F + z }, c);
                    }
                    
                }
                differenceInDistance.push_back(diff);
            }
        }
        timeus endTime = std::chrono::steady_clock::now();
        long long dt = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        float mean = 0;
        float min = 400;
        float max = 0;
        for (float f : differenceInDistance) {
            if (f < min) {
                min = f;
            }
            if (f > max) {
                max = f;
            }
            mean += f;
            //printf("%.1f\n", f);
        }
        mean /= differenceInDistance.size();
        printf("PATH VERIFICATION [%d^2] %.1fs MEAN:%.1f MIN:%.1f MAX:%.1f\n", NUM_PTS_RT, dt / 1000000.0, mean, min, max);
        SendDebug(this);
    }

    void listUnitWraps() {
        string tot = "UNITS:\n";
        for (auto it = UnitManager::units.begin(); it != UnitManager::units.end(); it++) {
            auto all = it->second;
            string type = UnitTypeToName(it->first);
            tot += ("\n"+ type + ":\n");
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                tot += strprintf("%lx\n",(*it2)->self);
            }
        }
        DebugText(this,tot, Point2D(0.01, 0.01), Color(100, 190, 215), 8);
    }

    void listUnitWrapsNeutral() {
        string tot = "UNITS:\n";
        for (auto it = UnitManager::neutrals.begin(); it != UnitManager::neutrals.end(); it++) {
            auto all = it->second;
            string type = UnitTypeToName(it->first);
            tot += ("\n" + type + ":\n");
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                tot += strprintf("%lx\n", (*it2)->self);
            }
        }
        DebugText(this,tot, Point2D(0.11, 0.01), Color(100, 190, 215), 8);
    }

    void listUnitWrapsEnemies() {
        string tot = "UNITS:\n";
        for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
            auto all = it->second;
            string type = UnitTypeToName(it->first);
            tot += ("\n" + type + ":\n");
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                tot += strprintf("%lx\n", (*it2)->self);
            }
        }
        DebugText(this,tot, Point2D(0.21, 0.01), Color(100, 190, 215), 8);
    }

    void listMacroActions() {
        string tot = "MACRO:\n";
        for (auto it = Macro::actions.begin(); it != Macro::actions.end(); it++) {
            auto all = it->second;
            string type = UnitTypeToName(it->first);
            tot += ("\n" + type + ":\n");
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                tot += strprintf("%s %d %.1f,%.1f\n", AbilityTypeToName(it2->ability), it2->index, it2->pos.x, it2->pos.y);
            }
        }
        DebugText(this,tot, Point2D(0.01, 0.11), Color(250, 50, 15), 8);
    }

    void probeLines() {
        auto probes = UnitManager::get(UNIT_TYPEID::PROTOSS_PROBE);
        for (auto it = probes.begin(); it != probes.end(); it++) {
            // printf("Probe %xu Mineral %xu\n", it->first, it->second.minerals);
            Probe* probe = ((Probe*)*it);
            if (Observation()->GetUnit(probe->getTargetTag(this)) != nullptr && Observation()->GetUnit(probe->self) != nullptr)
                DebugLine(this,Observation()->GetUnit(probe->getTargetTag(this))->pos + Point3D(0, 0, 1),
                                      Observation()->GetUnit(probe->self)->pos + Point3D(0, 0, 1), Color(20, 90, 215));
            else if (Observation()->GetUnit(probe->self) != nullptr) {
                DebugLine(this,Observation()->GetUnit(probe->self)->pos,
                                      Observation()->GetUnit(probe->self)->pos + Point3D(0, 0, 1), Color(20, 90, 215));
            }
        }
    }

    void orderDisplay() {
        Units units = Observation()->GetUnits(sc2::Unit::Alliance::Self);
        for (const Unit* unit : units) {
            if (unit->orders.size() == 0)
                continue;
            #define LETTER_DISP -0.07F
            string s = "";
            if (unit->orders[0].target_unit_tag != NullTag && unit->orders[0].target_pos.x != 0 && unit->orders[0].target_pos.y != 0){
                s += strprintf("%s [%lx] [%.1f, %.1f]", AbilityTypeToName(unit->orders[0].ability_id),
                                     unit->orders[0].target_unit_tag, unit->orders[0].target_pos.x,
                                     unit->orders[0].target_pos.y);
                
            }else if (unit->orders[0].target_pos.x != 0 && unit->orders[0].target_pos.y != 0) {
                s += strprintf("%s [%.1f, %.1f]", AbilityTypeToName(unit->orders[0].ability_id),
                                      unit->orders[0].target_pos.x, unit->orders[0].target_pos.y);
            } else if (unit->orders[0].target_unit_tag != NullTag) {
                s += strprintf("%s [%lx]", AbilityTypeToName(unit->orders[0].ability_id),
                                     unit->orders[0].target_unit_tag);
            } else {
                s += strprintf("%s", AbilityTypeToName(unit->orders[0].ability_id));
            }

            if (unit->weapon_cooldown != 0) {
                s += strprintf(" %.1fs", unit->weapon_cooldown);
            }
            DebugText(this,s, unit->pos + Point3D{s.size() * LETTER_DISP, 0, 0.5}, Color(100, 210, 55), 8);
        }
    }

    void tagDisplay() {
        Units units = Observation()->GetUnits(sc2::Unit::Alliance::Self);
        for (const Unit* unit : units) {
            if (unit->orders.size() == 0)
                continue;
            #define LETTER_DISP -0.07F
            string s = strprintf("%lx", unit->tag);
            DebugText(this,s, unit->pos + Point3D{s.size() * LETTER_DISP, 0.3, 0.5}, Color(210, 55, 55), 8);
        }
    }

    void neutralDisplay() {
        for (auto it = UnitManager::neutrals.begin(); it != UnitManager::neutrals.end(); it++) {
            auto all = it->second;
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                #define LETTER_DISP -0.07F
                string s = strprintf("%lx", (*it2)->self);
                DebugText(this,s, (*it2)->pos3D(this) + Point3D{s.size() * LETTER_DISP, 0.3, 0.5}, Color(210, 55, 55),
                                      8);
            }
        }
    }

    void buildingDisplay() {
        auto probes = UnitManager::get(UNIT_TYPEID::PROTOSS_PROBE);
        for (auto it = probes.begin(); it != probes.end(); it++) {
            Probe* probe = ((Probe*)*it);
            if (probe->buildings.size() == 0)
                continue;
            DebugText(this,strprintf("%s %d,%d", AbilityTypeToName(probe->buildings[0].build),
                                            probe->buildings[0].pos.x, probe->buildings[0].pos.y),
                                  Observation()->GetUnit(probe->self)->pos + Point3D{0, 0, 0}, Color(100, 30, 55),
                                  8);
        }
    }

    void enemiesDisplay() {
        for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
            auto all = it->second;
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                string tot = strprintf("%s", UnitTypeToName((*it2)->type));
                DebugText(this,tot, (*it2)->pos3D(this), {255, 40, 10}, 8);
                float radius = Aux::structureDiameter((*it2)->type);
                if (radius == 0) {
                    radius = 0.5;
                }
                DebugSphere(this,(*it2)->pos3D(this), radius, {255, 40, 10});
            }
        }
        
    }

    void expansionsLoc() {
        for (int i = 0; i < Aux::rankedExpansions.size(); i++) {
            Point3D p = P3D(Aux::rankedExpansions[i]);
            DebugSphere(this,p, 12, {253, 216, 53});
            DebugText(this,strprintf("%.1f", rankedExpansionDistance[i]), p + Point3D{0, 0, 0.5});
        }
    }

    void pylonBuildingLoc() {
        for (int i = 0; i < Aux::pylonLocations.size(); i++) {
            Point3D p = P3D(Aux::pylonLocations[i]);
            DebugBox(this,p + Point3D{-1, -1, 0}, p + Point3D{1, 1, 2});
        }
        for (int i = 0; i < Aux::buildingLocations.size(); i++) {
            Point3D p = P3D(Aux::buildingLocations[i]);
            DebugBox(this,p + Point3D{-1.5, -1.5, 0}, p + Point3D{1.5, 1.5, 3});
        }
    }

    void loadEnemyHealth() {
        for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                (*it2)->loadHealth(this);
            }
        }
    }

    //void manageArmy() {
    //    EnemySquads danger = checkDangerAtHome();
    //    if (danger.size() == 0) {
    //        #if MICRO_TEST
    //            squads[0].attack(START_OP);
    //        #elif MICRO_TEST_2
    //            squads[0].attack(Observation()->GetGameInfo().enemy_start_locations[0]);
    //        #else
    //            if (squads[0].army.size() > 11) {
    //                squads[0].attack(Observation()->GetGameInfo().enemy_start_locations[0]);
    //            } else {
    //                squads[0].attack(rally_point);
    //            }
    //        #endif
    //    } else {
    //        for (int i = 0; i < danger.size(); i++) {
    //            if (danger[i].unitComp.size() == 1 && (danger[i].unitComp[0] == UNIT_TYPEID::PROTOSS_PROBE ||
    //                                                   danger[i].unitComp[0] == UNIT_TYPEID::ZERG_DRONE ||
    //                                                   danger[i].unitComp[0] == UNIT_TYPEID::TERRAN_SCV)) {
    //                UnitWrappers probes = UnitManager::get(UNIT_TYPEID::PROTOSS_PROBE);
    //                UnitWrapper* closest = nullptr;
    //                for (UnitWrapper* probeWrap : probes) {
    //                    if (closest == nullptr || Distance2D(probeWrap->pos(this), danger[i].center) <
    //                                                  Distance2D(closest->pos(this), danger[i].center)) {
    //                        closest = probeWrap;
    //                    }
    //                }
    //                Actions()->UnitCommand(closest->self, ABILITY_ID::ATTACK, danger[i].center);
    //            } else {
    //                squads[0].attack(danger[0].center);
    //            }
    //        }
    //    }
    //}

    //EnemySquads checkDangerAtHome() {
    //    //UnitWrappers danger = UnitWrappers();
    //    EnemySquads danger = EnemySquads();
    //    for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
    //        auto all = it->second;
    //        for (auto it2 = all.begin(); it2 != all.end(); it2++) {
    //            Point2D pos = (*it2)->pos(this);
    //            if (pos == Point2D{0, 0}) {
    //                continue;
    //            }
    //            if (imRef(Aux::influenceMap, int(pos.x), int(pos.y)) != 0) {
    //                //danger.push_back((*it2));
    //                bool added = false;
    //                for (int i = 0; i < danger.size(); i++) {
    //                    if (Distance2D(danger[i].center, pos) < ENEMY_SQUAD_RADIUS) {
    //                        danger[i].add(*it2, this);
    //                        added = true;
    //                        break;
    //                    }
    //                }
    //                if (added == false) {
    //                    danger.emplace_back();
    //                    danger.back().add(*it2, this);
    //                }
    //            }
    //        }
    //    }
    //    return danger;
    //}

    Color pathingMapToColor(int8_t map) {
        switch (map) {
        case (0):
            return {255,255,255};
        case (127):
            return { 0,0,0 };
        case (64):
            return { 40,205,60 };
        case (101):
            return { 205,40,60 };
        case (102):
            return { 40,60,205 };
        default:
            return { 200, 180, (uint8_t)map };
        }
    }

    //! Called when a game is started or restarted.
    virtual void OnGameStart() final {

        profilerPrint = false;
        profilerThreshold = 10;
        last_time = clock();

        int mapWidth = Observation()->GetGameInfo().width;
        int mapHeight = Observation()->GetGameInfo().height;

        

        SpacialHash::grid = new map2d<UnitWrappers>(mapWidth, mapHeight, true);
        SpacialHash::gridEnemy = new map2d<UnitWrappers>(mapWidth, mapHeight, true);
        SpacialHash::gridModify = new map2d<int8_t>(mapWidth, mapHeight, true);

        Aux::buildingBlocked = new map2d<int8_t>(mapWidth, mapHeight, true);
        Aux::pathingMap = new map2d<int8_t>(mapWidth, mapHeight, true);

        Aux::loadPathables(this);

        Aux::influenceMap = new map2d<int8_t>(mapWidth, mapHeight, true);
        Aux::influenceMapEnemy = new map2d<int8_t>(mapWidth, mapHeight, true);

        Aux::visionMap = new map2d<int16_t>(mapWidth, mapHeight, true);

        path_zhang_suen = new map2d<int8_t>(mapWidth, mapHeight, true);

        PrimordialStar::blobGrid = new map2d<int8_t>(mapWidth, mapHeight, true);
        PrimordialStar::minDistanceGrid = new map2d<MinDistanceNode>(mapWidth, mapHeight, true);
        PrimordialStar::maxDistanceGrid = new map2d<DistanceNode>(mapWidth, mapHeight, true);
        
        UnitManager::enemyDamageNetReal = new map2d<int8_t>(mapWidth * UnitManager::damageNetPrecision, mapHeight * UnitManager::damageNetPrecision, true);
        UnitManager::enemyDamageNet = new map2d<DamageLocation>(mapWidth * UnitManager::damageNetPrecision, mapHeight * UnitManager::damageNetPrecision, true);
        UnitManager::enemyDamageNetModify = new map2d<int8_t>(mapWidth * UnitManager::damageNetPrecision, mapHeight * UnitManager::damageNetPrecision, true);
        UnitManager::enemyDamageNetTemp = new map2d<float>(mapWidth * UnitManager::damageNetPrecision, mapHeight * UnitManager::damageNetPrecision, true);

        SpacialHash::initGrid(this);
        SpacialHash::initGridEnemy(this);

        for (int i = 0; i < path_zhang_suen->width(); i++) {
            for (int j = 0; j < path_zhang_suen->height(); j++) {
                imRef(path_zhang_suen, i, j) = Observation()->IsPathable(Point2D{i+0.5F,j+0.5F});
            }
        }

        std::string words[] = {
            "wrath", "beans", "power", "soul", "rage", "fire",
            "energy", "love", "fury", "passion", "spirit",
            "micro", "strength", "presence", "heart", "truth",
            "macro", "pulse", "force", "breath", "light",
            "essence", "grace", "wisdom", "cheese", "embrace",
            "resolve", "might", "aura", "unity"
        };
        int numWords = sizeof(words) / sizeof(words[0]);

        printf("Playing on %s\n", Observation()->GetGameInfo().map_name.c_str());
        Actions()->SendChat("My Origin? Its Primordial, baby! (protoss)");
        Actions()->SendChat(strprintf("Feel the %s of my Protoss (pheart)", words[std::rand() % numWords].c_str()));

        Units units = Observation()->GetUnits(sc2::Unit::Alliance::Neutral);
        for (char c : Observation()->GetGameInfo().map_name) {
            if (c != ' ') {
                fileName += c;
            }
        }

        for (const Unit* unit : units) {
            //if (Aux::isMineral(*unit)) {
            //    //imRef(path_zhang_suen, int(unit->pos.x + 0.5F), int(unit->pos.y)) = 1;
            //    //imRef(path_zhang_suen, int(unit->pos.x - 0.5F), int(unit->pos.y)) = 1;
            //    Aux::loadUnitPlacement(path_zhang_suen, unit->pos, unit->unit_type, 2);

            //    //imRef(Aux::buildingBlocked, int(unit->pos.x + 0.5F), int(unit->pos.y)) = 1;
            //    //imRef(Aux::buildingBlocked, int(unit->pos.x - 0.5F), int(unit->pos.y)) = 1;
            //    Aux::loadUnitPlacement(Aux::buildingBlocked, unit->pos, unit->unit_type, 2);

            //    //imRef(Aux::pathingMap, int(unit->pos.x + 0.5F), int(unit->pos.y)) = 1;
            //    //imRef(Aux::pathingMap, int(unit->pos.x - 0.5F), int(unit->pos.y)) = 1;
            //    Aux::loadUnitPlacement(Aux::pathingMap, unit->pos, unit->unit_type, 2);
            //} else if (Aux::isVespene(*unit)) {
            //    for (int i = -1; i <= 1; i++) {
            //        for (int j = -1; j <= 1; j++) {
            //            imRef(path_zhang_suen, int(unit->pos.x + i), int(unit->pos.y + j)) = 1;
            //            imRef(Aux::buildingBlocked, int(unit->pos.x + i), int(unit->pos.y + j)) = 1;
            //            imRef(Aux::pathingMap, int(unit->pos.x + i), int(unit->pos.y + j)) = 1;
            //        }
            //    }
            //} else {
            //    printf("NEUTRAL:%s\n", UnitTypeToName(unit->unit_type));
            //    DebugText(this, UnitTypeToName(unit->unit_type), unit->pos + Point3D{ 0,0,1 });
            //    DebugSphere(this, unit->pos, unit->radius);
            //    Aux::addPlacement(unit->pos, unit->unit_type);
            //}
            Aux::loadUnitPlacement(path_zhang_suen, unit->pos, unit->unit_type, 1);

            int8_t pathingNum = 64;
            if (Aux::isVespeneType(unit->unit_type)) {
                pathingNum = 127;
            }


            Aux::loadUnitPlacement(Aux::buildingBlocked, unit->pos, unit->unit_type, pathingNum);

            if (unit->unit_type != UNIT_TYPEID::NEUTRAL_UNBUILDABLEBRICKSDESTRUCTIBLE &&
                unit->unit_type != UNIT_TYPEID::NEUTRAL_UNBUILDABLEPLATESDESTRUCTIBLE &&
                unit->unit_type != UNIT_TYPEID::UNBUILDABLEROCKSDESTRUCTIBLE) {
                Aux::loadUnitPlacement(Aux::pathingMap, unit->pos, unit->unit_type, pathingNum);
            }
        }
        timeus startGenerateGrid = std::chrono::steady_clock::now();

        float maximalMin = PrimordialStar::generateMinDistanceGrid(this);

        printf("Generating Min Distance Grid took %.3fms\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startGenerateGrid).count() / 1000.0);

        startGenerateGrid = std::chrono::steady_clock::now();

        float maximalMax = PrimordialStar::generateMaxDistanceGrid(this);

        printf("Generating Max Distance Grid took %.3fms\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startGenerateGrid).count() / 1000.0);

        //saveBitmap("black.bmp", mapWidth, mapHeight, [](int i, int j) {return 0;}, [](int i, int j) {return 0;}, [](int i, int j) {return 0;});
        saveBitmap("pathable.bmp", mapWidth, mapHeight, [](int i, int j) {return imRef(Aux::pathingMap, i, j);}, [](int i, int j) {return imRef(Aux::pathingMap, i, j);}, [](int i, int j) {return imRef(Aux::pathingMap, i, j);});
        //saveBitmap("placable.bmp", mapWidth, mapHeight, [](int i, int j) {return imRef(Aux::pathingMap, i, j) * 255;}, [](int i, int j) {return imRef(Aux::pathingMap, i, j) * 255;}, [](int i, int j) {return imRef(Aux::pathingMap, i, j) * 255;});

        saveBitmap(fileName + "_distanceMaxPP.bmp", mapWidth, mapHeight, 
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distancePP / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distancePP / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distancePP / maximalMax * 255.0);});

        saveBitmap(fileName + "_distanceMaxNP.bmp", mapWidth, mapHeight,
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distanceNP / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distanceNP / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distanceNP / maximalMax * 255.0);});

        saveBitmap(fileName + "_distanceMaxNN.bmp", mapWidth, mapHeight,
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distanceNN / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distanceNN / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distanceNN / maximalMax * 255.0);});

        saveBitmap(fileName + "_distanceMaxPN.bmp", mapWidth, mapHeight,
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distancePN / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distancePN / maximalMax * 255.0);},
            [maximalMax](int i, int j) {return uint8_t(imRef(PrimordialStar::maxDistanceGrid, i, j).distancePN / maximalMax * 255.0);});

        saveBitmap(fileName + "_distanceMin.bmp", mapWidth, mapHeight,
            [maximalMin](int i, int j) {return uint8_t(imRef(PrimordialStar::minDistanceGrid, i, j).distance / maximalMin * 255.0);},
            [maximalMin](int i, int j) {return uint8_t(imRef(PrimordialStar::minDistanceGrid, i, j).distance / maximalMin * 255.0);},
            [maximalMin](int i, int j) {return uint8_t(imRef(PrimordialStar::minDistanceGrid, i, j).distance / maximalMin * 255.0);});

        //saveBitmap(fileName + "_master.bmp", mapWidth, mapHeight,
        //    [this](int i, int j) {return (Aux::checkPathable(i, j, this) ? 255 : 0);},
        //    [this](int i, int j) {return (Aux::checkPlacable(i, j, this) ? 255 : 0);},
        //    [this](int i, int j) {return (Aux::checkPathable(i, j, this) || Aux::checkPlacable(i, j, this) ? 255 : 0);});

        saveBitmap(fileName + "_master.bmp", mapWidth, mapHeight,
            [this](int i, int j) {return pathingMapToColor(Aux::getPathable(i, j, this)).r;},
            [this](int i, int j) {return pathingMapToColor(Aux::getPathable(i, j, this)).g;},
            [this](int i, int j) {return pathingMapToColor(Aux::getPathable(i, j, this)).b;});

        //grid(false);
        displayMaxDistanceGrid(maximalMax, false);

        timeus startPathNoding = std::chrono::steady_clock::now();

        PrimordialStar::generatePathNodes(this);

        printf("Generating %d PathNodes took %.3fms ", PrimordialStar::basePathNodes.size(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startPathNoding).count() / 1000.0);
        
        int numConnections = 0;
        
        for (PrimordialStar::PathNode* node : PrimordialStar::basePathNodes) {
            numConnections += node->connected.size();
        }
        
        printf("with %d connections\n", numConnections);
        //original PrimordialStar on Abyssal Reef is 229.322 and 202.749 and 220.070 with 35932,35928
        //changed something (wall nocheck maybe?) and it changed it to 166ms
        //after adding the wall preprocessing it reduces to 141ms + 108ms
        //adding wall preprocessing with squared not root makes it 58ms + 39ms



        for (int i = 0; i < PrimordialStar::basePathNodes.size(); i++) {
            PrimordialStar::PathNode* node = PrimordialStar::basePathNodes[i];
            if (Distance2D(node->rawPos(), Observation()->GetCameraPos()) > 300) {
                continue;
            }
            DebugSphere(this, P3D(node->rawPos()), 0.5, {250,50,100});
            for (int c = 0; c < node->connected.size(); c++) {
                PrimordialStar::PathNode* node2 = PrimordialStar::basePathNodes[node->connected[c]];
                DebugLine(this, P3D(node->rawPos()) + Point3D{ 0,0,1 }, P3D(node2->rawPos()) + Point3D{ 0,0,1 }, Colors::Blue);
            }
        }
        pathVerification();

        #if MICRO_TEST == 0
            for (int i = -4; i <= 4; i++) {
                for (int j = -4; j <= 4; j++) {
                    imRef(path_zhang_suen, int(Observation()->GetStartLocation().x + i),
                          int(Observation()->GetStartLocation().y + j)) = 1;
                }
            }
        #endif

        zhangSuenThinning(path_zhang_suen, this);
        //thinning_BST(path_zhang_suen, this);

        gridmap = Grid{Observation()->GetGameInfo().width, Observation()->GetGameInfo().height};

        //const ObservationInterface* observe = Observation();
        //PathFinder pf(observe->GetGameInfo().width, observe->GetGameInfo().height);
        //cout << pf.findPath(observe->GetGameInfo().start_locations[0], observe->GetGameInfo().enemy_start_locations[0], this) << endl;
        //unordered_set<Location> walls{{5, 0}, {5, 1}, {2, 2}, {5, 2}, {2, 3}, {5, 3}, {2, 4}, {5, 4},
        //                              {2, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {2, 6}, {2, 7}};

        //Point2DI start{142, 45};
        //Point2DI goal{130, 60};

        //auto came_from = jps(gridmap, start, goal, Tool::euclidean, this);
        //auto pat = Tool::reconstruct_path(start, goal, came_from);
        //path = fullPath(pat);
        /*for (auto loc : path) {
            printf("%d,%d\n", loc.x, loc.y);
        }*/
        //Tool::draw_grid(this, map, {}, {}, path, came_from, start, goal);
        initializeStartings();
        initializeExpansions();

        Strategem::initStrategies();

        Point2D middle{Observation()->GetGameInfo().width / 2.0F, Observation()->GetGameInfo().height / 2.0F};

        vector<Point2DI> possiblePoints;

        #if MICRO_TEST == 0
        for (int i = -12; i <= 12; i++) {
            for (int j = -12; j <= 12; j++) {
                float d = Distance2D(Point2D{i + 0.5F, j + 0.5F}, {0,0});
                if (imRef(path_zhang_suen, int(Aux::rankedExpansions[0].x) + i, int(Aux::rankedExpansions[0].y) + j) == 1 &&
                    d > 10 && d < 12) {
                    possiblePoints.push_back({i + (int)Aux::rankedExpansions[0].x, j + (int)Aux::rankedExpansions[0].y});
                }
            }
        }
        #endif

        float min = -1;

        for (int i = 0; i < possiblePoints.size(); i++) {
            auto came_from =
                jps(gridmap, middle,
                                 {int(possiblePoints[i].x), int(possiblePoints[i].y)}, Tool::euclidean, this);
            auto pathToExpansion = Tool::reconstruct_path(Location(middle),
                                       {int(possiblePoints[i].x), int(possiblePoints[i].y)}, came_from);

            double length = fullDist(pathToExpansion);
            if (min == -1 || min > length) {
                min = length;
                rally_point = P2D(possiblePoints[i]) + Point2D{0.5F, 0.5F};
            }
        }

        squads.emplace_back();
        #if MICRO_TEST
            squads[0].attack(START_OP);
        #else
            squads[0].attack(rally_point);
        #endif

        //Debug()->DebugShowMap();
        //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_STALKER, middle, 2, 9);
        //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_VOIDRAY, Observation()->GetGameInfo().enemy_start_locations[0], 2, 9);

        #if MICRO_TEST_2

            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_STALKER, middle, 1, 9);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_IMMORTAL, middle, 1, 2);
            ////Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_OBSERVER, middle, 1, 12);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_COLOSSUS, middle, 1, 2);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::ZERG_ZERGLING, Observation()->GetGameInfo().enemy_start_locations[0], 2, 16);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_ADEPT, Observation()->GetGameInfo().enemy_start_locations[0], 2, 6);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::TERRAN_MARINE, Observation()->GetGameInfo().enemy_start_locations[0], 2, 14);

            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_OBSERVER, middle, 1, 1);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_STALKER, middle, 1, 1);
            //Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_ADEPT, Observation()->GetGameInfo().enemy_start_locations[0], 2, 2);
        #endif
    }

    //! Called when a game has ended.
    virtual void OnGameEnd() {

    }

    //! Called when a Unit has been created by the player.
    //!< \param unit The created unit.
    virtual void OnUnitCreated(const Unit* unit) {
        if (unit->tag == NullTag) {
            return;
        }
        if (unit->unit_type == UNIT_TYPEID::PROTOSS_PROBE) {
            Probe *u = new Probe(unit);
            u->execute(this);
        } else if (!unit->is_building) {
            if (unit->unit_type == UNIT_TYPEID::PROTOSS_STALKER) {
                new Stalker(unit);
            } else if (unit->unit_type == UNIT_TYPEID::PROTOSS_OBSERVER) {
                new ObserverEye(unit);
            } else if (unit->unit_type == UNIT_TYPEID::PROTOSS_ZEALOT) {
                new Zealot(unit);
            } else {
                new ArmyUnit(unit);
            }
        } else if (unit->unit_type == UNIT_TYPEID::PROTOSS_NEXUS) {
            Nexus *n = new Nexus(unit);
            //n->initVesp(this);
        }
        else{
            UnitWrapper* u = new UnitWrapper(unit);
            u->execute(this);
        }
        //else
        if (unit->is_building) {
            /*Aux::addPlacement(unit->pos, unit->unit_type);*/

            Aux::loadUnitPlacement(Aux::buildingBlocked, unit->pos, unit->unit_type, 101);
            Aux::loadUnitPlacement(Aux::pathingMap, unit->pos, unit->unit_type, 101);

            UnitTypes allData = Observation()->GetUnitTypeData();
            UnitTypeData unit_stats = allData.at(static_cast<uint32_t>(unit->unit_type));
            GameInfo game_info = Observation()->GetGameInfo();

            for (int i = std::max(0, int(unit->pos.x - unit_stats.sight_range) - 2);
                 i < std::min(game_info.width, int(unit->pos.x + unit_stats.sight_range) + 2); i++) {
                for (int j = std::max(0, int(unit->pos.y - unit_stats.sight_range) - 2);
                     j < std::min(game_info.height, int(unit->pos.y + unit_stats.sight_range) + 2); j++) {
                    if (Distance2D(Point2D{i + 0.5F, j + 0.5F}, unit->pos) < unit_stats.sight_range) {
                        imRef(Aux::influenceMap, i, j) += 1;
                    }
                }
            }
        }
        //else {
        //    ArmyUnit* u = new ArmyUnit(unit);
        //}
    }

    //! Called whenever one of the player's units has been destroyed.
    //!< \param unit The destroyed unit.
    virtual void OnUnitDestroyed(const Unit* unit) {
        if (unit->alliance == Unit::Alliance::Self) {
            UnitWrapper* u = UnitManager::find(unit->unit_type, unit->tag);
            delete u;
            //if (unit->is_building) {
            //    //Aux::removePlacement(unit->pos, unit->unit_type);
            //    //Aux::loadUnitPlacement(map2d<int8_t>*map, Point2D pos, UnitTypeID unit_type, int8_t value) {
            //    //UnitTypes allData = Observation()->GetUnitTypeData();
            //    //UnitTypeData unit_stats = allData.at(static_cast<uint32_t>(unit->unit_type));
            //    //GameInfo game_info = Observation()->GetGameInfo();

            //    //for (int i = std::max(0, int(unit->pos.x - unit_stats.sight_range) - 2);
            //    //     i < std::min(game_info.width, int(unit->pos.x + unit_stats.sight_range) + 2); i++) {
            //    //    for (int j = std::max(0, int(unit->pos.y - unit_stats.sight_range) - 2);
            //    //         j < std::min(game_info.height, int(unit->pos.y + unit_stats.sight_range) + 2); j++) {
            //    //        if (Distance2D(Point2D{i + 0.5F, j + 0.5F}, unit->pos) < unit_stats.sight_range) {
            //    //            imRef(Aux::influenceMap, i, j) -= 1;
            //    //        }
            //    //    }
            //    //}
            //}
        } else if(unit->alliance == Unit::Alliance::Enemy) {
            UnitWrapper* u = UnitManager::findEnemy(unit->unit_type, unit->tag);
            delete u;
            printf("HOSTILE %s %Ix DEAD\n", UnitTypeToName(unit->unit_type), unit->tag);
            if (unit->is_building) {
                //Aux::removePlacement(unit->pos, unit->unit_type);
                UnitTypes allData = Observation()->GetUnitTypeData();
                UnitTypeData unit_stats = allData.at(static_cast<uint32_t>(unit->unit_type));
                GameInfo game_info = Observation()->GetGameInfo();

                for (int i = std::max(0, int(unit->pos.x - unit_stats.sight_range) - 2);
                     i < std::min(game_info.width, int(unit->pos.x + unit_stats.sight_range) + 2); i++) {
                    for (int j = std::max(0, int(unit->pos.y - unit_stats.sight_range) - 2);
                         j < std::min(game_info.height, int(unit->pos.y + unit_stats.sight_range) + 2); j++) {
                        if (Distance2D(Point2D{i + 0.5F, j + 0.5F}, unit->pos) < unit_stats.sight_range) {
                            imRef(Aux::influenceMapEnemy, i, j) -= 1;
                        }
                    }
                }
            }
        }
        else {
            UnitWrapper* u = UnitManager::findNeutral(unit->unit_type, unit->tag);
            delete u;
            printf("NEUTRAL %s %Ix DEAD %p\n", UnitTypeToName(unit->unit_type), unit->tag, UnitManager::findNeutral(unit->unit_type, unit->tag));
        }
        if (unit->is_building) {
            Aux::loadUnitPlacement(Aux::buildingBlocked, unit->pos, unit->unit_type, 0);
            Aux::loadUnitPlacement(Aux::pathingMap, unit->pos, unit->unit_type, 0);
        }
    }

    //! Called when a unit becomes idle, this will only occur as an event so will only be called when the unit becomes
    //! idle and not a second time. Being idle is defined by having orders in the previous step and not currently having
    //! orders or if it did not exist in the previous step and now does, a unit being created, for instance, will call
    //! both OnUnitCreated and OnUnitIdle if it does not have a rally set.
    //!< \param unit The idle unit.
    virtual void OnUnitIdle(const Unit* unit) {
        //UnitManager::find(unit->unit_type, unit->tag)->execute(this);
    }

    void loadVisionMap(Agent* agent) {
        int mapWidth = agent->Observation()->GetGameInfo().width;
        int mapHeight = agent->Observation()->GetGameInfo().height;
        for (int i = 0; i < mapWidth; i++) {
            for (int j = 0; j < mapHeight; j++) {
                if (imRef(Aux::visionMap, i, j) > 1) {
                    imRef(Aux::visionMap, i, j) -= 1;
                }
            }
        }

        for (auto it = UnitManager::units.begin(); it != UnitManager::units.end(); it++) {
            auto all = it->second;
            UnitTypeData unit_stats = Aux::getStats(it->first, agent);
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                Point2D pos = (*it2)->pos(agent);

                for (int i = std::max(0, int(pos.x - unit_stats.sight_range) - 2);
                    i < std::min(mapWidth, int(pos.x + unit_stats.sight_range) + 2); i++) {
                    for (int j = std::max(0, int(pos.y - unit_stats.sight_range) - 2);
                        j < std::min(mapHeight, int(pos.y + unit_stats.sight_range) + 2); j++) {
                        if (Distance2D(Point2D{ i + 0.5F, j + 0.5F }, pos) < unit_stats.sight_range) {
                            imRef(Aux::visionMap, i, j) = Aux::visionMax;
                        }
                    }
                }
            }
        }


    }

    //! In non realtime games this function gets called after each step as indicated by step size.
    //! In realtime this function gets called as often as possible after request/responses are received from the game
    //! gathering observation state.
    virtual void OnStep() final {
        //printf(" ");
        Profiler onStepProfiler("onStep");
        //onStepProfiler.disable();

        Macro::execute(this);

        onStepProfiler.midLog("MacroExecute");

        //Probe::loadAbilities(this);
        UnitWrapper::loadAbilities(this);

        onStepProfiler.midLog("LoadAbilities");

        //UnitWrapper::loadAbilitiesEnemy(this);
        Aux::loadEffects(this);

        onStepProfiler.midLog("LoadEffects");

        auto probes = UnitManager::get(UNIT_TYPEID::PROTOSS_PROBE);
        for (auto it = probes.begin(); it != probes.end(); it++) {
            Probe* probe = ((Probe*)*it);
            probe->execute(this);
        }

        onStepProfiler.midLog("ProbeExecute");

        SpacialHash::updateGrid(this);

        onStepProfiler.midLog("SpacialFriendly");

        SpacialHash::updateGridEnemy(this);

        onStepProfiler.midLog("SpacialEnemy");

        loadVisionMap(this);

        onStepProfiler.midLog("LoadVisionMap");

        //manageArmy();
        ArmyControl::step(this, rally_point);

        //for (UnitWrapper* wrap : UnitManager::units[UNIT_TYPEID::PROTOSS_STALKER]) {
        //    float theta = ((float)std::rand()) * 2 * M_PI / RAND_MAX;
        //    float magnitude = 8;//((float)std::rand()) * 8 / RAND_MAX;

        //    Point2D displace{ cos(theta) * magnitude, sin(theta) * magnitude };

        //    Point3D upos = wrap->pos3D(this);
        //    Point3D blinkPos{ upos.x + displace.x, upos.y + displace.y, upos.z };
        //    DebugLine(this, upos, blinkPos, { 240, 73, 250 });
        //    printf("TELEPORT STALKER {%.1f, %.1f}\n", displace.x, displace.y);
        //    Actions()->UnitCommand(wrap->self, ABILITY_ID::EFFECT_BLINK_STALKER, blinkPos);
        //}

        onStepProfiler.midLog("ArmyControl");

        string s = "";
        for (int i = 0; i < squads.size(); i ++) {
            squads[i].execute(this);
            s += strprintf("SQUAD%d %s %.1f,%.1f int[%.1f,%.1f]:\n", i, SquadModeToString(squads[i].mode),
                           squads[i].location.x, squads[i].location.y, squads[i].intermediateLoc.x,
                           squads[i].intermediateLoc.y);
            Point2D c = squads[i].coreCenter(this);
            //Point2D cg = squads[i].center(this);
            s += strprintf("C:%.1f,%.1f S:%d\n", c.x, c.y, squads[i].army.size());
            for (int a = 0; a < squads[i].army.size(); a++) {
                ArmyUnit* unit = (ArmyUnit*)squads[i].army[a];
                unit->execute(this);
                Point3D pos = unit->pos3D(this);
                s += strprintf("%s %.1fs %Ix %c {%.1f,%.1f}\n", UnitTypeToName(unit->type),
                    unit->get(this)->weapon_cooldown,
                               ((ArmyUnit*)(unit))->targetWrap, squads[i].squadStates[unit->self], unit->posTarget.x, unit->posTarget.y);
                DebugText(this,strprintf("%c:%c", squads[i].squadStates[unit->self], squads[i].subSquadStates[unit->self]),
                                      pos,
                                      Color(210, 55, 55), 8);
                
                DebugLine(this, pos + Point3D{ 0,0,1 }, P3D(unit->posTarget) + Point3D{0,0,1}, {255, 150, 150});
                DebugLine(this, pos + Point3D{ 0,0,1 }, P3D(unit->escapeLoc) + Point3D{ 0,0,1 }, { 10, 150, 255 });
            }
            s += '\n';
            DebugSphere(this,P3D(squads[i].coreCenter(this)), squads[i].armyballRadius());
            DebugSphere(this,P3D(squads[i].intermediateLoc), 0.5, {30,230, 210});
        }
        DebugText(this, s, Point2D(0.71, 0.11), Color(1, 42, 212), 8);

        onStepProfiler.midLog("SquadExecute");
        #if MICRO_TEST == 0
        Strategem::Strategy strat = Strategem::shit_stalker_colossus;
            if (Observation()->GetGameLoop() == 2) {
                for (int i = 0; i < 32; i++) {
                    Macro::addProbe();
                    printf("probeGI:%d\n", MacroAction::globalIndex);
                }
                for (int i = 0; i < strat.size(); i++) {
                    Macro::addAction(strat[i]);
                    printf("GI:%d\n", MacroAction::globalIndex);
                }
            }
            //if (Observation()->GetGameLoop() == 2) {
            //    //HOME BASE MINERALS
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();

            //    //HOME BASE VESPENE 1
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();

            //    //HOME BASE VESPENE 2
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();

            //    //NATURAL MINERALS
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();//
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();//
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();//
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();//

            //    //NATURAL GAS 1
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();

            //    //NATURAL GAS 2
            //    Macro::addProbe();
            //    Macro::addProbe();
            //    Macro::addProbe();

            //    //Macro::addBuilding(ABILITY_ID::BUILD_PYLON, P2D(staging_location));
            //    //Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, P2D(staging_location) - Point2D{-2.5, 0.5});
            //    //Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR,
            //    //                   Observation()->GetUnit(UnitManager::getVespene()[0]->self)->pos);
            //    //Macro::addBuilding(ABILITY_ID::BUILD_NEXUS, P2D(rankedExpansions[0]));
            //    //Macro::addBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, P2D(staging_location) - Point2D{2.5, -0.5});
            //    //Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR,
            //    //                   Observation()->GetUnit(UnitManager::getVespene()[1]->self)->pos);
            //    //Macro::addBuilding(ABILITY_ID::BUILD_PYLON, P2D(staging_location) - Point2D{0.5, -2.5});
            //    //Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);
            //    //Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);
            //    //Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);
            //    //Macro::addBuilding(ABILITY_ID::BUILD_STARGATE, P2D(staging_location) - Point2D{-0.5, 3});

            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location));

            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1});

            //    Macro::addBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc);

            //    Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1});

            //    //Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR,
            //    //                   Observation()->GetUnit(UnitManager::getVespene()[0]->self)->pos);

            //    Macro::addBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1, -1});

            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);

            //    Macro::addAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSBAY, ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE);

            //    Macro::addBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0]));

            //    Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 });

            //    //Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR,
            //    //                   Observation()->GetUnit(UnitManager::getVespene()[1]->self)->pos);

            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1});
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);

            //    Macro::addBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1, -1});
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL);

            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, {-1, -1});

            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER);


            //    //GameRequestPtr request = Control()->Proto().MakeRequest();
            //    //SC2APIProtocol::RequestQuickSave request_join_game = request->quick_save();
            //} 
            //else if (Observation()->GetGameLoop() == int(3.00 * 60 * 22.4)) {
            //    //GameRequestPtr request = Control()->Proto().MakeRequest();
            //    //SC2APIProtocol::RequestQuickLoad request_join_game = request->quick_load();

            //    Macro::addAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_BLINK);

            //    Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 });

            //    Macro::addBuilding(ABILITY_ID::BUILD_ROBOTICSBAY, {-1, -1});

            //    Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 });

            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL);

            //    //Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR,
            //    //                   Observation()->GetUnit(UnitManager::getVespene()[2]->self)->pos);
            //    //Macro::addBuilding(ABILITY_ID::BUILD_ASSIMILATOR,
            //    //                   Observation()->GetUnit(UnitManager::getVespene()[3]->self)->pos);
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1});
            //    Macro::addBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1});

            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT);
            //
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM);
            //}
        #endif

        if (Observation()->GetGameLoop() % 200 == 0) { //1344
            int mapWidth = Observation()->GetGameInfo().width;
            int mapHeight = Observation()->GetGameInfo().height;
            time_t now = time(0);
            char* dt = ctime(&now);
            //string time = strprintf("_%s_%d_", dt, Observation()->GetGameLoop());
            string time = strprintf("_%d_", Observation()->GetGameLoop());
            saveBitmap(fileName + time +"visionMap.bmp", mapWidth, mapHeight,
                [this](int i, int j) {return (uint8_t)((float)imRef(Aux::visionMap, i, j) * 255 / Aux::visionMax);},
                [this](int i, int j) {return (uint8_t)((float)imRef(Aux::visionMap, i, j) * 255 / Aux::visionMax);},
                [this](int i, int j) {return (uint8_t)((float)imRef(Aux::visionMap, i, j) * 255 / Aux::visionMax);});
        }

        if (0) {
            //((ObservationImp*)Observation())->game_info_cached_ = false;
            int mapWidth = Observation()->GetGameInfo().width;
            int mapHeight = Observation()->GetGameInfo().height;
            if (Observation()->GetGameLoop() == 2) {
                saveBitmap(fileName + "_pre_pathable.bmp", mapWidth, mapHeight, 
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);}, 
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);}, 
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);});
                Debug()->DebugShowMap();
            }
            else if (Observation()->GetGameLoop() == 82) {
                saveBitmap(fileName + "_mid_pathable.bmp", mapWidth, mapHeight,
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);},
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);},
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);});
                Units units = Observation()->GetUnits(sc2::Unit::Alliance::Neutral);
                for (const Unit* unit : units) {
                    Debug()->DebugKillUnit(unit);
                }
            }
            else if (Observation()->GetGameLoop() == 162) {
                saveBitmap(fileName + "_end_pathable.bmp", mapWidth, mapHeight,
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);},
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);},
                    [this](int i, int j) {return (Observation()->IsPathable({ float(i), float(j) }) ? 255 : 0);});
            }
        }

        onStepProfiler.midLog("BuildOrderSetup");

        int spareMinerals = Observation()->GetMinerals();
        int spareVespene = Observation()->GetVespene();

        for (auto it = Macro::actions.begin(); it != Macro::actions.end(); it++) {
            auto all = it->second;
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                Cost c = it2->cost(this);
                spareMinerals -= c.minerals;
                spareVespene -= c.vespene;
            }
        }
        if (spareMinerals > 150 && spareVespene > 75) {
            //if (squads[0].has(UNIT_TYPEID::PROTOSS_WARPPRISM)) {
            //    Macro::addAction(UNIT_TYPEID::PROTOSS_WARPPRISM, ABILITY_ID::MORPH_WARPPRISMPHASINGMODE);
            //}
            Macro::addAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER, squads[0].coreCenter(this));
        }

        onStepProfiler.midLog("StalkerCreation");

        clock_t new_time = clock();
        int dt = (new_time - last_time);
        last_time = new_time;
        DebugText(this,strprintf("%d", dt), Point2D(0.10, 0.10), Color(100, 190, 215), 16);

        onStepProfiler.midLog("DT");

        //for (int i = 0; i < UnitManager::enemyDamageNet->width(); i++) {
        //    for (int j = 0; j < UnitManager::enemyDamageNet->height(); j++) {
        //        imRef(UnitManager::enemyDamageNet, i, j) = { 0.0,0.0,0.0,0.0,0.0,0.0 };
        //    }
        //}

        UnitManager::enemyDamageNetReal->clear();
        //UnitManager::enemyDamageNetModify->clear();
        //UnitManager::enemyDamageNetTemp->clear();

        onStepProfiler.midLog("DamageGridReset");

        loadEnemyHealth();

        onStepProfiler.midLog("LoadHealth");

        if (0) {
            //Profiler profiler("BigCircle");
            DebugSphere(this,P3D(Observation()->GetCameraPos()), 3);
            UnitManager::setEnemyDamageRadius3(Observation()->GetCameraPos(), 3, { Damage(200, 0,0,0,0,0,0), Damage(0, 0,0,0,0,0,0) }, this);
        }

        if (0) {
            //Profiler profiler("JPS");
            DebugSphere(this,P3D(Observation()->GetCameraPos()), 1);
            Point2DI cam = Observation()->GetCameraPos();
            Point2DI end = { 0,0 };
            auto came_from = jps(gridmap, rally_point, cam, Tool::euclidean, this, 1, &end);
            vector<Location> pat = Tool::reconstruct_path(Location(rally_point), end, came_from);
            if (pat.size() != 0) {
                vector<Point2DI> path = fullPath(pat);
                for (Point2DI p : path) {
                    DebugBox(this,P3D(P2D(p)) + Point3D{ 0,0,-1 }, P3D(P2D(p)) + Point3D{ 1,1,0.3 }, { 180,30,190 });
                }
                for (Point2DI p : pat) {
                    DebugBox(this,P3D(P2D(p)) + Point3D{ 0,0,-1 }, P3D(P2D(p)) + Point3D{ 1,1,0.3 }, { 40,30,250 });
                }
            }
        }
        onStepProfiler.midLog("OS-JPS");
        //DebugSphere(this,P3D(Observation()->GetCameraPos()), 0.6);
        //UnitManager::setEnemyDamageRadius(Observation()->GetCameraPos(), 0.6, {200,0,0,0,0,0}, this);
        //UnitManager::setEnemyDamageRadius(Observation()->GetCameraPos(), 0.6, {0,0,0,200,0,0 }, this);

        for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                std::vector<Weapon> weapons = Aux::getStats((*it2)->type, this).weapons;
                //TODO:
                // add abilities, oracle, baneling
                // add psi storm (more damage the more time it has left, prioritzed more since its constant dmag)
                // add helion line, lurker line
                for (Weapon w : weapons) {

                    //TODO: REPLACE WITH WEAPON TO DAMAGELOCATION FUNCTION
                    DamageLocation d;
                    Damage damage;
                    for (DamageBonus bonus : w.damage_bonus) {
                        if (bonus.attribute == Attribute::Light) {
                            damage.light += bonus.bonus / w.speed * 100;
                        }
                        else if (bonus.attribute == Attribute::Armored) {
                            damage.armored += bonus.bonus / w.speed * 100;
                        }
                        else if (bonus.attribute == Attribute::Biological) {
                            damage.biological += bonus.bonus / w.speed * 100;
                        }
                        else if (bonus.attribute == Attribute::Mechanical) {
                            damage.mechanical += bonus.bonus / w.speed * 100;
                        }
                        else if (bonus.attribute == Attribute::Massive) {
                            damage.massive += bonus.bonus / w.speed * 100;
                        }
                        else if (bonus.attribute == Attribute::Psionic) {
                            damage.psionic += bonus.bonus / w.speed * 100;
                        }
                        else {
                            printf("ATTRIBUTE NOT PROGRAMMED %d\n", bonus.attribute);
                        }
                    }
                    
                    if (w.type == Weapon::TargetType::Air || w.type == Weapon::TargetType::Any) {
                        d.air.normal = w.damage_ / w.speed * 100;
                        d.air += damage;
                    }
                    if (w.type == Weapon::TargetType::Ground || w.type == Weapon::TargetType::Any) {
                        d.ground.normal = w.damage_ / w.speed * 100;
                        d.ground += damage;
                    }
                    UnitManager::setEnemyDamageRadius3((*it2)->pos(this), (*it2)->radius + w.range, d, this);
                    //printf("%s %Ix set %.1f,%.1f  %.1f,%.1f  %.1f,%.1f\n", UnitTypeToName((*it2)->type), (*it2)->self, d.ground, d.air, d.groundlight, d.airlight, d.groundarmored, d.airarmored);
                }
            }
        }
        //for (const Unit* u : Observation()->GetUnits(Unit::Alliance::Self)) {
        //    UnitManager::setEnemyDamageRadius(u->pos, u->radius, this);
        //    //printf("HEIGHT %.1f\n", u->pos.z);
        //}

        onStepProfiler.midLog("DamageGridUpdate");

        //grid();

        //displaySpacialHashGrid();

        SpacialHash::gridModify->clear();

        float testRadius = 0.375F;

        DebugSphere(this, P3D(Observation()->GetCameraPos()), testRadius);

        SpacialHash::fillGridModify(Observation()->GetCameraPos(), testRadius, this);

        displaySpacialHashGridTEST();

        //displayEnemyDamageGrid();

        //displayPrimordialStarNodes();

        //pylonBuildingLoc();
        //listUnitWraps();
        //listUnitWrapsNeutral();
        //listUnitWrapsEnemies();

        expansionsLoc();

        listMacroActions();

        probeLines();

        orderDisplay();

        //tagDisplay();
        
        neutralDisplay();

        buildingDisplay();

        //enemiesDisplay();

        onStepProfiler.midLog("Debug");

        string profilestr = "";
        static int max1 = 0;
        static int max2 = 0;
        static int max3 = 0;
        static int max4 = 0;
        for (auto itr = profilerMap.begin(); itr != profilerMap.end(); itr ++) {
            string name = itr->first;
            int strlen = name.size();
            max1 = max(strlen + 1, max1);
            for (int i = 0; i < (max1 - strlen); i++) {
                name += " ";
            }
            string dtstr = strprintf("AVG:%.3f", ((double)itr->second) / profilerCoumt[itr->first] / 1000.0);
            strlen = dtstr.size();
            max2 = max(strlen + 1, max2);
            for (int i = 0; i < (max2 - strlen); i++) {
                dtstr += " ";
            }
            string totstr = strprintf("TOT:%.3f/%d", itr->second / 1000.0, profilerCoumt[itr->first]);
            strlen = totstr.size();
            max3 = max(strlen + 1, max3);
            for (int i = 0; i < (max3 - strlen); i++) {
                totstr += " ";
            }
            string lateststr = strprintf("LAT:%.3f", profilerLast[itr->first].time()/1000.0);
            strlen = lateststr.size();
            max4 = max(strlen + 1, max4);
            for (int i = 0; i < (max4 - strlen); i++) {
                lateststr += " ";
            }
            profilestr += (name + lateststr + dtstr + totstr + "\n");
        }
        DebugText(this,profilestr, Point2D(0.61, 0.41), Color(1, 212, 41), 8);

        onStepProfiler.midLog("ProfilerLog");

        SendDebug(this);

        onStepProfiler.midLog("DebugSend");
    }

    //!  Called when a neutral unit is created. For example, mineral fields observed for the first time
    //!< \param unit The observed unit.
    virtual void OnNeutralUnitCreated(const Unit* unit) {
        if (Aux::isVespene(*unit)) {
            Vespene* u = new Vespene(unit);
            u->execute(this);
        } else {
            UnitWrapper* u = new UnitWrapper(unit);
            //u->execute(this);
        }
    }

    //! Called when an upgrade is finished, warp gate, ground weapons, baneling speed, etc.
    //!< \param upgrade The completed upgrade.
    virtual void OnUpgradeCompleted(UpgradeID upgradeID) {
    }

    //! Called when the unit in the previous step had a build progress less than 1.0 but is greater than or equal to 1.0
    //! in
    // !the current step.
    //!< \param unit The constructed unit.
    virtual void OnBuildingConstructionComplete(const Unit* unit) {
    }

    //! Called when the unit in the current observation has lower health or shields than in the previous observation.
    //!< \param unit The damaged unit.
    //!< \param health The change in health (damage is positive)
    //!< \param shields The change in shields (damage is positive)
    virtual void OnUnitDamaged(const Unit* unit, float health, float shields) {
        UnitWrapper* wrap = UnitManager::find(unit->unit_type, unit->tag);
        if (unit->alliance == Unit::Alliance::Self && wrap != nullptr) {
            wrap->executeDamaged(this, health, shields);
        }
    }

    //! Called when a nydus is placed.
    virtual void OnNydusDetected() {
    }

    //! Called when a nuclear launch is detected.
    virtual void OnNuclearLaunchDetected() {
    }

    //! Called when an enemy unit enters vision from out of fog of war.
    //!< \param unit The unit entering vision.
    virtual void OnUnitEnterVision(const Unit* unit) {
        UnitWrapper* u = new UnitWrapper(unit);
        u->execute(this);
        Aux::addPlacement(unit->pos, unit->unit_type);

        if (unit->is_building) {

            Aux::loadUnitPlacement(Aux::buildingBlocked, unit->pos, unit->unit_type, 102);
            Aux::loadUnitPlacement(Aux::pathingMap, unit->pos, unit->unit_type, 102);

            UnitTypes allData = Observation()->GetUnitTypeData();
            UnitTypeData unit_stats = allData.at(static_cast<uint32_t>(unit->unit_type));
            GameInfo game_info = Observation()->GetGameInfo();

            for (int i = std::max(0, int(unit->pos.x - unit_stats.sight_range) - 2);
                 i < std::min(game_info.width, int(unit->pos.x + unit_stats.sight_range) + 2); i++) {
                for (int j = std::max(0, int(unit->pos.y - unit_stats.sight_range) - 2);
                     j < std::min(game_info.height, int(unit->pos.y + unit_stats.sight_range) + 2); j++) {
                    if (Distance2D(Point2D{i + 0.5F, j + 0.5F}, unit->pos) < unit_stats.sight_range) {
                        imRef(Aux::influenceMapEnemy, i, j) += 1;
                    }
                }
            }
        }
    }
};

//MICRO
//executeAttack doesnt work for some reason, redo it its also pretty dogshit code

//primordialstar optimizations: 
//-check max num of paths from nodes and max length of a path to optimize
//-modify max dist to not include rocks

//add chrono

//-rework enemy squads code

//-add scout to initial probe

//fix probe creation logic (currently hardcoded)

//-add search and destroy code

//units should still retreat when no targets nearby

//-every check, random find N positions to search, smallest of all, (slowly gets me a best position)

//-better stalker teleporting with new damagenet

//fix probe targetting to prioritize minerals with nexi

//fully optimize

//add no build zone
//fix building placements with lockout
//remove double actions from army units

//add correctly timed kiting
//stay on border of your own

//weapon net, 8x precision

//-tag on units that defines whether they are being individual controlled or group-controlled

//minute micro

//predictive theoretical health stutter step when no blink

//make sure all units are engaging in fight
//add ramp analysis

//add check for no possible units that can execute an action at all

//have nexus stop checking when it has two vespenes
//fix probe build-check

//?fix assimilator condition

//add generator to global variables

//if near supply cap, save for pylon

//build list

//army list

//per-unit micro

//squadrons
