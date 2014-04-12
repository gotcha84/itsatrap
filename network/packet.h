#ifndef PACKET_H
#define PACKET_H

// Packet for clients
struct packet {
	int eventId;	
};


// Initial connection between client/server
struct initResponsePacket
{
	int eventId;
	int givenPlayerId;
};

#endif
