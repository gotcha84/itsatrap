#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "packet.h"
#include "NetworkConfig.h"
#include "Player.h"
#include "WorldState.h"
#include "StateEntry.h"

using namespace std;

// Function Prototypes
int initialize();
void processMsg(char *, struct sockaddr_in *);
int receiveMsg(char *, struct sockaddr_in *);
int sendMsg(char *, int, struct sockaddr_in *);
DWORD WINAPI processBufferThread(LPVOID);
void processBuffer();
void broadcastWorldState();

struct bufferEntry {
	int playerId;
	char msg[BUFSIZE];
};

// Static Vars
static struct sockaddr_in	myAddress;
static int 					i_sockfd;
static char 				c_msg[BUFSIZE];
static WSADATA				wsaData;
static Player				players[MAX_PLAYERS];
static int					playerCount;
static struct bufferEntry	packetBuffer[PACKET_BUFFER_SIZE];
static int					packetBufferCount;
static WorldState			worldState;

int main(int argc, char ** argv) {

	initialize();
	printf("[SERVER]: Server is running.\n");

	while(1) {
		struct sockaddr_in source;
		if (receiveMsg(c_msg, &source) == 0)	
			processMsg(c_msg, &source);
	}

	return 0;
}


// Inital setup for the server
int initialize() {

	// Load WinSock
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) 
	{ 
		fprintf(stderr, "WSAStartup() failed"); 
		exit(1); 
	}

	// Init variables
	packetBufferCount = 0;
	playerCount = 0;

	// Create UDP socket
	i_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Set up server address and port
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddress.sin_port = htons(SERVER_PORT);

	// Bind server address and port
	if (bind(i_sockfd, (struct sockaddr *)&myAddress, sizeof(myAddress)) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - bind failed with error code %d\n", error);
		return 1;
	}

	DWORD tmp;
	CreateThread(NULL, 0, processBufferThread, NULL, 0, &tmp);

	return 0;
}

// Server processes a given message
void processMsg(char * msg, struct sockaddr_in *source) {
	// TODO (ktngo): Bad practice. Move away from using structs
	// and serialize messages.
	struct packet *p = (struct packet *) msg;
	printf("[SERVER]: Received a packet. eventId: %d\n", p->eventId);

	// Some events can be processed immediately (like init request)
	// Some events must be stored into the buffer, process it later
	if (p -> eventId == 1) 
	{
		if (playerCount < MAX_PLAYERS)
		{
			// Creating player object
			Player player;
			player.clientAddress = *source;
			player.playerId = playerCount;
			player.yPosition = 0;
			players[playerCount] = player;
				
			// Creating response message
			struct initResponsePacket response;
			response.eventId = 2;
			response.givenPlayerId = playerCount;
				
			// Send Message
			sendMsg((char *) &response, sizeof(response), source);
			// TODO: what if client failed to receive this?
			printf("[SERVER]: Init Response sent. Given ID = %d\n", response.givenPlayerId);
				
			playerCount++;
		}
		else
		{
			printf("[SERVER]: Too many players!\n");
		}
	}
	else
	{
		// TODO: NEED MUTEX LOCK
		memcpy(packetBuffer[packetBufferCount].msg, msg, BUFSIZE);
		packetBufferCount++;
		// TODO: NEED MUTEX UNLOCK
	}
}

DWORD WINAPI processBufferThread(LPVOID param)
{
	printf("[SERVER]: Process buffer thread started\n");
	while (1)
	{
		processBuffer();
		Sleep(1000);

		cout << " ===== CURRENT STATE OF THE WORLD =====" << endl;
		for (int i = 0; i < worldState.entries.size(); i++)
		{
			StateEntry entry = worldState.entries[i];
			printf("Object id:%d x:%.1f y:%.1f z:%.1f\n", entry.objectId, entry.x, entry.y, entry.z);
			
		}
		if (worldState.entries.size() > 0)
			broadcastWorldState();
	}
}

void broadcastWorldState()
{
	int size = 2*sizeof(int) + sizeof(struct StateEntry) * worldState.entries.size();
	char *buf = (char *) malloc(size);

	((int *)buf)[0] = 4;
	((int *)buf)[1] = worldState.entries.size();

	printf("size:%d\n", ((int *)buf)[1]);

	for (int i = 0; i < worldState.entries.size(); i++)
	{
		memcpy(buf + 2*sizeof(int) + i * sizeof(struct StateEntry), &worldState.entries[i], sizeof(struct StateEntry));
	}

	for (int i = 0; i < playerCount; i++)
	{
		sendMsg(buf, size, &players[i].clientAddress);
	}
}

void processBuffer()
{
	// TODO: NEED MUTEX LOCK

	for (int i = 0; i < packetBufferCount; i++)
	{
		struct packet *p = (struct packet *) packetBuffer[i].msg;
		printf("[SERVER]: Now processing packet at buffer %d with eventId: %d\n", i, p->eventId);
		
		switch (p->eventId)
		{
			case 3: // State updates
			{
				// Update player state
				struct singleStateUpdatePacket *updatePacket = (struct singleStateUpdatePacket *)p;

				bool found = false;
				for (int i = 0; i < worldState.entries.size(); i++)
				{
					if (worldState.entries[i].objectId == updatePacket->update.objectId)
					{
						found = true;
						worldState.entries[i].x = updatePacket->update.x;
						worldState.entries[i].y = updatePacket->update.y;
						worldState.entries[i].z = updatePacket->update.z;
					}
				}

				if (!found)
					worldState.addEntry(updatePacket->update);

				
				// Send new state
				//char tmp[BUFSIZE];
				//memset(tmp, 0, BUFSIZE);
				//sprintf(tmp, "You are now at position %d", players[e->playerId].yPosition);
				//sendMsg(tmp, BUFSIZE, &players[e->playerId].clientAddress);

				break;
			}
		
			default:
				printf("[SERVER]: Unknown event at buffer %d\n", i);
				break;
		}
	}

	packetBufferCount = 0;

	// TODO: NEED MUTEX UNLOCK
}

// Server receives a message
int receiveMsg(char * msg, struct sockaddr_in *source) {
	int len = sizeof(struct sockaddr_in);

	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *)source, &len) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	return 0;
}

// Server sends a message to the current client
int sendMsg(char * msg, int len, struct sockaddr_in *destination) {
	if (sendto(i_sockfd, msg, len, 0, (struct sockaddr *)destination, sizeof(struct sockaddr_in)) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	printf("[SERVER]: Sent a packet of length %d\n", len);

	return 0;
}
