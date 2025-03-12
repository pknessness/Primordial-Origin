#pragma once
#include <sc2api/sc2_api.h>

#include <map>

#include "constants.h"
#include "unit.hpp"

class Assimilator : public UnitWrapper {
private:
public:
    Assimilator(const Unit* unit) : UnitWrapper(unit)  {
FUNC_START

    }

    void nexusNearbyUpdate(Agent* agent)  {
FUNC_START
        UnitWrappers nexuses = UnitManager::get(UNIT_TYPEID::PROTOSS_NEXUS);

        for (UnitWrapper* nexusWrap : nexuses) {
            Point2D targPos = nexusWrap->pos(agent);
            if (DistanceSquared2D(targPos, pos(agent)) < 100) {
                nexusNearby[self] = true;
            }
        }
    }
};