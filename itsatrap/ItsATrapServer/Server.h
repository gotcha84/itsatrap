#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "Packet.h"
#include "NetworkConfig.h"
#include "Player.h"
#include "DynamicWorld.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "StaticWorld.h"
#include "Stopwatch.h"

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
	static StaticWorld			staticWorld;

	// Functions
	static int initialize();
	static void processIncomingMsg(char *, struct sockaddr_in *);
	static int receiveMsg(char *, struct sockaddr_in *);
	static int sendMsg(char *, int, struct sockaddr_in *);
	static DWORD WINAPI bufferProcessorThread(LPVOID);
	static void processBuffer();
	static void broadcastDynamicWorld();

public:

	// Functions
	static int startServer();
};

#endif