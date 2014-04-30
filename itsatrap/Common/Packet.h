#ifndef PACKET_H
#define PACKET_H

#include "NetworkObjects.h"

#define INIT_REQUEST_EVENT 1
#define INIT_RESPONSE_EVENT 2
#define PLAYER_UPDATE_EVENT 3
#define WORLD_UPDATE_EVENT 4
#define STATIC_OBJECT_CREATION_EVENT 5

// Superclass of all packets
struct packet {
	int eventId;	
};


// Initial connection between client/server
struct initResponsePacket
{
	int eventId;
	int givenPlayerId;
};


struct playerUpdatePacket
{
	int eventId;
	struct playerObject playerObj;
};

struct staticObjectPacket
{
	int eventId;
	int playerId; // who is sending this?
	struct staticObject object;
};

#endif
