// The MIT License (MIT)
//
// Copyright (c) 2021-2024 Alexander Kurbatov
//#undef byte
#define BACKWARD_HAS_BFD 1
#include "backward/backward.hpp"
#include "Bot.hpp"

#include <sc2api/sc2_coordinator.h>
#include <sc2api/sc2_gametypes.h>
#include <sc2utils/sc2_arg_parser.h>

#include <iostream>

#define MICRO_TEST 0
#define START_ME Point2D{40,50}
#define START_OP Point2D{60,50}
//#define BUILD_FOR_LADDER

#ifdef BUILD_FOR_LADDER
namespace
{

struct Options
{
    Options(): GamePort(0), StartPort(0)
    {}

    int32_t GamePort;
    int32_t StartPort;
    std::string ServerAddress;
    std::string OpponentId;
};

void ParseArguments(int argc, char* argv[], Options* options_)
{
    sc2::ArgParser arg_parser(argv[0]);
    arg_parser.AddOptions(
        {
            {"-g", "--GamePort", "Port of client to connect to", false},
            {"-o", "--StartPort", "Starting server port", false},
            {"-l", "--LadderServer", "Ladder server address", false},
            {"-x", "--OpponentId", "PlayerId of opponent", false},
        });


    arg_parser.Parse(argc, argv);

    std::string GamePortStr;
    if (arg_parser.Get("GamePort", GamePortStr))
        options_->GamePort = atoi(GamePortStr.c_str());

    std::string StartPortStr;
    if (arg_parser.Get("StartPort", StartPortStr))
        options_->StartPort = atoi(StartPortStr.c_str());

    std::string OpponentId;
    if (arg_parser.Get("OpponentId", OpponentId))
        options_->OpponentId = OpponentId;

    arg_parser.Get("LadderServer", options_->ServerAddress);
}

}  // namespace

int main(int argc, char* argv[])
{
    backward::SignalHandling sh;
    //Options options;
    //ParseArguments(argc, argv, &options);

    //sc2::Coordinator coordinator;
    //Bot bot;

    //size_t num_agents = 2;
    //coordinator.SetParticipants({ CreateParticipant(sc2::Race::Random, &bot, "BlankBot") });

    //std::cout << "Connecting to port " << options.GamePort << std::endl;
    //coordinator.Connect(options.GamePort);
    //coordinator.SetupPorts(num_agents, options.StartPort, false);

    //// NB (alkurbatov): Increase speed of steps processing.
    //// Disables ability to control your bot during game.
    //// Recommended for competitions.
    //coordinator.SetRawAffectsSelection(true);

    //coordinator.JoinGame();
    //coordinator.SetTimeoutMS(10000);
    //std::cout << "Successfully joined game" << std::endl;

    //while (coordinator.Update())
    //{}

    Options Options;
    ParseArguments(argc, argv, &Options);

    printf("PARSED OPTIONS: GamePort:%d StartPort:%d OppID:%s Server:%s\n", Options.GamePort, Options.StartPort, Options.OpponentId.c_str(), Options.ServerAddress.c_str());

    sc2::Coordinator coordinator;
    Bot bot;

    // Add the custom bot, it will control the players.
    int num_agents = 2;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Protoss, &bot), });
    //bot.SetOpponentId(Options.OpponentId);

    // Request the feature layers to allow the use of the single drop micro
    sc2::FeatureLayerSettings settings;
    coordinator.SetFeatureLayers(settings);
    // Connect to the game client
    std::cout << "Connecting to port " << Options.GamePort << std::endl;
    coordinator.Connect(Options.GamePort);
    coordinator.SetupPorts(num_agents, Options.StartPort, false);
    // Set the unit selection policy
    // (if true, the selection will jump around everywhere so it can be harder to debug and doesn't allow a human to play at the same time)
    coordinator.SetRawAffectsSelection(true);
    // Join the already started game
    coordinator.JoinGame();
    coordinator.SetTimeoutMS(120000);	// 2 min
    std::cout << "Successfully joined game" << std::endl;
    // Step forward the game simulation.
    while (coordinator.Update()) {
    }

    return 0;
}

#else

int main(int argc, char* argv[])
{
    //backward::SignalHandling sh;
    srand(clock());
    Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    Bot bot;
    Difficulty diff = Difficulty::HardVeryHard;

    if (MICRO_TEST) {
        Race race = Race::Random;
        coordinator.SetParticipants({ CreateParticipant(Race::Protoss, &bot), CreateComputer(race, diff) });

        coordinator.LaunchStarcraft();
        //coordinator.StartGame("MicroAIArena/Tier1MicroAIArena_v1.SC2Map");
        coordinator.StartGame("Test/Empty.SC2Map");

        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_STALKER, START_ME, 1, 6);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::ZERG_ZERGLING, START_OP, 2, 16);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_ADEPT, START_OP, 2, 2);

        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_STALKER, START_ME, 1, 9);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_IMMORTAL, START_ME, 1, 2);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_COLOSSUS, START_ME, 1, 2);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::ZERG_ZERGLING, START_OP, 2, 16);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_ADEPT, START_OP, 2, 6);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::TERRAN_MARINE, START_OP, 2, 14);

        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_OBSERVER, START_ME, 1, 13);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_COLOSSUS, START_ME, 1, 1);
        //bot.Debug()->DebugCreateUnit(UNIT_TYPEID::PROTOSS_ADEPT, START_OP, 2, 6);

        while (coordinator.Update()) {
        }

    }
    else {
        Race race = (Race)(std::rand() % 4);  // Race::Random;
        coordinator.SetParticipants({ CreateParticipant(Race::Protoss, &bot), CreateComputer(race, diff) });

        

        coordinator.LaunchStarcraft();
        //std::string maps[6] = { "5_13/Oceanborn513AIE.SC2Map",  "5_13/Equilibrium513AIE.SC2Map",
        //                       "5_13/GoldenAura513AIE.SC2Map", "5_13/Gresvan513AIE.SC2Map",
        //                       "5_13/HardLead513AIE.SC2Map",   "5_13/SiteDelta513AIE.SC2Map" };

        std::string maps[6] = { "AbyssalReefAIE.SC2Map",  "AcropolisAIE.SC2Map",
                               "AutomatonAIE.SC2Map", "EphemeronAIE.SC2Map",
                               "InterloperAIE.SC2Map",   "ThunderbirdAIE.SC2Map" };

        int r = std::rand() % 6;
        printf("rand %d [%d %d %d %d %d %d] %d\n", r, std::rand(), std::rand(), std::rand(), std::rand(), std::rand(),
            std::rand(), RAND_MAX);

        coordinator.StartGame(maps[r]);
        while (coordinator.Update()) {
        }
    }

    return 0;
}

#endif
