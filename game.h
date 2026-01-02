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

// Use internal if you want this function private to game.cpp
void SimulateGame(Input* input, float deltaTime);

class Player;
extern std::vector<Player> players;

extern MapGenerator mapGenerator;