#include "armyunit.hpp"

namespace ArmyControl {

    class EnemySquad {
    public:
        UnitWrappers army;
        std::vector<UnitTypeID> unitComp;

        EnemySquad() {

        }

        void add(UnitWrapper* unitWrap) {
            army.push_back(unitWrap);

            if (std::find(unitComp.begin(), unitComp.end(), unitWrap->type) == unitComp.end()) {
                unitComp.push_back(unitWrap->type);
            }
        }

        Point2D center(Agent *agent) {
            Point2D center;
            int count = 0;
            for (int i = 0; i < army.size(); i++) {
                center += army[i]->pos(agent);
                count++;
            }
            return center / count;
        }
    };

    using EnemySquads = std::vector<EnemySquad>;

    EnemySquads enemySquads;

	constexpr float enemyRadius = 5.0F;
	void collectEnemySquads(Agent *agent){
        enemySquads.clear();
        UnitWrappers allEnemies;
        for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
            auto all = it->second;
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                UnitWrapper* wrap = *it2;
                if (!wrap->isBuilding) {
                    allEnemies.push_back(wrap);
                }
            }
        }
        if (allEnemies.size() > 0) {
            enemySquads.emplace_back();
            enemySquads[0].add(allEnemies.back());
            allEnemies.pop_back();
            int squadPointer = 0;
            while (allEnemies.size() > 0){
                for (int i = 0; i < enemySquads[squadPointer].army.size(); i ++) {
                    UnitWrappers found = SpacialHash::findInRadiusEnemy(enemySquads[squadPointer].army[i]->pos(agent), enemyRadius, agent);
                    bool added = false;
                    if (found.size() != 0) {
                        for (UnitWrapper* enemy : found) {
                            auto find = std::find(allEnemies.begin(), allEnemies.end(), enemy);
                            if (find != allEnemies.end() &&
                                std::find(enemySquads[squadPointer].army.begin(), enemySquads[squadPointer].army.end(), enemy) == enemySquads[squadPointer].army.end()) {
                                enemySquads[squadPointer].army.push_back(enemy);
                                allEnemies.erase(find);
                                added = true;
                            }
                        }
                    }
                    if(!added && allEnemies.size() > 0) {
                        enemySquads.emplace_back();
                        squadPointer += 1;
                        enemySquads[squadPointer].add(allEnemies.back());
                        allEnemies.pop_back();
                    }
                }
            }
            printf("\n");
            for (int i = 0; i < enemySquads.size(); i++) {
                printf("%d:\n", i);
                for (int e = 0; e < enemySquads[i].army.size(); e++) {
                    printf("%s:\n", UnitTypeToName(enemySquads[i].army[e]->type));
                }
            }
            
        }
	}

	void step(Agent* agent, Point2D rally) {
        collectEnemySquads(agent);
        int unitCount = 0;
        for (Squad s : squads) {
            unitCount += s.army.size();
        }

        int enemyBuildingCount = 0;
        UnitWrappers buildings;
        for (auto it = UnitManager::enemies.begin(); it != UnitManager::enemies.end(); it++) {
            auto all = it->second;
            for (auto it2 = all.begin(); it2 != all.end(); it2++) {
                UnitWrapper* wrap = *it2;
                if (wrap->isBuilding) {
                    enemyBuildingCount += 1;
                    buildings.push_back(wrap);
                }//add break optimization if one is building all are building
            }
        }

        EnemySquads dangerous;

        for (int i = 0; i < enemySquads.size(); i++) {
            Point2D pos = enemySquads[i].center(agent);
            if (imRef(Aux::influenceMap, int(pos.x), int(pos.y)) > 0) {
                dangerous.push_back(enemySquads[i]);
            }
        }
        if (dangerous.size() > 0) {
            if (unitCount == 0) {
                squads[0].attack(dangerous[0].center(agent)); //probe defense
            }
            else {
                squads[0].attack(dangerous[0].center(agent));
            }
        }
        else if (enemyBuildingCount > 0) {
            if (squads[0].army.size() > 12) {
                if (squads[0].getCore(agent) != nullptr) {
                    float mindist = 400;
                    UnitWrapper* min = nullptr;
                    for (UnitWrapper* wrap : buildings) {
                        float dist = PrimordialStar::getPathLength(squads[0].coreCenter(agent), wrap->pos(agent), squads[0].getCore(agent)->radius, agent);
                        if (dist < mindist) {
                            min = wrap;
                            mindist = dist;
                        }
                    }
                    squads[0].attack(min->pos(agent));
                }
            }
            else {
                
            }
        }
        else {
            squads[0].search(Point2D{ 0,0 });
        }

	}
}
