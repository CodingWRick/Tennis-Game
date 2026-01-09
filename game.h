#pragma once
#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <vector>
#include "platformCommon.h"
#include "renderer.h"
#include "logger.h"
#include "player.h"
#include "mapGenerator.h"
#include "networking.h"

#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

extern bool eNetInit;
extern int myPlayerId;
extern u16 needed;

void SimulateGame(Input* input, float deltaTime);

namespace Game {
    void ENetInit();
    void SetMoveP();
    void ListPlayers();
    void SetPNeeded(u16 playerCount);
}

class Player;
extern std::vector<Player> players;

extern MapGenerator mapGenerator;