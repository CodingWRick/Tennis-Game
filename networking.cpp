#include "networking.h"
#include "game.h"

int maxClients = 4;
int currentClients = 0;
bool isServer = false;
bool mapInitialized = false;

void SendData(const void* message, size_t s, ENetPeer *to) {
    ENetPacket* packet = enet_packet_create(message, s, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(to, 0, packet);
}

void SendMapData(ENetPeer *to) {
    const std::vector<POINT>& mapData = mapGenerator.GetMapData();
    if (mapData.empty()) return;

    size_t mapSize = mapData.size() * sizeof(POINT);
    SendData(mapData.data(), mapSize, to);
}

void SendPlayerData(ENetPeer *to) {
    for (const Player& p : players) {
        struct PlayerData {
            int playerId;
            float x, y;
            bool moving;
            int needed;
        } pData;

        pData.playerId = needed--;
        pData.x = p.posX;
        pData.y = p.posY;
        pData.moving = p.moving;

        SendData(&pData, sizeof(pData), to);
    }
}

void SendPNeeded(ENetPeer *to) {
    u16 pNeeded = (u16)currentClients + 1;
    SendData(&pNeeded, sizeof(pNeeded), to);
}