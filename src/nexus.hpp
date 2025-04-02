#pragma once
#include <sc2api/sc2_api.h>

#include <map>

#include "constants.h"
#include "unit.hpp"

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
};