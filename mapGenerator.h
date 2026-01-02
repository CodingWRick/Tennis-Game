#pragma once
#include <vector>
#include "renderer.h"
#include "AStarManager.h"
#include "node.h"
#include "logger.h"

class MapGenerator {
    public:
        void DrawMap();
        void SetMapData(const std::vector<POINT>& data);
        const std::vector<POINT>& GetMapData();
        
        vector<POINT> openPos;
    private:
        void DrawObstacles();
        void GenerateNodeGrid();
        void ResetMap();

        int maxObstacles;

        int initialWidth = renderState.width;
        int initialHeight = renderState.height;

        bool mapGenerated = false;
        bool obstaclesGenerated = false;
        bool gridGenerated = false;
        vector<POINT> obstacles;
        u32 grassColor = 0x00FF00;
        u32 waterColor = 0x00FFFF;
};