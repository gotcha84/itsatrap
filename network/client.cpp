#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "packet.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 5555
#define BUFSIZE 512

// Function Prototypes
int initialize();
int recieveMsg(char * msg);
int sendMsg(char * msg, int length);

// Global Variables
struct sockaddr_in myAddress, serverAddress;
socklen_t len;
int 	i_sockfd;
char 	c_msg[BUFSIZE];

int main() {
	initialize();

	while (1)
	{
		memset(c_msg, 0, BUFSIZE);
		
		// Waiting for user input
		fgets (c_msg, BUFSIZE, stdin);

		// Send a message
		sendMsg(c_msg, sizeof(c_msg));
		
		// Clearing stuff
		memset(c_msg, 0, BUFSIZE);
		len = sizeof(serverAddress);
		memset(&serverAddress, 0, len);
		
		// Receive a message
		recieveMsg(c_msg);
		
		printf("%s\n", c_msg);
	}
	
}

int initialize() {
	// Creating UDP socket
	i_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// Init structs
	memset(&myAddress, 0, sizeof(myAddress));
	memset(&serverAddress, 0, sizeof(serverAddress));

	// TODO (ktngo): Remove when using multiple different computers
	// Setting this client's address and port, and bind them (may not be necessary)
	// WE SHOULD UNCOMMENT THIS LATER
	/*
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	myAddress.sin_port=htons(SERVER_PORT);
	if (bind(sockfd, (struct sockaddr *)&myAddress, sizeof(myAddress)) < 0)
	{
		perror("bind failed");
		return 1;
	}
	*/
	
	// Setting the server's address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	serverAddress.sin_port = htons(SERVER_PORT);
	
	// Send init event
	struct packet initPacket;
	initPacket.eventId = 1;
	sendMsg((char *) &initPacket, sizeof(initPacket));
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
// Receive a message
	// TODO: this should be on a different thread, but since we know the server will 
	//       echo our message, this is ok. Bad because, what if the server doesn't reply?
	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *) &serverAddress, &len) < 0) {
		perror("[CLIENT]: client.cpp - recvfrom failed");
		return 1;
	}

	return 0;
}

// Client sends message to the server
int sendMsg(char * msg, int length) {
	if (sendto(i_sockfd, msg, length, 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		perror("[CLIENT]: client.cpp - sendto failed");
		return 1;
	}

	return 0;
}
