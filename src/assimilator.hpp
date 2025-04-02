#pragma once
#include <sc2api/sc2_api.h>

#include <map>

#include "constants.h"
#include "unit.hpp"

class Assimilator : public UnitWrapper {
private:
public:
    Assimilator(const Unit* unit) : UnitWrapper(unit){

    }

    void nexusNearbyUpdate(Agent* agent) {
        UnitWrappers nexuses = UnitManager::get(UNIT_TYPEID::PROTOSS_NEXUS);

        for (UnitWrapper* nexusWrap : nexuses) {
            Point2D targPos = nexusWrap->pos(agent);
            printf("Assimilator is sqrt(%.1f) away\n", DistanceSquared2D(targPos, pos(agent)));
            if (DistanceSquared2D(targPos, pos(agent)) < 100) {
                nexusNearby[self] = true;
                if (((Nexus*)nexusWrap)->assimilator1 == NullTag) {
                    ((Nexus*)nexusWrap)->assimilator1 = self;
                }else if (((Nexus*)nexusWrap)->assimilator2 == NullTag) {
                    ((Nexus*)nexusWrap)->assimilator2 = self;
                }
                else {
                    printf("TRIPLE ASSIMILATOR WTF\n");
                }
            }
        }
    }
};