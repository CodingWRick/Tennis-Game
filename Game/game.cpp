#include "game.h"

std::vector<Player> players;
bool eNetInit = false;
int myPlayerId = 0;
u16 needed = 0;
MapGenerator mapGenerator;

void SimulateGame(Input* input, float deltaTime) {
    ClearScreen(0xFFFFFF);

    if(!eNetInit) {
        Game::ENetInit();
    }

    Game::ListPlayers();

    POINT p;
    GetCursorPos(&p);
    if(gWindow) ScreenToClient(gWindow, &p);

    //Game::DrawSelectTile(p);
    if(pressed(LEFT_MOUSE)) {
        players[myPlayerId].SetTarget((float)p.x, (float)p.y);
    }

    if(!players.empty()) {
        players[myPlayerId].Move(deltaTime);
    }

    mapGenerator.DrawMap();

    for(Player& p : players) {
        p.Draw();
    }
}

namespace Game {
    void ENetInit() {
        if(enet_initialize() != 0) {
            Log(LOG_ERROR, "Failed to initialize ENet.");
        } else {
            Log(LOG_INFO, "Network inizializzato.");
            Log(LOG_INFO, "Server o Client? (s/c): ");
            std::thread([](){
                char choice = '\0';
                std::cin >> choice;
                if (choice == 's') {
                    ServerInit();
                } else if (choice == 'c') {
                    ClientInit();
                }
            }).detach();
            eNetInit = true;
            Log(LOG_ERROR, "Malformed packet, impossible to send player data and sync obstacles.");
        }
    }

    /*
    void DrawSelectTile(POINT p) {
        u32 selectColor = 0xFFFF00;
        u32 size = (u32)step;
        u32 borderWidth = 20;

        int x0 = (int)(FindClosestNode(p.x, p.y)->posX); // Line 63 and 64 crashing game
        int y0 = (int)(FindClosestNode(p.x, p.y)->posY);

        PixelDrawRectEmpty(x0, y0, size, size, borderWidth, selectColor);
    }
    */

    void ListPlayers() {
        needed = (u16)currentClients + 1;
        if (players.size() < needed) {
            for (u16 i = players.size(); i < needed; ++i) {
                Player newPlayer;
                newPlayer.playerId = (int)i;
                players.push_back(newPlayer);
            }
        } else if (players.size() > needed) {
            players.resize(needed);
        }
    }

    void SetPNeeded(u16 playerCount) {
        needed = playerCount;
    }
}