#ifndef PACKET_H
#define PACKET_H

#include "StateEntry.h"

#define INIT_REQUEST_EVENT 1
#define INIT_RESPONSE_EVENT 2
#define SINGLE_STATE_UPDATE_EVENT 3
#define WORLD_UPDATE_EVENT 4

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

// State updates
struct singleStateUpdatePacket
{
	int eventId;
	int playerId;
	struct stateEntry entry;
};

#endif
