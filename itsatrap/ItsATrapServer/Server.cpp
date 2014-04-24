#include "Server.h"

using namespace std;

// Static Vars
struct sockaddr_in	Server::myAddress;
int 				Server::i_sockfd;
char 				Server::c_msg[BUFSIZE];
WSADATA				Server::wsaData;
Player				Server::players[MAX_PLAYERS];
int					Server::playerCount;
struct bufferEntry	Server::packetBuffer[PACKET_BUFFER_SIZE];
int					Server::packetBufferCount;
DynamicWorld		Server::dynamicWorld;
StaticWorld			Server::staticWorld;




int Server::startServer()
{
	if (Server::initialize() != 0)
	{
		printf("[SERVER]: Server initialization failed!\n");
		return 1;
	}
	printf("[SERVER]: Server is running.\n");

	while(1) {
		struct sockaddr_in source;
		if (Server::receiveMsg(c_msg, &source) == 0)	
			Server::processIncomingMsg(c_msg, &source);
	}
}

// Inital setup for the server
int Server::initialize() {

	// Load WinSock
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) 
	{ 
		fprintf(stderr, "WSAStartup() failed"); 
		return 1; 
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
	CreateThread(NULL, 0, bufferProcessorThread, NULL, 0, &tmp);

	return 0;
}

// Server processes a given message
void Server::processIncomingMsg(char * msg, struct sockaddr_in *source) {
	// TODO (ktngo): Bad practice. Move away from using structs
	// and serialize messages.
	struct packet *p = (struct packet *) msg;
	printf("[SERVER]: Received a packet. eventId: %d\n", p->eventId);

	// Some events can be processed immediately (like init request)
	// Some events must be stored into the buffer, process it later
	if (p -> eventId == INIT_REQUEST_EVENT) 
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
			response.eventId = INIT_RESPONSE_EVENT;
			response.givenPlayerId = playerCount;
				
			// Send Message
			Server::sendMsg((char *) &response, sizeof(response), source);
			// TODO: what if client failed to receive this?
			printf("[SERVER]: Init Response sent. Given ID = %d\n", response.givenPlayerId);
				
			playerCount++;
		}
		else
		{
			printf("[SERVER]: Too many players!\n");
		}
	}
	else if (p->eventId == STATIC_OBJECT_CREATION_EVENT)
	{
		struct staticObjectPacket *staticObjPkt = (struct staticObjectPacket *)p;
		staticWorld.addObject(staticObjPkt->object);
		staticWorld.printWorld();
	}
	else
	{
		// TODO: NEED MUTEX LOCK
		memcpy(packetBuffer[packetBufferCount].msg, msg, BUFSIZE);
		packetBufferCount++;
		// TODO: NEED MUTEX UNLOCK
	}
}

DWORD WINAPI Server::bufferProcessorThread(LPVOID param)
{
	printf("[SERVER]: Process buffer thread started\n");
	while (1)
	{
		processBuffer();
		Sleep(33);
	}
}

void Server::broadcastDynamicWorld()
{
	char *buf;
	int size = dynamicWorld.serialize(&buf);

	for (int i = 0; i < playerCount; i++)
	{
		Server::sendMsg(buf, size, &players[i].clientAddress);
	}
}

void Server::processBuffer()
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
				dynamicWorld.updateObject(updatePacket->entry);

				break;
			}
		
			default:
				printf("[SERVER]: Unknown event at buffer %d\n", i);
				break;
		}
	}

	packetBufferCount = 0;

	if (dynamicWorld.getSize() > 0)
			broadcastDynamicWorld();

	// TODO: NEED MUTEX UNLOCK
}

// Server receives a message
int Server::receiveMsg(char * msg, struct sockaddr_in *source) {
	int len = sizeof(struct sockaddr_in);

	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *)source, &len) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	return 0;
}

// Server sends a message to the current client
int Server::sendMsg(char * msg, int len, struct sockaddr_in *destination) {
	if (sendto(i_sockfd, msg, len, 0, (struct sockaddr *)destination, sizeof(struct sockaddr_in)) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	//printf("[SERVER]: Sent a packet of length %d\n", len);

	return 0;
}
