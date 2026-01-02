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
                    Log(LOG_INFO, "Nuovo client connesso " + std::to_string(event.peer->address.host) + ":" + std::to_string(event.peer->address.port));
                    SendMapData(event.peer);
                    client = event.peer;
                    currentClients++;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    enet_packet_destroy(event.packet);
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