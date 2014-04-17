
#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "Networking\Packet.h"
#include "Networking\NetworkConfig.h"

// Function Prototypes
int receiveMsg(char *);
int sendMsg(char *, int);
DWORD WINAPI receiverThread(LPVOID);

// Static Variables
static struct sockaddr_in myAddress, serverAddress;
static int		len;
static int 		i_sockfd;
static char 	c_msg[BUFSIZE];
static WSADATA	wsaData;
static int playerId;

int initializeClient() {

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
		printf("[CLIENT]: client.cpp - bind failed with error code %d\n", error);
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
	initPacket.eventId = 1;
	sendMsg((char *) &initPacket, sizeof(initPacket));
	printf("Init request sent\n");
	
	// Receives Server response
	receiveMsg(c_msg); // TODO: what if server doesn't respond?

	struct packet *p = (struct packet *)c_msg;
	if (p->eventId == 2)
	{
		// Init response
		struct initResponsePacket *irp = (struct initResponsePacket *)p;
		printf("[CLIENT]: Server gave me ID: %d\n", irp->givenPlayerId);
		playerId = irp->givenPlayerId;
	}

	return 0;

}

// For now, direction up = 1, down = 2.
void sendMoveEvent(int direction)
{
	struct moveEvent packet;
	packet.eventId = 3;
	packet.playerId = playerId;
	packet.direction = direction;
	sendMsg((char *)&packet, sizeof(packet));
}

void startReceiverThread()
{
	DWORD tmp;
	CreateThread(NULL, 0, receiverThread, NULL, 0, &tmp);
}

DWORD WINAPI receiverThread(LPVOID param)
{
	printf("[CLIENT]: Receiver thread started\n");
	while (1)
	{
		char buf[BUFSIZE];
		receiveMsg(buf);
		printf("[CLIENT]: received: %s\n", buf);
	}
}

// Client receives messages from the server
int receiveMsg(char * msg) {

	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *) &serverAddress, &len) < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client.cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	return 0;
}

// Client sends message to the server
int sendMsg(char * msg, int len) {
	if (sendto(i_sockfd, msg, len, 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client.cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	return 0;
}
