#include "networking.h"
#include "game.h"

void ServerInit() {
    ENetAddress address = {};
    ENetHost* server = nullptr;
    address.host = ENET_HOST_ANY;
    address.port = 1642;

    server = enet_host_create(&address, maxClients, 2, 0, 0);
    if(server == NULL) {
        Log(LOG_ERROR, "Failed to create ENet server host.");
        return;
    } else {
        Log(LOG_INFO, "Host creato con successo.");
        isServer = true;
    }

    ENetPeer *client = 0;

    while(true) {
        ENetEvent event;
        while(enet_host_service(server, &event, 10) > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    ServerConnect(event.peer, client);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    ServerGetPacket(event);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    Log(LOG_INFO, "Client disconnesso.");
                    client = 0;
                    break;
                default:
                    break;
            }
        }
    }

    enet_host_destroy(server);
}

void ServerConnect(ENetPeer *peer, ENetPeer* client) {
    Log(LOG_INFO, "Nuovo client connesso " + std::to_string(peer->address.host) + ":" + std::to_string(peer->address.port));
    SendMapData(peer);
    SendPlayerData(peer);
    SendPNeeded(peer);
    client = peer;
    currentClients++;
}

void ServerGetPacket(ENetEvent event) {
    u64 bytes = event.packet->dataLength;
    u16 pNeeded = NULL;
    if(bytes == sizeof(int) + sizeof(float) * 2 + sizeof(bool)) {
        for(Player &p : players) {
            struct PlayerData {
                int playerId;
                float x, y;
                bool moving;
            }* pData = (PlayerData*)event.packet->data;
            pNeeded++;

            p.SetPlayerData(pData->playerId, pData->x, pData->y, pData->moving, myPlayerId);
        }

        SendPlayerData(event.peer);
    } else if(bytes == sizeof(u16)) {
        pNeeded = *(u16*)event.packet->data;
        Game::SetPNeeded(pNeeded);
        
        SendPNeeded(event.peer);
    } else if(bytes > 0 && bytes % sizeof(POINT) == 0){
        u64 count = bytes / sizeof(POINT);
        POINT* pts = (POINT*)event.packet->data;
        std::vector<POINT> data;
        data.reserve(count);
        for(u64 i = 0; i < count; ++i) data.push_back(pts[i]);
        mapGenerator.SetMapData(data);
    } else {
        Log(LOG_ERROR, "Received malformed data packet (size mismatch).");
    }

    enet_packet_destroy(event.packet);
}