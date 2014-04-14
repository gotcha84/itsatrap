#ifndef PACKET_H
#define PACKET_H

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

// Move events
struct moveEvent
{
	int eventId;
	int playerId;
	int direction;
};

#endif
