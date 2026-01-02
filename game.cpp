#include "game.h"

std::vector<Player> players;
bool eNetInit = false;
int myPlayerId = 0;
MapGenerator mapGenerator;

void SimulateGame(Input* input, float deltaTime) {
    ClearScreen(0x00FF00);
    if(!eNetInit) {
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

    if(pressed(LEFT_MOUSE)) {
        POINT p;
        GetCursorPos(&p);
        if(gWindow) ScreenToClient(gWindow, &p);
        players[myPlayerId].SetTarget((float)p.x, (float)p.y);
        players[myPlayerId].moving = true;
    }

    if(!players.empty() && players[myPlayerId].moving) {
        players[myPlayerId].Move(deltaTime);
    }

    mapGenerator.DrawMap();

    // Ensure we have one Player per connected client plus one for the server host.
    size_t needed = (size_t)currentClients + 1;
    if (players.size() < needed) {
        for (size_t i = players.size(); i < needed; ++i) {
            Player newPlayer;
            newPlayer.playerId = (int)i;
            players.push_back(newPlayer);
        }
    }

    for(Player& p : players) {
        p.Draw();
    }
}