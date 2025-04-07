#pragma once
#include <sc2api/sc2_api.h>

#include <map>
 
#include "constants.h"
#include "profiler.hpp"
#include "unit.hpp"
#include "spacialhashgrid.hpp"
#include "unitpriority.hpp"
#include "debugging.hpp"
#include "primordialstar.hpp"

constexpr int BERTH = 1;
constexpr int numChecksSearch = 100;

enum SquadMode {
    ATTACK,
    RETREAT,
    DEFEND,
    FULL_RETREAT,
    SEARCH
};

char *SquadModeToString(SquadMode mode) {
    if (mode == ATTACK) {
        return "ATTACK";
    } else if (mode == RETREAT) {
        return "RETREAT";
    } else if (mode == DEFEND) {
        return "DEFEND";
    } else if (mode == FULL_RETREAT) {
        return "FULL_RETREAT";
    } else if (mode == SEARCH) {
        return "SEARCH";
    }
    return "HUH?";
}

//map2dFloat<float>* enemyDamageNet;

struct DamageNet {
    Point2D pos;
    Weapon weapon;
};

constexpr int escapePointChecks = 5;
constexpr float escapePointWeight = 10.0F;
constexpr float escapePointRadius = 12.0F;

constexpr float squadExtraRadius = 6.0F;
constexpr int alongPurePathBisects = 4;


class Squad {
private:
    UnitWrapper* core;
    Composition comp;

public:    
    Point2D location;

    Point2D intermediateLoc;
    SquadMode mode;
    

    /*
    * u is unjoined, hasn't yet joined the squad
    * m is manual, operates separately from the squad
    * n is normal
    * r is retreating
    */
    std::map<Tag, char> squadStates; 

    std::map<Tag, char> subSquadStates;

    float radius;
    int8_t ignoreFrames;

    std::vector<UnitWrapper *> army;
    std::vector<UnitWrapper*> targets;

    Squad() {
        army = std::vector<UnitWrapper *>();
        targets = std::vector<UnitWrapper*>();
        radius = 0;
        comp = Composition::Invalid;
        squadStates = std::map<Tag, char>();
        subSquadStates = std::map<Tag, char>();
        ignoreFrames = 0;
        core = nullptr;
    }

    bool has(UnitTypeID type) {
        for (UnitWrapper *u : army) {
            if (u->getStorageType() == type) {
                return true;
            }
        }
        return false;
    }

    bool find(UnitTypeID tag) {
        for (UnitWrapper *u : army) {
            if (u->self == tag) {
                return true;
            }
        }
        return false;
    }

    Point2D numericalCenter(Agent* agent) {
        Point2D center = { 0, 0 };
        if (army.size() == 0)
            return center;
        int cnt = 0;
        for (int i = 0; i < army.size(); i++) {
            if (squadStates[army[i]->self] == 'n') {
                center += army[i]->pos(agent);
                cnt++;
            }
        }
        if (cnt == 0) {
            return army[std::rand() % army.size()]->pos(agent);
        }
        return (center / (float)cnt);
    }

    UnitWrapper* getCore(Agent* agent) {
        if (core == nullptr || core->get(agent) == nullptr) {
            Composition c = composition(agent);
            Point2D numCenter = numericalCenter(agent);
            UnitWrapper* newCore = nullptr;
            float dist2 = -1;
            for (auto wrap : army) {
                if (c == Composition::Air || (wrap->get(agent) != nullptr && !wrap->get(agent)->is_flying)) {
                    Point2D pos = wrap->pos(agent);
                    float distWrap = DistanceSquared2D(pos, numCenter);
                    if (dist2 == -1 || distWrap < dist2) {
                        dist2 = distWrap;
                        newCore = wrap;
                    }
                }
            }
            core = newCore;
        }
        return core;
    }

    Point2D coreCenter(Agent* agent) {
        if (getCore(agent) != nullptr) {
            return core->pos(agent);
        }
        return Point2D{ 0,0 };
    }

    Composition composition(Agent *agent) {
        if (comp != Composition::Invalid) {
            return comp;
        }
        for (int i = 0; i < army.size(); i++) {
            if (comp != Composition::Any) {
                singleUnitComp(army[i], agent);
            } else {
                break;
            }
        }
        return comp;
    }

    void singleUnitComp(UnitWrapper *unit, Agent *agent) {
        if (unit->get(agent)->is_flying) {
            if (comp == Composition::Invalid) {
                comp = Composition::Air;
                return;
            } else if (comp == Composition::Ground) {
                comp = Composition::Any;
                return;
            }
        } else {
            if (comp == Composition::Invalid) {
                comp = Composition::Ground;
                return;
            } else if (comp == Composition::Air) {
                comp = Composition::Any;
                return;
            }
        }
    }

    float armyballRadius() {
        if (radius != 0)
            return radius;
        else {
            //printf("A:%d SQRT%.1f\n", army.size(), std::sqrt(army.size()));
            return (float)std::sqrt(army.size())*2;
        }
    }

    bool execute(Agent *agent) {
        //auto squadEx = Profiler("sE");
        if (mode == ATTACK) {
            if (ignoreFrames > 0) {
                ignoreFrames--;
                return false;
            }
            
            //targets.clear();
            if (coreCenter(agent) != Point2D{ 0,0 }) {
                Circles c;
                for (int i = 0; i < army.size(); i++) {
                    if (squadStates[army[i]->self] != 'u') {
                        c.push_back(Circle{ army[i]->pos(agent), Army::maxWeaponRadius(army[i]->getType(agent)) });
                    }
                }
                targets = SpacialHash::findInRadiiEnemy(c, agent);//SpacialHash::findInRadiusEnemy(coreCenter(agent), armyballRadius() + squadExtraRadius, agent);
            }
            //targets = SpacialHash::findInRadiusEnemy(coreCenter(agent), armyballRadius() + squadExtraRadius, agent);

            ignoreFrames = 3;
            return false;
        } else if (mode == RETREAT) {
            return false;
        } else if (mode == DEFEND) {
            return false;
        } else if (mode == FULL_RETREAT) {
            return false;
        } else if (mode == SEARCH) {
            return false;
        }

        //for (auto wrap : army) {
        //    if (squadStates[wrap->self] == 'u' && Distance2D(wrap->pos(agent), coreCenter(agent)) < armyballRadius()) {
        //        squadStates[wrap->self] = 'n';
        //    }
        //}

        return false;
    }

    bool attack(Point2D location_) {
        mode = ATTACK;
        location = location_;
        return false;
    }

    bool retreat(Point2D location_) {
        mode = RETREAT;
        location = location_;
        return false;
    }

    bool defend(Point2D location_) {
        mode = DEFEND;
        location = location_;
        return false;
    }

    bool fullRetreat(Point2D location_) {
        mode = FULL_RETREAT;
        location = location_;
        return false;
    }

    bool search(Point2D location_) {
        mode = SEARCH;
        location = location_;
        return false;
    }

    int coreCount() {
        int c = 0;
        for (int i = 0; i < army.size(); i++) {
            if (squadStates[army[i]->self] != 'u') {
                c++;
            }
        }
        return c;
    }
};

std::vector<Squad> squads = std::vector<Squad>();

class ArmyUnit : public UnitWrapper {
private:
    
    UnitTypeData stats;
    //bool is_flying;
    //int8_t targetFrames = 0;
    
    
    float escapeCost;
    float distToTarget;

public:
    Point2D escapeLoc;

    Point2D posTarget;
    UnitWrapper* targetWrap;
    Squad* squad;

    ArmyUnit(const Unit* unit) : UnitWrapper(unit) {
        targetWrap = nullptr;
        if (squads.size() == 0) {
            squads.emplace_back();
        }
        squads[0].army.push_back(this);
        squad = &squads[0];
        //is_flying = unit->is_flying; 
        squad->squadStates[self] = 'u';
        squad->subSquadStates[self] = '.';
        escapeLoc = { 0,0 };
        escapeCost = -1;
        distToTarget = -1;
    }

    virtual void attack(UnitWrapper* wrap, Agent* agent) {
        agent->Actions()->UnitCommand(self, ABILITY_ID::ATTACK, wrap->self);
    }

    UnitTypeData getStats(Agent *agent) {
        if (stats.unit_type_id == UNIT_TYPEID::INVALID) {
            stats = Aux::getStats(getType(agent),agent);
        }
        return stats;
    }

    bool withSquad(Agent* agent) {
        //printf("D%.1f %.1f\n", Distance2D(pos(agent), squad->coreCenter(agent)), squad->armyballRadius());
        return Distance2D(pos(agent), squad->coreCenter(agent)) < squad->armyballRadius();
    }

    float priorityAttack(Weapon w, UnitWrapper* op, Agent *agent) {  // HIGHER IS MORE DESIRABLE TO ATTACK
        if (Army::hitsUnit(w.type, op->getComposition(agent))) {
            return Army::Priority(getType(agent), op->getType(agent)) - 3 * ((op->health + op->shields)/(op->healthMax + op->shieldsMax)); //include health
        }
        //if (Army::hitsUnit(w.type, Army::unitTypeTargetComposition(op->type))) {
        //    return Army::Priority(type, op->type); //include health
        //}
        return -1;
    }

    float priorityAvoid(UnitTypeID self_type, UnitTypeID opponent_type, Weapon weapon,
        Agent* agent) {  // HIGHER IS MORE DESIRABLE TO AVOID
        //UnitTypes allData = agent->Observation()->GetUnitTypeData();
        //UnitTypeData myStats = allData.at(static_cast<uint32_t>(self_type));
        //UnitTypeData enemyStats = allData.at(static_cast<uint32_t>(opponent->unit_type));
        return 1;
    }

    virtual bool execute(Agent *agent) {
        if (get(agent) == nullptr) {
            return false;
        }
        if (squad->mode == ATTACK) {
            return executeAttack(agent);
        } else if (squad->mode == RETREAT) {
            return executeRetreat(agent);
        } else if (squad->mode == DEFEND) {
            return executeDefend(agent);
        } else if (squad->mode == FULL_RETREAT) {
            return executeFullRetreat(agent);
        } else if (squad->mode == SEARCH) {
            return executeSearch(agent);
        }
        return false;
    }

    UnitWrappers getTargetEnemy(UnitWrappers targets, Agent* agent) {
        Point2D position = pos(agent);
        UnitWrappers potentialTargets = UnitWrappers();
        std::vector<float> potentialPriority = std::vector<float>();
        for (UnitWrapper* enemy : targets) {
            for (Weapon w : getStats(agent).weapons) {
                float weaponRadius = w.range + radius + enemy->radius;
                float enemyRadius = Distance2D(position, enemy->pos(agent));
                if (Army::hitsUnit(w.type, enemy->getComposition(agent))) {
                    bool inserted = false;
                    float priority = priorityAttack(w, enemy, agent);
                    if (enemyRadius > weaponRadius) {
                        priority += (weaponRadius - enemyRadius) * 4.0F;
                    }
                    if (potentialTargets.size() == 0) {
                        inserted = true;
                        potentialTargets.push_back(enemy);
                        potentialPriority.push_back(priority);
                    }
                    for (int d = 0; d < potentialTargets.size(); d++) {
                        if (potentialPriority[d] < priority) {
                            potentialTargets.insert(potentialTargets.begin() + d, enemy);
                            potentialPriority.insert(potentialPriority.begin() + d, priority);
                            inserted = true;
                            break;
                        }
                    }
                    if (inserted == false) {
                        potentialTargets.push_back(enemy);
                        potentialPriority.push_back(priority);
                    }
                    break;
                }
            }

        }
        return potentialTargets;
    }

    float calculatePathDamage(Point2D start, Point2D end, Agent* agent) {
        float damageCost = 0.0F;
        vector<Point2D> fullpath;
        if (get(agent)->is_flying) {
            fullpath = { start, end };
        }
        else {
            fullpath = PrimordialStar::getPath(start, end, radius, agent);
        }
        vector<Point2D> stepPoints = PrimordialStar::stepPointsAlongPath(fullpath, 1.0F);

        if (fullpath.size() != 0) {
            float l2 = 0;
            for (int i = 0; i < fullpath.size() - 1; i++) {
                l2 += Distance2D(fullpath[i], fullpath[i + 1]);
                //DebugLine(agent, AP3D(fullpath[i]) + Point3D{ 0,0,1 }, AP3D(fullpath[i + 1]) + Point3D{ 0,0,1 }, Colors::Green);
            }
            for (Point2D p : stepPoints) {
                DamageLocation d = UnitManager::getRadiusAvgDamage(P2D(p) + Point2D{ 0.5F,0.5F }, radius);
                damageCost += UnitManager::getRelevantDamage(this, d, agent) * 0.2F;

                //DebugSphere(agent, AP3D(p), 0.5, { 21,42,220 });
                //DebugText(agent, strprintf("%.2f", damageCost), AP3D(p) + Point3D{ 0,0,1 });
            }
            //DebugSphere(this, P3D(PrimordialStar::distanceAlongPath(path, 5.5)), 0.5, {21,42,120});
        }
        damageCost /= (PrimordialStar::getPathLength(fullpath) + 4.0F);
        damageCost += UnitManager::getRelevantDamage(this, UnitManager::getRadiusAvgDamage(end, radius + 2.0F), agent);
        return damageCost;
    }

    Point2D randomPointRadius(Point2D center, float rad) {
        float theta = ((float)std::rand()) * 2 * M_PI / RAND_MAX;
        float r = ((float)std::rand()) * rad / RAND_MAX;
        float x = std::cos(theta) * r;
        float y = std::sin(theta) * r;

        return Point2D { center.x + x, center.y + y };
    }

    //void updateRandomMinDamagePoint(Point2D position, float rad, int numChecks, Point2D posTarget, Agent* agent) {
    //    for (int i = 0; i < escapePointChecks; i++) {
    //        Profiler profil("escapePoint");
    //        
    //        float theta = ((float)std::rand()) * 2 * 3.1415926 / RAND_MAX;
    //        float r = ((float)std::rand()) * rad / RAND_MAX;
    //        float x = std::cos(theta) * r;
    //        float y = std::sin(theta) * r;

    //        Point2DI escapePoint{ int(position.x + x), int(position.y + y) };
    //        Point2D escapePointF{ position.x + x, position.y + y };
    //        if (!Aux::checkPathable(escapePointF, agent)) {
    //            i--;
    //            continue;
    //        }
    //        profil.midLog("escapePoint-setup");
    //        //auto came_from = jps(gridmap, position, escapePoint, Tool::euclidean, agent);
    //        //vector<Location> pat = Tool::reconstruct_path(Location(position), escapePoint, came_from);
    //        //profil.midLog("escapePoint-pathing");
    //        //if (pat.size() == 0) {
    //        //    i--;
    //        //    continue;
    //        //}
    //        //vector<Point2DI> path = fullPath(pat);
    //        //profil.midLog("escapePoint-fullPath");

    //        float damageCost = calculatePathDamage(position, escapePointF, agent);

    //        //vector<Point2D> fullpath = PrimordialStar::getPath(position, escapePointF, radius, agent);
    //        //vector<Point2D> stepPoints = PrimordialStar::stepPointsAlongPath(fullpath, 1.0F);

    //        //if (fullpath.size() != 0) {
    //        //    float l2 = 0;
    //        //    for (int i = 0; i < fullpath.size() - 1; i++) {
    //        //        l2 += Distance2D(fullpath[i], fullpath[i + 1]);
    //        //        DebugLine(agent, AP3D(fullpath[i]) + Point3D{ 0,0,1 }, AP3D(fullpath[i + 1]) + Point3D{ 0,0,1 }, Colors::Green);
    //        //    }
    //        //    for (Point2D p : stepPoints) {
    //        //        DamageLocation d = UnitManager::getRadiusAvgDamage(P2D(p) + Point2D{ 0.5F,0.5F }, radius, agent);
    //        //        damageCost += UnitManager::getRelevantDamage(this, d, agent) * 0.2;

    //        //        DebugSphere(agent, AP3D(p), 0.5, { 21,42,220 });
    //        //        DebugText(agent, strprintf("%.2f", damageCost), AP3D(p) + Point3D{0,0,1});
    //        //    }
    //        //    //DebugSphere(this, P3D(PrimordialStar::distanceAlongPath(path, 5.5)), 0.5, {21,42,120});
    //        //}
    //        //damageCost /= (PrimordialStar::getPathLength(fullpath) + 4.0F);
    //        //damageCost += UnitManager::getRelevantDamage(this, UnitManager::getRadiusAvgDamage(escapePointF, radius + 2.0F, agent), agent);

    //        //printf("%.1f,%.1f eC:%.1f dC:%.1fPRE\n",x ,y , escapeCost, damageCost);
    //        DebugLine(agent,Point3D{ escapePointF.x,escapePointF.y,0 }, Point3D{ escapePointF.x,escapePointF.y, 53.0F });

    //        if (escapeCost == -1 || damageCost < escapeCost) {
    //            escapeCost = damageCost;
    //            escapeLoc = escapePointF;
    //            escapeDist = -1;
    //        }
    //        else {
    //            //if (escapeDist == -1) {
    //            //    //escapeDist = agent->Query()->PathingDistance(escapeLoc, posTarget);
    //            //    escapeDist = PrimordialStar::getPathLength(escapeLoc, posTarget, radius, agent);
    //            //}
    //            escapeDist = PrimordialStar::getPathLength(escapeLoc, posTarget, radius, agent);
    //            //float damageDist = agent->Query()->PathingDistance(escapePointF, posTarget);
    //            float damageDist = PrimordialStar::getPathLength(escapePointF, posTarget, radius, agent);
    //            if (damageCost == escapeCost && damageDist < escapeDist) {
    //                escapeCost = damageCost;
    //                escapeLoc = escapePointF;
    //                escapeDist = -1;
    //            }
    //        }
    //        //printf("eC:%.1f dC:%.1fPOST\n", escapeCost, damageCost);
    //        profil.midLog("escapePoint-endcheck");
    //    }
    //}

    virtual bool executeAttack(Agent *agent) {
        if (ignoreFrames > 0) {
            ignoreFrames--;
        }
        else {
            /*
            * . for nothing
            * a for attacking
            * r for retreating/going to safety
            * c for go to squad (no targets for them)
            * j for joining
            * k for attack joining
            * m for moving (with squad) //NOTUSED
            * l for no shields escape attack
            * p for no shields escape 
            * h for GET THE FUCK OUT YOU'RE LOW
            */
            //char mode = '.';
            squad->subSquadStates[self] = '.';
            targetWrap = nullptr;
            Point2D position = pos(agent);
            posTarget = { 0,0 };
            distToTarget = -1;
            if (squad->squadStates[self] == 'u') {
                if (withSquad(agent)) {
                    squad->squadStates[self] = 'n';
                }
            }

            if (squad->squadStates[self] == 'u') {
                //UnitWrappers potentialTargets = getTargetEnemy(SpacialHash::findInRadiusEnemy(position, Army::maxWeaponRadius(self) + radius, agent), agent);
                //if(position)
                posTarget = squad->coreCenter(agent);
                if (get(agent)->weapon_cooldown > 0) {
                    //mode = 'j';
                    squad->subSquadStates[self] = 'j';
                }
                else {
                    //mode = 'k';
                    squad->subSquadStates[self] = 'k';
                }
            }
            else {

                //UnitWrappers personalTargets = SpacialHash::findInRadiusEnemy(position, Army::maxWeaponRadius(type), agent);
                //for (int i = 0; i < squad->targets.size(); i++) {
                //    if (std::find(personalTargets.begin(), personalTargets.end(), squad->targets[i]) == personalTargets.end()) {
                //        personalTargets.push_back(squad->targets[i]);
                //    }
                //}

                //UnitWrappers potentialTargets = getTargetEnemy(personalTargets, agent);

                UnitWrappers potentialTargets = getTargetEnemy(squad->targets, agent);
                if ((health + shields) / (healthMax + shieldsMax) < 0.25) {
                    squad->subSquadStates[self] = 'h';
                } else if (shields < 0.05) {
                    if (get(agent)->weapon_cooldown > 0) {
                        squad->subSquadStates[self] = 'p';
                    }
                    else {
                        squad->subSquadStates[self] = 'l';
                    }
                }
                else if (potentialTargets.size() != 0 && getStats(agent).weapons.size() > 0) {
                    targetWrap = potentialTargets.front();

                    Weapon weap = getStats(agent).weapons[0];
                    float damage = UnitManager::getRelevantDamage(targetWrap, weap, agent);
                    for (int w = 1; w < getStats(agent).weapons.size(); w++) {
                        float dmag = UnitManager::getRelevantDamage(targetWrap, getStats(agent).weapons[w], agent);
                        if (dmag > damage) {
                            damage = dmag;
                            weap = getStats(agent).weapons[w];
                        }
                    }

                    float damageRadius = radius + targetWrap->radius + weap.range;

                    float dTtoEnemy = abs(Distance2D(pos(agent), targetWrap->pos(agent)) - damageRadius) / getStats(agent).movement_speed;
                    //printf("dto %f:wC %f\n", dTtoEnemy, get(agent)->weapon_cooldown);
                    if (dTtoEnemy >= get(agent)->weapon_cooldown) {
                        posTarget = targetWrap->pos(agent);
                        //mode = 'a';
                        squad->subSquadStates[self] = 'a';
                    }
                    else {
                        posTarget = squad->coreCenter(agent);//posTarget = pos(agent);
                        //mode = 'r';
                        squad->subSquadStates[self] = 'c';
                    }
                }
                else {
                    if (withSquad(agent)) {
                        posTarget = squad->location;
                        squad->subSquadStates[self] = 'r';
                    }
                    else{
                        posTarget = squad->coreCenter(agent);
                        if (get(agent)->weapon_cooldown > 0) {
                            //mode = 'j';
                            squad->subSquadStates[self] = 'j';
                        }
                        else {
                            //mode = 'k';
                            squad->subSquadStates[self] = 'k';
                        }
                    }
                    //mode = 'r';
                }
            }

            if (squad->subSquadStates[self] == 'a') {
                //agent->Actions()->UnitCommand(self, ABILITY_ID::ATTACK, targetWrap->self);
                attack(targetWrap, agent);
                escapeLoc = posTarget;
            }
            else if (squad->subSquadStates[self] == 'k') {
                agent->Actions()->UnitCommand(self, ABILITY_ID::ATTACK, posTarget);
                escapeLoc = posTarget;
            }
            else if (squad->subSquadStates[self] == 'p' || squad->subSquadStates[self] == 'h' || squad->subSquadStates[self] == 'l') {
                if (!Aux::isWithin(escapeLoc, agent)) {
                    escapeLoc = position;
                }
                escapeCost = calculatePathDamage(position, escapeLoc, agent);

                for (int i = 0; i < escapePointChecks; i++) {
                    Point2D checkPoint = randomPointRadius(position, escapePointRadius);
                    if (!Aux::isWithin(checkPoint, agent)) {
                        i--;
                        continue;
                    }
                    float checkCost = calculatePathDamage(position, checkPoint, agent);
                    if (escapeCost > checkCost) {
                        escapeLoc = checkPoint;
                        escapeCost = checkCost;
                    }
                    else if (escapeCost == checkCost) {
                        if (imRef(PrimordialStar::minDistanceGrid, (int)escapeLoc.x, (int)escapeLoc.y).distance < imRef(PrimordialStar::minDistanceGrid, (int)escapeLoc.x, (int)escapeLoc.y).distance) {
                            escapeLoc = checkPoint;
                            escapeCost = checkCost;
                        }
                    }
                }

                agent->Actions()->UnitCommand(self, ABILITY_ID::MOVE_MOVE, escapeLoc);
                DebugLine(agent, Point3D{ escapeLoc.x,escapeLoc.y, pos3D(agent).z }, Point3D{ escapeLoc.x,escapeLoc.y, pos3D(agent).z + 1.5F }, Colors::Purple);

            }
            else if (squad->subSquadStates[self] == 'j' || squad->subSquadStates[self] == 'r' || squad->subSquadStates[self] == 'c') {
                if (!Aux::isWithin(escapeLoc,agent)) {
                    escapeLoc = position;
                }
                escapeCost = calculatePathDamage(position, escapeLoc, agent);
                vector<Point2D> pathToTarget;
                if (get(agent)->is_flying) {
                    pathToTarget = { escapeLoc, posTarget };
                }
                else {
                    pathToTarget = PrimordialStar::getPath(escapeLoc, posTarget, radius, agent);
                }
                distToTarget = PrimordialStar::getPathLength(pathToTarget);

                if (calculatePathDamage(position, posTarget, agent) == 0) {
                    escapeLoc = posTarget;
                    escapeCost = 0;
                    distToTarget = 0;
                }
                else {
                    int numPoints = escapePointChecks + alongPurePathBisects - 1;
                    vector<Point2D> checkPoints;
                    vector<float> checkPointDistances;
                    checkPoints.reserve(numPoints);
                    for (int i = 0; i < escapePointChecks; i++) {
                        Point2D checkPoint = randomPointRadius(position, escapePointRadius);
                        if (!Aux::checkPathable(checkPoint)) {
                            i--;
                            continue;
                        }
                        checkPoints.push_back(checkPoint);
                        checkPointDistances.push_back(-1);
                    }
                    if (position != posTarget && !get(agent)->is_flying) {
                        vector<Point2D> pathDijkstra = PrimordialStar::getPath(position, posTarget, radius, agent);
                        //vector<Point2D> pathDijkstra = PrimordialStar::getPathDijkstra(position, posTarget, radius, agent);
                        float dijkstraLength = PrimordialStar::getPathLength(pathDijkstra);
                        for (int i = 0; i < alongPurePathBisects - 1; i++) {
                            Point2D checkPoint = PrimordialStar::distanceAlongPath(pathDijkstra, (i + 1) * dijkstraLength / alongPurePathBisects);
                            if (!isinf(checkPoint.x) && !isinf(checkPoint.y) && (checkPoint.x == checkPoint.x) && (checkPoint.y == checkPoint.y)) {
                                checkPoints.push_back(checkPoint);
                                checkPointDistances.push_back((3 - i) * dijkstraLength / alongPurePathBisects);
                            }
                            else {
                                printf("INFINF\n");
                            }
                        }
                    }

                    for (int i = 0; i < checkPoints.size(); i++) {
                        Point2D checkPoint = checkPoints[i];
                        float checkCost = calculatePathDamage(position, checkPoint, agent);
                        if (escapeCost > checkCost) {
                            escapeLoc = checkPoint;
                            escapeCost = checkCost;
                            distToTarget = -1;
                        }
                        else if (escapeCost == checkCost) {
                            if (distToTarget == -1) {
                                vector<Point2D> pathToTarget;
                                if (get(agent)->is_flying) {
                                    pathToTarget = { escapeLoc, posTarget };
                                }
                                else {
                                    pathToTarget = PrimordialStar::getPath(escapeLoc, posTarget, radius, agent);
                                }
                                distToTarget = PrimordialStar::getPathLength(pathToTarget);
                            }
                            if (checkPointDistances[i] == -1) {
                                vector<Point2D> pathToTarget;
                                if (get(agent)->is_flying) {
                                    pathToTarget = { escapeLoc, posTarget };
                                }
                                else {
                                    pathToTarget = PrimordialStar::getPath(checkPoint, posTarget, radius, agent);
                                }
                                checkPointDistances[i] = PrimordialStar::getPathLength(pathToTarget);
                            }
                            if (distToTarget > checkPointDistances[i]) {
                                escapeLoc = checkPoint;
                                escapeCost = checkCost;
                                distToTarget = checkPointDistances[i];
                            }
                        }
                    }
                }

                agent->Actions()->UnitCommand(self, ABILITY_ID::MOVE_MOVE, escapeLoc);
                DebugLine(agent,Point3D{ escapeLoc.x,escapeLoc.y, pos3D(agent).z}, Point3D{escapeLoc.x,escapeLoc.y, pos3D(agent).z + 1.5F}, Colors::Purple);
            }
            //statTargetPos = posTarget;
            ignoreFrames = 5;
        }
        return false;
    }

    virtual bool executeRetreat(Agent *agent) {
        Units enemies = agent->Observation()->GetUnits(Unit::Alliance::Enemy);
        return false;
    }

    virtual bool executeDefend(Agent *agent) {
        Units enemies = agent->Observation()->GetUnits(Unit::Alliance::Enemy);
        float dist = Distance2D(squad->location, pos(agent));
        //printf("%s %.1f\n", UnitTypeToName(type), dist);
        if ((get(agent)->orders.size() == 0 || get(agent)->orders[0].target_pos != squad->location) &&
            dist > squad->armyballRadius()) {
            agent->Actions()->UnitCommand(self, ABILITY_ID::ATTACK, squad->location);
        }
        return false;
    }

    virtual bool executeFullRetreat(Agent *agent) {
        Units enemies = agent->Observation()->GetUnits(Unit::Alliance::Enemy);
        return false;
    }

    float searchCost(Point2D p) {
        if (p.x == 0 && p.y == 0) {
            return -1;
        }
        return imRef(Aux::visionMap, (int)(p.x), (int)(p.y));
    }

    virtual bool executeSearch(Agent* agent) {
        //if (DistanceSquared2D(pos(agent), posTarget) < 9 || posTarget == Point2D{0,0}) {
        //    float cost = -1;
        //    posTarget = { 0,0 };
        //    for (int i = 0; i < numChecksSearch; i++) {
        //        Point2D check;
        //        if (get(agent)->is_flying) {
        //            check = Aux::getRandomNonPathable(agent);
        //        }
        //        else {
        //            check = Aux::getRandomPathable(agent);
        //        }
        //        float c = searchCost(check);
        //        if (c < cost || cost == -1) {
        //            cost = c;
        //            posTarget = check;
        //        }
        //    }
        //}
        float cost = -1;
        if (Aux::isWithin(posTarget, agent)) {
            cost = searchCost(posTarget);
        }
        //posTarget = { 0,0 };
        for (int i = 0; i < numChecksSearch; i++) {
            Point2D check;
            if (get(agent)->is_flying) {
                check = Aux::getRandomNonPathable(agent);
            }
            else {
                check = Aux::getRandomPathable(agent);
            }
            float cos = searchCost(check);
            if (cos < cost || cost == -1) {
                cost = cos;
                posTarget = check;
            }
        }
        agent->Actions()->UnitCommand(self, ABILITY_ID::MOVE_MOVE, posTarget);
        
        return false;
    }

    virtual bool executeDamaged(Agent *agent, float u_health, float u_shields) {
        Units enemies = agent->Observation()->GetUnits(Unit::Alliance::Enemy);
        return false;
    }

    virtual ~ArmyUnit() {
        for (auto it = squad->army.begin(); it != squad->army.end(); it++) {
            // printf("%lx %lx", )
            if ((*it)->self == self) {
                squad->army.erase(it);
                break;
            }
        }
    }
};

constexpr int blinkChecks = 20;

class Stalker : public ArmyUnit {
private:
public:
    Stalker(const Unit *unit) : ArmyUnit(unit) {
    }

    //the higher the more u want to go.
    float blinkPriority(Point2D pos, Agent* agent) {
        return -UnitManager::getRelevantDamage(this, UnitManager::getRadiusAvgDamage(pos, radius + 1.0F), agent);
    }

    virtual bool executeDamaged(Agent *agent, float u_health, float u_shields) {
        if (u_shields < 0.05 && checkAbility(ABILITY_ID::EFFECT_BLINK)) {
            printf("TELEPORT STALKER\n");

            float priorityMax = -1;
            Point2D pos;

            for (int i = 0; i < blinkChecks; i++) {
                float theta = ((float)std::rand()) * 2 * M_PI / RAND_MAX;
                float magnitude = 8;//((float)std::rand()) * 8 / RAND_MAX;

                Point2D displace{ cos(theta) * magnitude, sin(theta) * magnitude };

                Point3D upos = pos3D(agent);

                Point3D blinkPos{ upos.x + displace.x, upos.y + displace.y, upos.z };

                if (!Aux::checkPathable(blinkPos)) {
                    i--;
                    continue;
                }

                float priority = blinkPriority(blinkPos, agent);
                
                if (priorityMax == -1 || priorityMax < priority) {
                    pos = blinkPos;
                    priorityMax = priority;
                }

                DebugLine(agent, upos, blinkPos, { 240, 73, 250 });

            }

            agent->Actions()->UnitCommand(self, ABILITY_ID::EFFECT_BLINK_STALKER, pos);
        }
        return false;
    }
};

class ObserverEye : public ArmyUnit {
private:

public:
    ObserverEye(const Unit *unit) : ArmyUnit(unit) {
        squad->squadStates[self] = 'm';
    }

    virtual bool execute(Agent *agent) {
        if (ignoreFrames > 0) {
            ignoreFrames--;
            return false;
        }
        agent->Actions()->UnitCommand(self, ABILITY_ID::MOVE_MOVE, squad->coreCenter(agent));
        ignoreFrames = 50;
        return false;
    }
};

class Zealot : public ArmyUnit {
private:

public:
    Zealot(const Unit* unit) : ArmyUnit(unit) {
        squad->squadStates[self] = 'm';
    }

    virtual bool execute(Agent* agent) {
        if (ignoreFrames > 0) {
            ignoreFrames--;
            return false;
        }
        if (squad->intermediateLoc != Point2D{ 0,0 }) {
            agent->Actions()->UnitCommand(self, ABILITY_ID::ATTACK, squad->intermediateLoc);
        }
        else {
            agent->Actions()->UnitCommand(self, ABILITY_ID::ATTACK, squad->location);
        }
        ignoreFrames = 3;
        return false;
    }
};

class WarpPrism : public ArmyUnit {
private:
public:
    WarpPrism(const Unit *unit) : ArmyUnit(unit) {
    }

    // virtual bool execute(Agent *agent) {
    //
    // }
};

class Adept : public ArmyUnit {
private:
public:
    Adept(const Unit *unit) : ArmyUnit(unit) {
    }

    // virtual bool execute(Agent *agent) {
    //
    // }
};

class Immortal : public ArmyUnit {
private:
public:
    Immortal(const Unit *unit) : ArmyUnit(unit) {
    }

    // virtual bool execute(Agent *agent) {
    //
    // }
};

class Disruptor : public ArmyUnit {
private:
public:
    Disruptor(const Unit* unit) : ArmyUnit(unit) {
    }

    virtual void attack(UnitWrapper* wrap, Agent* agent) {
        //agent->Actions()->UnitCommand(self, ABILITY_ID::PURIFICATIONNOVA, wrap->self);
        //agent->Actions()->UnitCommand(self, ABILITY_ID::PURIFICATIONNOVA, wrap->pos(agent));
        //agent->Actions()->UnitCommand(self, ABILITY_ID::EFFECT_PURIFICATIONNOVA, wrap->self);
        agent->Actions()->UnitCommand(self, ABILITY_ID::EFFECT_PURIFICATIONNOVA, wrap->pos(agent));
    }

    // virtual bool execute(Agent *agent) {
    //
    // }
};

constexpr float bonusOnKill = 2.0F;

class PurificationNova : public ArmyUnit {
private:
public:

    int lifetime; //2.1 * 22.4 = 47.04 frames

    float escapeBenefit;

    PurificationNova(const Unit* unit) : ArmyUnit(unit), lifetime(47), escapeBenefit(-1) {
        escapeLoc = { 0,0 };
    }

    float calculateBenefit(Point2D p, Agent* agent) {
        UnitWrappers enemies = SpacialHash::findInRadiusEnemy(p, 1.5F, agent);
        float benefit = 0;
        for (int i = 0; i < enemies.size(); i++) {
            UnitWrapper* enemy = enemies[i];
            if (enemy->getComposition(agent) == Composition::Ground && Distance2D(p, enemy->pos(agent)) < 1.5F + enemy->radius) {
                float remainingShield = max(0.0F, enemy->shields - 100);
                float remainHealth = max(0.0F, enemy->health + remainingShield - 100);
                float depletedHealth = enemy->health + enemy->shields - remainHealth;

                float benefitSingle = depletedHealth / 200.0F;
                if (remainHealth == 0) {
                    benefitSingle += bonusOnKill;
                }
                benefit += benefitSingle * priorityAttack(Aux::extraWeapons[ABILITY_ID::EFFECT_PURIFICATIONNOVA].w, enemy, agent);
            }
        }
        return benefit;
    }

    virtual bool execute(Agent *agent) {
        float radiusRemaining = lifetime / fps * Aux::getStats(UNIT_TYPEID::PROTOSS_DISRUPTORPHASED, agent).movement_speed;
        Point3D p = pos3D(agent);

        DebugSphere(agent, p, radiusRemaining, Colors::Yellow);
        escapeBenefit = calculateBenefit(escapeLoc, agent);

        vector<Point2D> checkPoints;
        UnitWrappers enemies = SpacialHash::findInRadiusEnemy(p, radiusRemaining, agent);
        checkPoints.reserve(enemies.size() + 3);

        for (int i = 0; i < enemies.size(); i++) {
            checkPoints.push_back(enemies[i]->pos(agent));
        }

        for (int i = 0; i < 3; i++) {
            checkPoints.push_back(randomPointRadius(pos(agent), radiusRemaining));
        }

        for (int i = 0; i < checkPoints.size(); i++) {
            Point2D checkP = checkPoints[i];
            float checkBenefit = calculateBenefit(checkP, agent);
            DebugSphere(agent, AP3D(checkP), 0.3F, checkBenefit == 0 ? Colors::Green : Colors::Red);
            if (escapeBenefit == -1 || escapeBenefit < checkBenefit) {
                escapeLoc = checkP;
                escapeBenefit = checkBenefit;
                agent->Actions()->UnitCommand(self, ABILITY_ID::GENERAL_MOVE, checkP);
            }
        }

        lifetime--;
        return false;
    }
};

class HighTemplar : public ArmyUnit {
private:
public:
    HighTemplar(const Unit* unit) : ArmyUnit(unit) {
    }

    virtual bool execute(Agent* agent) {
        const Unit* uni = get(agent);
        if (uni == nullptr) {
            return false;
        }
        if (uni->orders.size() > 0 && (uni->orders[0].ability_id == ABILITY_ID::MORPH_ARCHON || uni->orders[0].ability_id == ABILITY_ID::ARCHONWARP_ARCHONWARPTARGET)) {
            return false;
        }
        if (squad->mode == ATTACK) {
            return executeAttack(agent);
        }
        else if (squad->mode == RETREAT) {
            return executeRetreat(agent);
        }
        else if (squad->mode == DEFEND) {
            return executeDefend(agent);
        }
        else if (squad->mode == FULL_RETREAT) {
            return executeFullRetreat(agent);
        }
        else if (squad->mode == SEARCH) {
            return executeSearch(agent);
        }
        return false;
    }
};