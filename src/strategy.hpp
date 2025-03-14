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

    UnitTypeID UnitOrder[18] = {
        UNIT_TYPEID::PROTOSS_ZEALOT,
        UNIT_TYPEID::PROTOSS_STALKER,
        UNIT_TYPEID::PROTOSS_SENTRY,
        UNIT_TYPEID::PROTOSS_ADEPT,
        UNIT_TYPEID::PROTOSS_DARKTEMPLAR,
        UNIT_TYPEID::PROTOSS_HIGHTEMPLAR,
        UNIT_TYPEID::PROTOSS_ARCHON,

        UNIT_TYPEID::PROTOSS_OBSERVER,
        UNIT_TYPEID::PROTOSS_IMMORTAL,
        UNIT_TYPEID::PROTOSS_WARPPRISM,
        UNIT_TYPEID::PROTOSS_COLOSSUS,
        UNIT_TYPEID::PROTOSS_DISRUPTOR,

        UNIT_TYPEID::PROTOSS_PHOENIX,
        UNIT_TYPEID::PROTOSS_ORACLE,
        UNIT_TYPEID::PROTOSS_VOIDRAY,
        UNIT_TYPEID::PROTOSS_TEMPEST,
        UNIT_TYPEID::PROTOSS_CARRIER,

        UNIT_TYPEID::PROTOSS_MOTHERSHIP
    };

    AbilityID UnitCreationAbility[18] = {
        ABILITY_ID::TRAIN_ZEALOT,
        ABILITY_ID::TRAIN_STALKER,
        ABILITY_ID::TRAIN_SENTRY,
        ABILITY_ID::TRAIN_ADEPT,
        ABILITY_ID::TRAIN_DARKTEMPLAR,
        ABILITY_ID::TRAIN_HIGHTEMPLAR,
        ABILITY_ID::TRAIN_ARCHON,

        ABILITY_ID::TRAIN_OBSERVER,
        ABILITY_ID::TRAIN_IMMORTAL,
        ABILITY_ID::TRAIN_WARPPRISM,
        ABILITY_ID::TRAIN_COLOSSUS,
        ABILITY_ID::TRAIN_DISRUPTOR,

        ABILITY_ID::TRAIN_PHOENIX,
        ABILITY_ID::TRAIN_ORACLE,
        ABILITY_ID::TRAIN_VOIDRAY,
        ABILITY_ID::TRAIN_TEMPEST,
        ABILITY_ID::TRAIN_CARRIER,

        ABILITY_ID::TRAIN_MOTHERSHIP
    };

    UnitTypeID UnitCreators[18] = {
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_ARCHON,

        UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY,
        UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY,
        UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY,
        UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY,
        UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY,

        UNIT_TYPEID::PROTOSS_STARGATE,
        UNIT_TYPEID::PROTOSS_STARGATE,
        UNIT_TYPEID::PROTOSS_STARGATE,
        UNIT_TYPEID::PROTOSS_STARGATE,
        UNIT_TYPEID::PROTOSS_STARGATE,

        UNIT_TYPEID::PROTOSS_NEXUS
    };

    struct Strategy {
        std::vector<MacroAction> build_order;
        UnitRatio unitRatio;
    };

    Strategy shit_stalker_colossus;
    Strategy pig_stalker_colossus;
    Strategy chargelot_immortal_archon_timing;

    void initStrategies() {
        shit_stalker_colossus.build_order = {
            MacroBuilding(ABILITY_ID::BUILD_PYLON, P2D(Aux::staging_location)),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1,-1}),
            MacroBuilding(ABILITY_ID::GENERAL_MOVE, Aux::enemyLoc),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, {-1,-1}),
            MacroBuilding(ABILITY_ID::BUILD_CYBERNETICSCORE, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_CYBERNETICSCORE, ABILITY_ID::RESEARCH_WARPGATE),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSBAY, ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE),
            MacroBuilding(ABILITY_ID::BUILD_NEXUS, P2D(Aux::rankedExpansions[0])),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_PYLON, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSFACILITY, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_OBSERVER),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_IMMORTAL),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_GATEWAY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_TWILIGHTCOUNCIL, {-1, -1}),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_STALKER),
            MacroAction(UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL, ABILITY_ID::RESEARCH_BLINK),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroBuilding(ABILITY_ID::BUILD_ROBOTICSBAY, {-1, -1}),
            MacroBuilding(ABILITY_ID::BUILD_ASSIMILATOR, { -1,-1 }),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_COLOSSUS),
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
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM)
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
    }
}