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
#include "WorldState.h"
#include "StateEntry.h"

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
	static WorldState			worldState;

	// Functions
	static int initialize();
	static void processMsg(char *, struct sockaddr_in *);
	static int receiveMsg(char *, struct sockaddr_in *);
	static int sendMsg(char *, int, struct sockaddr_in *);
	static DWORD WINAPI processBufferThread(LPVOID);
	static void processBuffer();
	static void broadcastWorldState();

public:

	// Functions
	static int startServer();
};

#endif