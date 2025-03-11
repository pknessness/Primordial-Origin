#pragma once
#include <sc2api/sc2_api.h>

#include <map>

#include "constants.h"
#include "unit.hpp"

class Nexus : public UnitWrapper {
private:
    UnitTypeID type1;
    Tag vespene1;
    bool taken1;

    UnitTypeID type2;
    Tag vespene2;
    bool taken2;
public:
    Nexus(const Unit* unit) : UnitWrapper(unit), type1(UNIT_TYPEID::INVALID), vespene1(NullTag), taken1(false), type2(UNIT_TYPEID::INVALID), vespene2(NullTag), taken2(false) {

    }

    void nexusNearbyUpdate(Agent* agent) {
        UnitWrappers mineralWraps = UnitManager::getMinerals();
        UnitWrappers assimilatorWraps = UnitManager::get(UNIT_TYPEID::PROTOSS_ASSIMILATOR);
        mineralWraps.insert(mineralWraps.end(), assimilatorWraps.begin(), assimilatorWraps.end());
        for (UnitWrapper* targWrap : mineralWraps) {
            Point2D targPos = targWrap->pos(agent);
            if (DistanceSquared2D(targPos, pos(agent)) < 100) {
                nexusNearby[targWrap->self] = true;
            }
        }
    }
};