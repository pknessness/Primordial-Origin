#pragma once
#include <sc2api/sc2_api.h>

#include <map>

#include "constants.h"
#include "unit.hpp"

static UnitWrappers chronoBoost;

class Nexus : public UnitWrapper {
private:
public:
    Tag assimilator1;
    Tag assimilator2;

    Tag minerals[8];
    int8_t minsFound;

    Nexus(const Unit* unit) : UnitWrapper(unit), assimilator1(NullTag), assimilator2(NullTag), minsFound(0) {
        for (int i = 0; i < 8; i++) {
            minerals[i] = NullTag;
        }
    }

    void nexusNearbyUpdate(Agent* agent) {
        UnitWrappers mineralWraps = UnitManager::getMinerals();
        UnitWrappers assimilatorWraps = UnitManager::get(UNIT_TYPEID::PROTOSS_ASSIMILATOR);
        mineralWraps.insert(mineralWraps.end(), assimilatorWraps.begin(), assimilatorWraps.end());
        for (UnitWrapper* targWrap : mineralWraps) {
            Point2D targPos = targWrap->pos(agent);
            if (DistanceSquared2D(targPos, pos(agent)) < 100) {
                nexusNearby[targWrap->self] = true;
                minerals[minsFound++] = targWrap->self;
            }
        }
    }

    static void addChrono(UnitWrapper* t) {
        chronoBoost.push_back(t);
    }

    bool execute(Agent* agent) {
        if (chronoBoost.size() > 0) {
            if (get(agent)->energy >= Aux::unitAbilityToCost(ABILITY_ID::EFFECT_CHRONOBOOSTENERGYCOST, agent).energy) {
                vector<BuffID> buffs = chronoBoost[0]->get(agent)->buffs;
                if (std::find(buffs.begin(), buffs.end(), BUFF_ID::CHRONOBOOSTENERGYCOST) == buffs.end()) {
                    agent->Actions()->UnitCommand(self, ABILITY_ID::EFFECT_CHRONOBOOSTENERGYCOST, chronoBoost[0]->self);
                    chronoBoost.erase(chronoBoost.begin());
                }

            }
        }
        return false;
    }
};