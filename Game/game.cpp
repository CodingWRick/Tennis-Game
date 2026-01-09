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

    POINT p = Game::DrawSelectTile();
    if(pressed(LEFT_MOUSE)) {
        Game::SetMoveP(p);
    }

    if(!players.empty() && players[myPlayerId].moving) {
        players[myPlayerId].Move(deltaTime);
    }

    mapGenerator.DrawMap();

    Game::ListPlayers();

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
        }
    }

    POINT DrawSelectTile() {
        POINT p;
        GetCursorPos(&p);
        if(gWindow) ScreenToClient(gWindow, &p);

        u32 selectColor = 0xFFFF00;
        float halfSize = step / 2.f;
        u32 borderWidth = 20;
        PixelDrawRectEmpty((float)(p.x) - halfSize, (float)(p.y) - halfSize, halfSize * 2, halfSize * 2, borderWidth, selectColor);
        
        Log(LOG_INFO, p.x + ", " + p.y);

        return p;
    }

    void SetMoveP(POINT p) {
        players[myPlayerId].SetTarget((float)p.x, (float)p.y);
        players[myPlayerId].moving = true;
    }

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