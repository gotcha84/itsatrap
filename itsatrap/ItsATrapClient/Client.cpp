#include "Client.h"

// Variables
struct sockaddr_in	Client::myAddress, Client::serverAddress;
int					Client::len;
int 				Client::i_sockfd;
char 				Client::c_msg[BUFSIZE];
WSADATA				Client::wsaData;
int					Client::playerId;

int Client::initializeClient() {

	// Load WinSock
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) 
	{ 
		fprintf(stderr, "WSAStartup() failed"); 
		exit(1); 
	}

	len = sizeof(serverAddress);

	// Creating UDP socket
	i_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// TODO (ktngo): Remove when using multiple different computers
	// Setting this client's address and port, and bind them
	// WE SHOULD UNCOMMENT THIS LATER
	/*
	memset(&myAddress, 0, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	myAddress.sin_port=htons(SERVER_PORT);
	if (bind(i_sockfd, (struct sockaddr *)&myAddress, sizeof(myAddress)) < 0)
	{
		int error = WSAGetLastError();
		printf("[CLIENT]: client->cpp - bind failed with error code %d\n", error);
		return 1;
	}
	*/
	
	// Setting the server's address
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	serverAddress.sin_port = htons(SERVER_PORT);
	
	// Send init event
	struct packet initPacket;
	initPacket.eventId = INIT_REQUEST_EVENT;
	Client::sendMsg((char *) &initPacket, sizeof(initPacket));
	printf("[CLIENT]: Init request sent\n");
	
	// Receives Server response
	Client::receiveMsg(c_msg); // TODO: what if server doesn't respond?

	struct packet *p = (struct packet *)c_msg;
	if (p->eventId == INIT_RESPONSE_EVENT)
	{
		// Init response
		struct initResponsePacket *irp = (struct initResponsePacket *)p;
		printf("[CLIENT]: Server gave me ID: %d\n", irp->givenPlayerId);
		playerId = irp->givenPlayerId;
	}

	Client::startReceiverThread();

	return 0;
	
}

void Client::startReceiverThread()
{
	DWORD tmp;
	CreateThread(NULL, 0, Client::receiverThread, NULL, 0, &tmp);
}

DWORD WINAPI Client::receiverThread(LPVOID param)
{
	printf("[CLIENT]: Receiver thread started\n");
	while (1)
	{
		char buf[BUFSIZE];
		if (Client::receiveMsg(buf) == 0)
		{
			struct packet *p = (struct packet *) buf;

			if (p->eventId == WORLD_UPDATE_EVENT)
			{
				// World update. 
				// This variable 'world' is the world given by the server
				DynamicWorld world(p);
				testUpdateWorld(&world);
			}
		}
	}
}

// Sending state updates
void Client::sendStateUpdate(int id, float x, float y, float z)
{
	struct singleStateUpdatePacket p;
	p.eventId = SINGLE_STATE_UPDATE_EVENT;
	p.playerId = playerId;
	p.entry.objectId = id;
	p.entry.x = x;
	p.entry.y = y;
	p.entry.z = z;

	Client::sendMsg((char *)&p, sizeof(p));
}


// Client receives messages from the server
int Client::receiveMsg(char * msg) {

	int bytesReceived = 0;
	bytesReceived = recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *) &serverAddress, &len);

	if (bytesReceived < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client->cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	//printf("[CLIENT]: received message of size %d\n", bytesReceived);

	return 0;
}

// Client sends message to the server
int Client::sendMsg(char * msg, int len) {
	if (sendto(i_sockfd, msg, len, 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client->cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	return 0;
}

int Client::getPlayerId()
{
	return playerId;
}

void Client::sendStaticObject(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	struct staticObjectPacket packet;
	packet.eventId = STATIC_OBJECT_CREATION_EVENT;
	packet.object.minX = minX;
	packet.object.minY = minY;
	packet.object.minZ = minZ;
	packet.object.maxX = maxX;
	packet.object.maxY = maxY;
	packet.object.maxZ = maxZ;

	sendMsg((char *)&packet, sizeof(struct staticObjectPacket));
}