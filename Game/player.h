#pragma once
#include <vector>
#include <iostream>
#include "../Engine/base.h"
#include "mapGenerator.h"
#include "../Engine/win32.h"

class Player {
    public:
        int playerId = 0;

        float posX = 50.f;
        float posY = 50.f;

        float speed = 100.f;

        float halfSize = 25.f;  

        u32 color = 0x000000;

        void Draw();
        void Move(float deltaTime);
        void SetTarget(float x, float y);
        void SetPlayerData(int id, float x, float y, int myId);
    private:
        vector<Node*> path;

        void FollowPath(float deltaTime);
};