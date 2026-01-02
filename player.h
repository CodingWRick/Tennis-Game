#pragma once
#include <vector>
#include <iostream>
#include "renderer.h"
#include "mapGenerator.h"
#include "win32.h"
#include "logger.h"

class Player {
    public:
        int playerId = 0;

        float posX = 0.f;
        float posY = 0.f;

        float speed = 100.f;

        bool moving = false;

        float halfSizeX = 50.f; 
        float halfSizeY = 50.f; 

        u32 color = 0x000000;

        void Draw();
        void Move(float deltaTime);
        void SetTarget(float x, float y);
    private:
        vector<Node*> path;

        void FollowPath(float deltaTime);
};