
#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Packet.h"
#include "NetworkConfig.h"

// Function Prototypes
int recieveMsg(char *);
int sendMsg(char *);

// Global Variables
struct sockaddr_in myAddress, serverAddress;
int		len;
int 	i_sockfd;
char 	c_msg[BUFSIZE];
WSADATA	wsaData;

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
	sendMsg((char *) &initPacket);
	printf("Init request sent\n");
	
	// Recieve Server response
	recieveMsg(c_msg);	

	struct packet *p = (struct packet *)c_msg;
	if (p->eventId == 2)
	{
		// Init response
		struct initResponsePacket *irp = (struct initResponsePacket *)p;
		printf("Server gave me ID: %d\n", irp->givenPlayerId);
	}

	return 0;

}

// Client recieves messages from the server
int recieveMsg(char * msg) {

	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *) &serverAddress, &len) < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client.cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	return 0;
}

// Client sends message to the server
int sendMsg(char * msg) {
	if (sendto(i_sockfd, msg, sizeof(msg), 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client.cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	return 0;
}
