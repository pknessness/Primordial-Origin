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
    using Strategy = std::vector<MacroAction>;

    std::vector<MacroAction> shit_stalker_colossus;

    void initStrategies() {
        shit_stalker_colossus = {
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
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_GATEWAY, ABILITY_ID::TRAIN_ZEALOT),
            MacroAction(UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY, ABILITY_ID::TRAIN_WARPPRISM)
        };
    }
}