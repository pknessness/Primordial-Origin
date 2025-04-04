#pragma once
#include <sc2api/sc2_api.h>

#include <map>

#include "constants.h"
#include "unit.hpp"

class Assimilator : public UnitWrapper {
private:
    UnitWrapper* vespene;
    UnitWrapper* nexus;
public:
    

    Assimilator(const Unit* unit) : UnitWrapper(unit), vespene(nullptr){

    }

    UnitWrapper* getVespene(Agent* agent) {
        if (vespene == nullptr) {
            UnitWrappers vesps = UnitManager::getVespene();
            for (int i = 0; i < vesps.size(); i++) {
                if (DistanceSquared2D(pos(agent), vesps[i]->pos(agent)) < 2) {
                    vespene = vesps[i];
                    break;
                }
            }
            printf("DIDN'T FIND VESPENE\n");
        }
        return vespene;
    }

    void nexusNearbyUpdate(Agent* agent) {
        UnitWrappers nexuses = UnitManager::get(UNIT_TYPEID::PROTOSS_NEXUS);

        for (UnitWrapper* nexusWrap : nexuses) {
            Point2D targPos = nexusWrap->pos(agent);
            //printf("Assimilator is sqrt(%.1f) away\n", DistanceSquared2D(targPos, pos(agent)));
            if (DistanceSquared2D(targPos, pos(agent)) < 100) {
                nexusNearby[self] = true;
                nexus = nexusWrap;
                if (((Nexus*)nexusWrap)->assimilator1 == NullTag) {
                    ((Nexus*)nexusWrap)->assimilator1 = self;
                }else if (((Nexus*)nexusWrap)->assimilator2 == NullTag) {
                    ((Nexus*)nexusWrap)->assimilator2 = self;
                }
                else {
                    printf("TRIPLE ASSIMILATOR WTF\n");
                }
                break;
            }
        }
    }

    bool execute(Agent* agent) {
        if (getVespene(agent)->get(agent)->vespene_contents == 0 && nexus != nullptr) {
            if (((Nexus*)nexus)->assimilator1 == self) {
                ((Nexus*)nexus)->assimilator1 = NullTag;
            }else if (((Nexus*)nexus)->assimilator2 == self) {
                ((Nexus*)nexus)->assimilator2 = NullTag;
            } else {
                printf("NOT FOUND SELF ASSIMILATOR ON NEXUS\n");
            }
        }
        return false;
    }
};