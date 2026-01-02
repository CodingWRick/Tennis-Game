#include "networking.h"
#include "game.h"

void ClientInit() {
    ENetHost* client = nullptr;
    ENetPeer* server = nullptr;

    client = enet_host_create(NULL, 1, 2, 0, 0);
    if(client == NULL) {
        Log(LOG_ERROR, "Failed to create ENet client host.");
        return;
    } else {
        isServer = false;
    }

    ENetAddress address = {};
    enet_address_set_host(&address, "localhost");
    address.port = 1642;

    server = enet_host_connect(client, &address, 2, 0);
    if(server == nullptr) {
        Log(LOG_ERROR, "No available peers for initiating an ENet connection.");
        enet_host_destroy(client);
    }
    else {
        ENetEvent event = {};
        if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            Log(LOG_INFO, "Connessione riuscita.");

            while(true) {
                while(enet_host_service(client, &event, 10) > 0) {
                    switch(event.type) {
                        case ENET_EVENT_TYPE_RECEIVE: {
                            Log(LOG_INFO, "Paccehtto di lunghezza: " + std::to_string(event.packet->dataLength) + " è stato ricevuto dal server.");
                            size_t bytes = event.packet->dataLength;
                            if (bytes > 0 && bytes % sizeof(POINT) == 0) {
                                size_t count = bytes / sizeof(POINT);
                                POINT* pts = (POINT*)event.packet->data;
                                std::vector<POINT> data;
                                data.reserve(count);
                                for (size_t i = 0; i < count; ++i) data.push_back(pts[i]);
                                mapGenerator.SetMapData(data);
                            } else {
                                Log(LOG_ERROR, "Received malformed map data (size mismatch).");
                            }
                            enet_packet_destroy(event.packet);
                            break;
                        }
                        case ENET_EVENT_TYPE_DISCONNECT:
                            Log(LOG_INFO, "Disconnesso dal server.");
                            return;
                        default:
                            break;
                    }
                }
            }
        } else {
            Log(LOG_ERROR, "Connessione fallita.");
        }

        enet_peer_reset(server);
        enet_host_destroy(client);
    }
}