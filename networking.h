#pragma once
#include <enet/enet.h>
#include "logger.h"

extern int maxClients;
extern int currentClients;
extern bool isServer;
extern bool mapInitialized;

void SendData(const void* message, size_t s, ENetPeer *to);
void SendMapData(ENetPeer *to);
void SendPlayerData(ENetPeer *to);
void SendPNeeded(ENetPeer *to);
void ServerInit();
void ServerConnect(ENetPeer *peer, ENetPeer* client);
void ServerGetPacket(ENetEvent event);
void ClientInit();
void ClientGetPacket(ENetEvent event);