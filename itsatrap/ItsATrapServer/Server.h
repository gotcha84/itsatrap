#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "Stopwatch.h"
#include "Packet.h"
#include "NetworkConfig.h"
#include "Player.h"
#include "DynamicWorld.h"
#include "NetworkObjects.h"

using namespace std;

struct bufferEntry {
	int playerId;
	char msg[BUFSIZE];
};

/*
 * class Server
 *
 * Everything in this class is static because we will only need one instance
 * of this class at all times.
 */
class Server {

private:

	// Variables
	static struct sockaddr_in	myAddress;
	static int 					i_sockfd;
	static char 				c_msg[BUFSIZE];
	static WSADATA				wsaData;
	static Player				players[MAX_PLAYERS];
	static int					playerCount;
	static struct bufferEntry	packetBuffer[PACKET_BUFFER_SIZE];
	static int					packetBufferCount;
	static DynamicWorld			dynamicWorld;
	static Stopwatch			stopwatch;

	static int					timeUntilResourceBonus;
	static int					timeUntilHotSpotChange;
	static int					resourcePerInterval;
	static int					resourceHotSpotBonusPerInterval;
	static int					resourceInterval;
	static int					hotSpotChangeInterval;

	//static vector<glm::vec3>	hotSpotLocations;
	//static glm::vec3			currentHotSpot;
	//static int					currentHotSpotIndex;

	static vector<int>			resourceNodeLocations;
	static int					currentActiveResourceNodeIndex;
	static int					currentResourceOwner;
	static bool					isChanneling;

	// Functions
	static int initialize();
	static void processIncomingMsg(char *, struct sockaddr_in *);
	static int receiveMsg(char *, struct sockaddr_in *);
	static int sendMsg(char *, int, struct sockaddr_in *);
	static int broadcastMsg(char *, int);
	static DWORD WINAPI bufferProcessorThread(LPVOID);
	static void processBuffer();
	static void broadcastDynamicWorld();
	static void respawnPlayer(int id);
	static void updateResources();
	//static void sendHotSpotUpdate(int x, int y, int z);
	static void sendActiveNodeUpdate(int id);
	static void printPacket(struct packet *p);
	static void checkConnection();
	static void disconnectPlayer(int id);

public:

	// Functions
	static int startServer();
};

#endif