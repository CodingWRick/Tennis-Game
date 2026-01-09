#pragma once
#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <vector>
#include "../Engine/platformCommon.h"
#include "../Engine/renderer.h"
#include "../Engine/logger.h"
#include "player.h"
#include "mapGenerator.h"
#include "Networking/networking.h"

#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

extern bool eNetInit;
extern int myPlayerId;
extern u16 needed;

void SimulateGame(Input* input, float deltaTime);

namespace Game {
    void ENetInit();
    POINT DrawSelectTile();
    void SetMoveP(POINT p);
    void ListPlayers();
    void SetPNeeded(u16 playerCount);
}

class Player;
extern std::vector<Player> players;

extern MapGenerator mapGenerator;