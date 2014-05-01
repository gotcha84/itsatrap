#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "Packet.h"
#include "NetworkConfig.h"
#include "DynamicWorld.h"
#include "NetworkObjects.h"
#include "enrico.h"

/*
 * class Client
 *
 * Everything in this class is static because we will only need one instance
 * of this class at all times.
 */
class Client {

private:

	// Variables
	static struct sockaddr_in myAddress, serverAddress;
	static int		len;
	static int 		i_sockfd;
	static char 	c_msg[BUFSIZE];
	static WSADATA	wsaData;
	static int playerId;

	// Functions
	static int receiveMsg(char *);
	static int sendMsg(char *, int);
	static DWORD WINAPI receiverThread(LPVOID);

public:

	// Functions
	static int initializeClient();
	static void startReceiverThread();
	static int getPlayerId();
	static void sendStaticObject(float, float, float, float, float, float);
	static void sendPlayerUpdate(struct playerObject);
	static void requestToSpawnTrap(struct trapObject);
	static void sendKnifeHitEvent(int targetId);
};



#endif