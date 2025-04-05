#include "macro.hpp"

struct MacroBuilding : MacroAction {

    MacroBuilding(AbilityID ability_, Point2D pos_) : MacroAction(UNIT_TYPEID::PROTOSS_PROBE, ability_, pos_) {

    }
};

MacroAction MacroBuilding(AbilityID ability_, Point2D pos_) {
    return MacroAction(UNIT_TYPEID::PROTOSS_PROBE, ability_, pos_);
}
//MacroAction Action(UnitTypeID unit_type_, AbilityID ability_, Point2D pos_) {
//    return MacroAction(unit_type_, ability_, pos_);
//}

namespace Strategem {
    //using Strategy = std::vector<MacroAction>;
    struct UnitRatio {
        int8_t zealot = 0;
        int8_t stalker = 0;
        int8_t sentry = 0;
        int8_t adept = 0;
        int8_t darktemplar = 0;
        int8_t hightemplar = 0;
        int8_t archon = 0;

        int8_t observer = 0;
        int8_t immortal = 0;
        int8_t warpprism = 0;
        int8_t colossus = 0;
        int8_t disruptor = 0;

        int8_t pheonix = 0;
        int8_t oracle = 0;
        int8_t voidray = 0;
        int8_t tempest = 0;
        int8_t carrier = 0;
        int8_t mothership = 0;
    };

    struct UnitRatioFloat {
        float zealot = 0;
        float stalker = 0;
        float sentry = 0;
        float adept = 0;
        float darktemplar = 0;
        float hightemplar = 0;
        float archon = 0;

        float observer = 0;
        float immortal = 0;
        float warpprism = 0;
        float colossus = 0;
        float disruptor = 0;

        float pheonix = 0;
        float oracle = 0;
        float voidray = 0;
        float tempest = 0;
        float carrier = 0;
        float mothership = 0;
    };

    struct Strategy {
        std::vector<MacroAction> build_order;
        UnitRatio unitRatio;
        int armyAttackNum = 12;
    };

    Strategy shit_stalker_colossus;
    Strategy pig_stalker_colossus; //https://lotv.spawningtool.com/build/184604/
    Strategy chargelot_immortal_archon_timing; //https://lotv.spawningtool.com/build/188377/
    Strategy hupsaiya_adept_timing; //https://lotv.spawningtool.com/build/190069/
    Strategy zuka_colossus_voidray;

    Strategy classic_gsl_tempest_rush;//https://lotv.spawningtool.com/build/97659/
    Strategy zuka_proxy_tempest;//https://lotv.spawningtool.com/build/150094/
    Strategy chirartem_proxy_dt;//https://lotv.spawningtool.com/build/187995/

    Strategy gemini_chargelot_archon_disruptor;//https://lotv.spawningtool.com/build/187138/

    void initStrategies() {
        shit_stalker_colossus.build_order = {
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSBAY, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_BLINK),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSBAY, ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM),
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[1])),
        };
        shit_stalker_colossus.unitRatio.stalker = 8;
        shit_stalker_colossus.unitRatio.immortal = 4;
        shit_stalker_colossus.unitRatio.colossus = 2;
        shit_stalker_colossus.unitRatio.observer = 1;
        shit_stalker_colossus.unitRatio.warpprism = 1;

        pig_stalker_colossus.build_order = {
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])),
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSBAY, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSBAY, ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, { -1,-1 }),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_SENTRY),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_SENTRY),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_SENTRY),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM)
        };
        pig_stalker_colossus.unitRatio.stalker = 8;
        pig_stalker_colossus.unitRatio.colossus = 2;
        pig_stalker_colossus.unitRatio.observer = 1;
        pig_stalker_colossus.unitRatio.warpprism = 1;
        
        chargelot_immortal_archon_timing.build_order = {
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])),
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_SENTRY),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL),
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_FORGE, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_CHARGE),
            MacroAction(UNIT_TYPEID::PROTOSS_FORGE, ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONS),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_TEMPLARARCHIVE, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSBAY, ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE)
        };
        chargelot_immortal_archon_timing.unitRatio.immortal = 4;
        chargelot_immortal_archon_timing.unitRatio.zealot = 20;
        chargelot_immortal_archon_timing.unitRatio.archon = 8;
        chargelot_immortal_archon_timing.unitRatio.observer = 1;
        chargelot_immortal_archon_timing.unitRatio.warpprism = 1;
        chargelot_immortal_archon_timing.armyAttackNum = 19;

        
        hupsaiya_adept_timing.build_order = {
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_ADEPTRESONATINGGLAIVES),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}), //Proxy Pylon near their 3rd base
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT),
        };
        hupsaiya_adept_timing.unitRatio.adept = 1; 
        hupsaiya_adept_timing.armyAttackNum = 1; //Hit at 4:37 with 14 Adepts

        zuka_colossus_voidray.build_order = {
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)), // 14 probes
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}), // 15 probes
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}), // 16 probes
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])), // 20 probes
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1,-1}), // 20 probes
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}), // 21 probes
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}), // 22 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 22 probes
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE), // 23 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 26 probes
            MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}), // 29 probes
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}), // 33 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 33 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_ORACLE), // 37 probes
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}), // 38 probes
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}), // 43 probes
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}), // 43 probes
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1,-1}), // 44 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 46 probes
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}), // 46 probes
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}), // 51 probes
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSBAY, {-1,-1}), // 52 probes
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}), // 53 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER), // 53 probes
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}), // 53 probes
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, {-1,-1}), // 57 probes
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}), // 59 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSBAY, ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE), // 59 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL), // 60 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 66 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 66 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 66 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS), // 66 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER), // 78 probes
            MacroBuilding(ABILITY_ID::BUILD_FORGE, {-1,-1}), // 86 probes
            MacroBuilding(ABILITY_ID::BUILD_FORGE, {-1,-1}), // 86 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 88 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 88 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 88 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS), // 96 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 97 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 97 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 97 probes
            MacroAction(UNIT_TYPEID::PROTOSS_FORGE, ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL1), // 97 probes
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}), // 101 probes
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}), // 101 probes
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}), // 101 probes
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}), // 104 probes
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}), // 104 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS), // 107 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 113 probes
            MacroBuilding(ABILITY_ID::BUILD_FORGE, {-1,-1}), // 104 probes
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, {-1,-1}), // 107 probes
            MacroBuilding(ABILITY_ID::BUILD_FORGE, {-1,-1}), // 107 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 107 probes
            MacroAction(UNIT_TYPEID::PROTOSS_FORGE, ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL2), // 110 probes
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}), // 110 probes
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}), // 110 probes
            MacroAction(UNIT_TYPEID::PROTOSS_FORGE, ABILITY_ID::RESEARCH_PROTOSSGROUNDARMORLEVEL1), // 110 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER), // 110 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 116 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 116 probes
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL), // 122 probes
            MacroAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_CHARGE), // 122 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 122 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 122 probes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 122 probes
            MacroBuilding(ABILITY_ID::BUILD_TEMPLARARCHIVE, {-1,-1}), // 122 probes
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 126 probes
        };
        zuka_colossus_voidray.unitRatio.stalker = 10;
        zuka_colossus_voidray.unitRatio.colossus = 4;
        zuka_colossus_voidray.unitRatio.voidray = 6;
        zuka_colossus_voidray.unitRatio.observer = 1;
        zuka_colossus_voidray.unitRatio.immortal = 2;
        zuka_colossus_voidray.armyAttackNum = 16; //Adjust timing and army composition as needed

        classic_gsl_tempest_rush.build_order = {
            // Early Economy
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),  // 14 probes @ 0:18
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 15 probes @ 0:36
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 17 probes @ 0:48
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 18 probes @ 0:56

            // Defensive Setup
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),        // 19 probes @ 1:05
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1,-1}),          // 20 probes @ 1:24

            // Early Units
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT, {0,0}, true), // 23 probes @ 1:59 (Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE), // 25 probes @ 2:01

            // Expansion Phase
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT), // 26 probes @ 2:19
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 26 probes @ 2:19
            MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}),                 // 28 probes @ 2:29
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])),                    // 29 probes @ 2:46
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT), // 30 probes @ 2:50

            // Tech Transition
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 32 probes @ 2:58
            MacroBuilding(ABILITY_ID::BUILD_FLEETBEACON, {-1,-1}),              // 33 probes @ 3:16
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 34 probes @ 3:29

            // Tempest Production (All Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 38 probes @ 4:00
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),    // 43 probes @ 4:05
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 45 probes @ 4:20
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 47 probes @ 4:30

            // Forward Pressure
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),  // 54 probes @ 4:50
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 54 probes @ 4:55
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST), // 56 probes @ 5:00 (MOVE OUT)
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}), // 63 probes @ 5:10

            //MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}),
            //MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}),
        };
        classic_gsl_tempest_rush.unitRatio.adept = 3;
        classic_gsl_tempest_rush.unitRatio.tempest = 3;
        classic_gsl_tempest_rush.armyAttackNum = 6;  // Hit at 5:00 with 3 Tempests

        zuka_proxy_tempest.build_order = {
            // ===== Early Economy ===== (0:00-1:21)
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),  // 12 probes @ 0:08
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 13 probes @ 0:26
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 14 probes @ 0:41
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 16 probes @ 0:51
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1,-1}),          // 18 probes @ 1:14
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::enemyRankedExpansions[3])),                    // 18 probes @ 1:21

            // ===== Tech Transition ===== (1:21-2:26)
            MacroBuilding(ABILITY_ID::BUILD_STARGATE, P2D(Aux::enemyRankedExpansions[3]) + Point2D{2.5, 0.5}),                 // 20 probes @ 1:51
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT, {0,0}, true), // 21 probes @ 1:54 (Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT), // 25 probes @ 2:17
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::enemyRankedExpansions[3]) + Point2D{-2, -2}),                    // 26 probes @ 2:23
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY, {0,0}, true), // 26 probes @ 2:26 (Chrono)

            // ===== Defensive Setup ===== (2:26-3:57)
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, P2D(Aux::enemyRankedExpansions[3]) + Point2D{-2, 0}),            // 30 probes @ 2:42 (x2)
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, P2D(Aux::enemyRankedExpansions[3]) + Point2D{-2, 2}),            // 30 probes @ 2:42 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY), // 30 probes @ 2:57
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, P2D(Aux::enemyRankedExpansions[3]) + Point2D{0, 2}),            // 34 probes @ 3:09
            MacroBuilding(ABILITY_ID::BUILD_FLEETBEACON, {-1,-1}),              // 34 probes @ 3:28
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, P2D(Aux::enemyRankedExpansions[3]) + Point2D{0, -2}),            // 34 probes @ 3:57

            // ===== Tempest Transition ===== (4:12-5:02)
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 35 probes @ 4:12
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),            // 36 probes @ 4:24
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 36 probes @ 4:29 (Chrono)
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),            // 36 probes @ 4:30
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),            // 41 probes @ 4:39
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE), // 41 probes @ 4:48
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 41 probes @ 4:49
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 41 probes @ 5:02 (Chrono)

            // ===== Mid-Game Push Prep ===== (5:21-6:16)
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 46 probes @ 5:21
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),            // 46 probes @ 5:24
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 42 probes @ 5:38 (Chrono)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 47 probes @ 5:45
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 43 probes @ 6:12
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 43 probes @ 6:16 (Chrono)

            // ===== Late-Game Army ===== (6:38-7:57)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 48 probes @ 6:38 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 48 probes @ 6:38 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_TEMPEST, {0,0}, true), // 52 probes @ 7:01 (Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 57 probes @ 7:25 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 57 probes @ 7:25 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 57 probes @ 7:25 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 57 probes @ 7:25 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 61 probes @ 7:57 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 61 probes @ 7:57 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 61 probes @ 7:57 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER)  // 61 probes @ 7:57 (x4)
        };
        zuka_proxy_tempest.unitRatio.adept = 2;
        zuka_proxy_tempest.unitRatio.voidray = 2;
        zuka_proxy_tempest.unitRatio.tempest = 5;
        zuka_proxy_tempest.unitRatio.stalker = 10;
        zuka_proxy_tempest.armyAttackNum = 1;

        chirartem_proxy_dt.build_order = {
            // Early Economy
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),  // 14 probes @ 0:17
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 16 probes @ 0:36 (Chrono probes)
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 17 probes @ 0:46
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 18 probes @ 0:55 (Chrono probes)

            // Proxy Setup
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1,-1}),          // 21 probes @ 1:23
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::enemyRankedExpansions[3])),    // 22 probes @ 1:31 (Proxy)
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, P2D(Aux::enemyRankedExpansions[3]) + Point2D{2.5, 0.5}), // 24 probes @ 1:59 (Proxy)
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE), // 24 probes @ 2:00
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 24 probes @ 2:02

            // Tech Rush
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, P2D(Aux::enemyRankedExpansions[3]) + Point2D{-0.5, 2.5}), // 25 probes @ 2:12 (Proxy)
            MacroBuilding(ABILITY_ID::BUILD_DARKSHRINE, P2D(Aux::enemyRankedExpansions[3]) + Point2D{-2, 0}), // 25 probes @ 2:37 (Proxy)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 25 probes @ 2:44 (x2)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM, {0,0}, true), // 25 probes @ 2:59 (Chrono)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 25 probes @ 3:07
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 25 probes @ 3:17
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 25 probes @ 3:21

            // DT Production
            MacroAction(UNIT_TYPEID::PROTOSS_WARPGATE, ABILITY_ID::TRAIN_DARKTEMPLAR), // 25 probes @ 3:49 (x3)
            MacroAction(UNIT_TYPEID::PROTOSS_WARPGATE, ABILITY_ID::TRAIN_DARKTEMPLAR),
            MacroAction(UNIT_TYPEID::PROTOSS_WARPGATE, ABILITY_ID::TRAIN_DARKTEMPLAR)
        };
        chirartem_proxy_dt.unitRatio.stalker = 1;
        chirartem_proxy_dt.unitRatio.darktemplar = 3;
        chirartem_proxy_dt.unitRatio.warpprism = 1;
        chirartem_proxy_dt.armyAttackNum = 4;  // Hit with 3 DTs + Warp Prism

        gemini_chargelot_archon_disruptor.build_order = {
            // Early Economy
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),  // 14 probes @ 0:18
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 15 probes @ 0:38
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 16 probes @ 0:47

            // Expansion
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])), // 20 probes @ 1:24
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1,-1}),          // 20 probes @ 1:34

            // Tech Setup
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 21 probes @ 1:45
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 21 probes @ 1:53
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT, {0,0}, true), // 22 probes @ 2:08 (Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE), // 23 probes @ 2:11

            // Air Transition
            MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}),                 // 28 probes @ 2:31
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER), // 29 probes @ 2:39
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ADEPT), // 37 probes @ 3:09

            // Mid-Game
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 39 probes @ 3:17
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_VOIDRAY, {0,0}, true), // 39 probes @ 3:17 (Chrono)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 45 probes @ 3:28
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 46 probes @ 3:35
            MacroAction(UNIT_TYPEID::PROTOSS_STARGATE, ABILITY_ID::TRAIN_ORACLE, {0,0}, true), // 47 probes @ 3:45 (Chrono)

            // Tech Expansion
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[1])), // 53 probes @ 4:01
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),                    // 53 probes @ 4:03
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 55 probes @ 4:15 (x2)
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),            // 57 probes @ 4:26
            MacroBuilding(ABILITY_ID::BUILD_FORGE, {-1,-1}),                    // 57 probes @ 4:29
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, {-1,-1}),          // 57 probes @ 4:29

            // Army Composition
            MacroAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_CHARGE, {0,0}, true), // 61 probes @ 4:53 (Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_FORGE, ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL1, {0,0}, true), // 61 probes @ 4:54 (Chrono)
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 61 probes @ 4:55 (x4)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),

            // Late-Game Transitions
            MacroBuilding(ABILITY_ID::BUILD_TEMPLARARCHIVE, {-1,-1}),          // 63 probes @ 5:12
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),                  // 67 probes @ 5:28 (x2)
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 69 probes @ 5:39 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON), // 76 probes @ 5:53 (x4)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON), // 87 probes @ 5:58 (x2)
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),

            // Robotics Transition
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1,-1}),          // 87 probes @ 6:00
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 87 probes @ 6:06 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),

            // Late-Game Economy
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 91 probes @ 6:13 (x2)
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1,-1}),

            // Army Spikes
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 94 probes @ 6:24 (x5)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT), // 96 probes @ 6:31

            // Support Units
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_SENTRY), // 117 probes @ 6:55 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_SENTRY),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER), // 117 probes @ 6:56
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[2])), // 117 probes @ 6:57

            // Upgrades and Tech
            MacroAction(UNIT_TYPEID::PROTOSS_FORGE, ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL2, {0,0}, true), // 125 probes @ 7:03 (Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON), // 125 probes @ 7:13 (x2)

            // Heavy Units
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL, {0,0}, true), // 129 probes @ 7:22 (Chrono)
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1,-1}),          // 132 probes @ 7:28
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSBAY, {-1,-1}),               // 136 probes @ 7:33

            // Defensive Structures
            MacroBuilding(ABILITY_ID::BUILD_PHOTONCANNON, {-1,-1}),             // 136 probes @ 7:41 (x4)
            MacroBuilding(ABILITY_ID::BUILD_PHOTONCANNON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PHOTONCANNON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_PHOTONCANNON, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_SHIELDBATTERY, {-1,-1}),            // 136 probes @ 7:47

            // Late-Game Army
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_HIGHTEMPLAR), // 138 probes @ 8:04 (x2)
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_HIGHTEMPLAR),

            // Final Economy
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),              // 152 probes @ 8:12 (x2)
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),

            // Disruptor Tech
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_DISRUPTOR, {0,0}, true), // 152 probes @ 8:16 (x2 Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_DISRUPTOR, {0,0}, true),
            MacroBuilding(ABILITY_ID::BUILD_PHOTONCANNON, {-1,-1}),             // 152 probes @ 8:17 (x2)
            MacroBuilding(ABILITY_ID::BUILD_PHOTONCANNON, {-1,-1}),

            // Archon Production
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ARCHON), // 156 probes @ 8:24 (x2)

            // Final Push Composition
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_DISRUPTOR, {0,0}, true), // 164 probes @ 8:42 (x2 Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_DISRUPTOR, {0,0}, true),

            // Sky Transition
            MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}),                 // 180 probes @ 9:03 (x2)
            MacroBuilding(ABILITY_ID::BUILD_STARGATE, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_FLEETBEACON, {-1,-1}),              // 180 probes @ 9:06
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_DISRUPTOR, {0,0}, true),  // 180 probes @ 9:12 (x2 Chrono)
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_DISRUPTOR, {0,0}, true),
        };
        gemini_chargelot_archon_disruptor.unitRatio.adept = 2;
        gemini_chargelot_archon_disruptor.unitRatio.stalker = 1;
        gemini_chargelot_archon_disruptor.unitRatio.voidray = 2;
        gemini_chargelot_archon_disruptor.unitRatio.oracle = 1;
        gemini_chargelot_archon_disruptor.unitRatio.zealot = 10;
        gemini_chargelot_archon_disruptor.unitRatio.archon = 5;
        gemini_chargelot_archon_disruptor.unitRatio.immortal = 1;
        gemini_chargelot_archon_disruptor.unitRatio.disruptor = 4;
        gemini_chargelot_archon_disruptor.unitRatio.observer = 1;
        gemini_chargelot_archon_disruptor.armyAttackNum = 8;  // First push at 5:58 with 2 Archons
    }
}