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
#include "ClientInstance.h"

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
	static bool		moveEvents[NUM_DIRECTIONS]; // UP, DOWN, LEFT, RIGHT
	static bool		jumpEvent, cameraChanged;
	static cameraObject playerCam;

	// Functions
	static int receiveMsg();
	static int sendMsg(char *, int);
	static void startReceiverThread();
	static void startSenderThread();
	static void startRefresherThread();
	static DWORD WINAPI receiverThread(LPVOID);
	static DWORD WINAPI senderThread(LPVOID);
	static DWORD WINAPI refresherThread(LPVOID);
	static void updateHotSpot(int x, int y, int z);
	static void updateActiveResourceNode(int id);
	static void handleDisconnectPlayer(int id);

public:
	// Functions
	static int initializeClient();
	static int getPlayerId();
	static void sendStaticObject(float, float, float, float, float, float);
	static void sendStaticRampObject(AABB rampBB, float slope);
	static void sendStaticResourceObject(AABB resourceBB, int id);
	static void sendPlayerUpdate(struct playerObject);
	static void sendSpawnTrapEvent(struct trapObject);
	static void sendKnifeHitEvent(int targetId);
	static void sendChannelAttemptEvent(int resourceId);
	static void sendReloadConfigFile();
	static void sendMoveEvent(Direction dir);
	static void sendJumpEvent();
	static void sendLookEvent(struct cameraObject cam);
};
#endif