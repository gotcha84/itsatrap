#ifndef PACKET_H
#define PACKET_H

#include "StateEntry.h"

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
	StateEntry update;
};

#endif
