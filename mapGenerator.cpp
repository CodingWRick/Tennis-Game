#include "mapGenerator.h"

void MapGenerator::DrawMap() {
    step = 128.f;

    if(!mapGenerated) {
        openPos.clear();
        for(int x = 0; x < renderState.width; x += (int)step) {
            for(int y = 0; y < renderState.height; y += (int)step) {
                POINT tilePos;
                tilePos.x = x;
                tilePos.y = y;
                openPos.push_back(tilePos);

                PixelDrawRect(x, y, (int)step, (int)step, grassColor);
                Log(LOG_DEBUG, "Generated normal tile at (" + std::to_string(x) + ", " + std::to_string(y) + ").");
            }
        }

        Log(LOG_ERROR, "Normal tiles not generating, using clearscreen instead.");
        mapGenerated = true;
    }
    else {
        mapGenerated = false;
        
        for(const POINT &pos : openPos) {
            PixelDrawRect(pos.x, pos.y, (int)step, (int)step, grassColor);
        }

        mapGenerated = true;
    }

    DrawObstacles();

    if(!gridGenerated) GenerateNodeGrid();
}

void MapGenerator::DrawObstacles() {
    maxObstacles = int(0.00001f * renderState.width * renderState.height);

    if(obstacles.empty() && !regeneratingMap) {
        obstacles.clear();
        int available = (int)openPos.size();
        int toPlace = maxObstacles;
        if (toPlace > available) toPlace = available;

        for(int i = 1; i < toPlace; ++i) {
            if (openPos.empty()) break;
            int index = rand() % (int)openPos.size();
            POINT obstaclePos = openPos[index];

            obstacles.push_back(obstaclePos);
            PixelDrawRect(obstaclePos.x, obstaclePos.y, obstaclePos.x + (int)step, obstaclePos.y + (int)step, waterColor);

            openPos.erase(openPos.begin() + index);
        }
    }
    else {
        for(const POINT &pos : obstacles) {
            PixelDrawRect(pos.x, pos.y, pos.x + (int)step, pos.y + (int)step, waterColor);
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
}

void MapGenerator::SetMapData(const std::vector<POINT>& data) {
    ResetMap();
    obstacles = data;
    regeneratingMap = false;
}