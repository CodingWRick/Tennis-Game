#include "mapGenerator.h"

void MapGenerator::DrawMap() {
    step = 128.f;

    if(!mapGenerated) {
        openPos.clear();
        for(u32 x = 0; x < renderState.width; x += (u32)step) {
            for(u32 y = 0; y < renderState.height; y += (u32)step) {
                POINT tilePos;
                tilePos.x = x;
                tilePos.y = y;

                openPos.push_back(tilePos);
            }
        }

        mapGenerated = true;
    }
    else {
        for(const POINT &pos : openPos) {
            PixelDrawRect(pos.x, pos.y, (u32)step, (u32)step, grassColor);
        }
    }

    DrawObstacles();

    if(!gridGenerated) GenerateNodeGrid();
}

void MapGenerator::DrawObstacles() {
    float obstaclesMtpr = 0.00001f;
    maxObstacles = int(obstaclesMtpr * renderState.width * renderState.height);

    if(obstacles.empty() && !regeneratingMap) {
        obstacles.clear();
        int available = (int)openPos.size();
        int toPlace = maxObstacles;
        if(toPlace > available) toPlace = available;

        for(int i = 1; i < toPlace; ++i) {
            if (openPos.empty()) break;
            int index = rand() % (int)openPos.size(); 
            POINT obstaclePos = openPos[index];

            obstacles.push_back(obstaclePos);
            openPos.erase(openPos.begin() + index);
        }
    }
    else {
        for(const POINT &pos : obstacles) {
            PixelDrawRect(pos.x, pos.y, (u32)step, (u32)step, waterColor);
        }
    }
}

void MapGenerator::GenerateNodeGrid() {
    nodes.clear();

    for(size_t i = 0; i < openPos.size(); i++) {
        Node* n = new Node();
        n->posX = openPos[i].x + step / 2.f;
        n->posY = openPos[i].y + step / 2.f;
        nodes.push_back(n);
    }

    for(Node* n : nodes) {
        n->neighbors = n->GetNeighbors();
    }

    gridGenerated = true;
}

void MapGenerator::ResetMap() {
    regeneratingMap = true;
    
    initialWidth = renderState.width;
    initialHeight = renderState.height;

    mapGenerated = false;
    obstacles.clear();
    gridGenerated = false;
}

const std::vector<POINT>& MapGenerator::GetMapData() {
    return obstacles;
    Log(LOG_ERROR, "Obstacles being registered as open positions, send both openPos and obstacles in a struct.");
}

void MapGenerator::SetMapData(const std::vector<POINT>& data) {
    ResetMap();
    obstacles = data;
    regeneratingMap = false;
}