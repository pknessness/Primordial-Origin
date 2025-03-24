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
        UNIT_TYPEID::PROTOSS_WARPPRISM, //PHASED
        UNIT_TYPEID::PROTOSS_COLOSSUS,
        UNIT_TYPEID::PROTOSS_DISRUPTOR,

        UNIT_TYPEID::PROTOSS_PHOENIX,
        UNIT_TYPEID::PROTOSS_ORACLE,
        UNIT_TYPEID::PROTOSS_VOIDRAY,
        UNIT_TYPEID::PROTOSS_TEMPEST,
        UNIT_TYPEID::PROTOSS_CARRIER,

        UNIT_TYPEID::PROTOSS_MOTHERSHIP
    };

    UnitTypeID ArmyUnitsProtoss[] = {
        UNIT_TYPEID::PROTOSS_ZEALOT,
        UNIT_TYPEID::PROTOSS_STALKER,
        UNIT_TYPEID::PROTOSS_SENTRY,
        UNIT_TYPEID::PROTOSS_ADEPT,
        UNIT_TYPEID::PROTOSS_DARKTEMPLAR,
        UNIT_TYPEID::PROTOSS_HIGHTEMPLAR,
        UNIT_TYPEID::PROTOSS_ARCHON,

        UNIT_TYPEID::PROTOSS_OBSERVER,
        UNIT_TYPEID::PROTOSS_IMMORTAL,
        UNIT_TYPEID::PROTOSS_WARPPRISM, //PHASED
        UNIT_TYPEID::PROTOSS_COLOSSUS,
        UNIT_TYPEID::PROTOSS_DISRUPTOR,

        UNIT_TYPEID::PROTOSS_PHOENIX,
        UNIT_TYPEID::PROTOSS_ORACLE,
        UNIT_TYPEID::PROTOSS_VOIDRAY,
        UNIT_TYPEID::PROTOSS_TEMPEST,
        UNIT_TYPEID::PROTOSS_CARRIER,

        UNIT_TYPEID::PROTOSS_MOTHERSHIP,

        UNIT_TYPEID::PROTOSS_PROBE,
        UNIT_TYPEID::PROTOSS_PHOTONCANNON,
        UNIT_TYPEID::PROTOSS_SHIELDBATTERY,
    };

    UnitTypeID ArmyUnitsTerran[] = {
        UNIT_TYPEID::TERRAN_MARINE,
        UNIT_TYPEID::TERRAN_MARAUDER,
        UNIT_TYPEID::TERRAN_REAPER,

        UNIT_TYPEID::TERRAN_GHOST,

        UNIT_TYPEID::TERRAN_HELLION, //TERRAN_HELLIONTANK
        UNIT_TYPEID::TERRAN_WIDOWMINE,
        UNIT_TYPEID::TERRAN_CYCLONE,
        UNIT_TYPEID::TERRAN_SIEGETANK, //TERRAN_SIEGETANKSIEGED
        
        UNIT_TYPEID::TERRAN_THOR, //TERRAN_THORAP
        
        UNIT_TYPEID::TERRAN_VIKINGFIGHTER, //TERRAN_VIKINGASSAULT
        UNIT_TYPEID::TERRAN_MEDIVAC,
        UNIT_TYPEID::TERRAN_LIBERATOR, //TERRAN_LIBERATORAG
        UNIT_TYPEID::TERRAN_RAVEN,
        UNIT_TYPEID::TERRAN_AUTOTURRET,
        UNIT_TYPEID::TERRAN_BANSHEE,
        
        UNIT_TYPEID::TERRAN_BATTLECRUISER,

        UNIT_TYPEID::TERRAN_SCV,
        UNIT_TYPEID::TERRAN_MULE,
        UNIT_TYPEID::TERRAN_MISSILETURRET,
        UNIT_TYPEID::TERRAN_PLANETARYFORTRESS,
        UNIT_TYPEID::TERRAN_BUNKER,
        UNIT_TYPEID::TERRAN_KD8CHARGE,
    };

    UnitTypeID ArmyUnitsZerg[] = {  //BURROWED, COCOON
        UNIT_TYPEID::ZERG_ZERGLING, 
        UNIT_TYPEID::ZERG_QUEEN,

        UNIT_TYPEID::ZERG_ROACH,
        UNIT_TYPEID::ZERG_RAVAGER,

        UNIT_TYPEID::ZERG_BANELING,

        UNIT_TYPEID::ZERG_HYDRALISK,

        UNIT_TYPEID::ZERG_INFESTOR,
        UNIT_TYPEID::ZERG_SWARMHOSTMP,
        UNIT_TYPEID::ZERG_LOCUSTMP,

        UNIT_TYPEID::ZERG_MUTALISK,
        UNIT_TYPEID::ZERG_CORRUPTOR,

        UNIT_TYPEID::ZERG_LURKERMP,
        UNIT_TYPEID::ZERG_LURKERMPBURROWED,

        UNIT_TYPEID::ZERG_VIPER,

        UNIT_TYPEID::ZERG_NYDUSCANAL,

        UNIT_TYPEID::ZERG_ULTRALISK,

        UNIT_TYPEID::ZERG_BROODLORD,
        UNIT_TYPEID::ZERG_BROODLING,

        UNIT_TYPEID::ZERG_DRONE,
        UNIT_TYPEID::ZERG_OVERLORD,
        UNIT_TYPEID::ZERG_OVERLORDTRANSPORT,
        UNIT_TYPEID::ZERG_OVERSEER,

        UNIT_TYPEID::ZERG_SPINECRAWLER,
        UNIT_TYPEID::ZERG_SPORECRAWLER,
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
        UNIT_TYPEID::PROTOSS_GATEWAY,

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
        int armyAttackNum = 12;
    };

    Strategy shit_stalker_colossus;
    Strategy pig_stalker_colossus;
    Strategy chargelot_immortal_archon_timing;
    Strategy hupsaiya_adept_timing;
    Strategy zuka_colossus_voidray;

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

        //https://lotv.spawningtool.com/build/184604/
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

        //https://lotv.spawningtool.com/build/188377/
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

        //https://lotv.spawningtool.com/build/190069/
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
    }
}