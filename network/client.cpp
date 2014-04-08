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


int main()
{

	struct sockaddr_in myAddress, serverAddress;
	socklen_t len;
	
	// Creating UDP socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// Init structs
	memset(&myAddress, 0, sizeof(myAddress));
	memset(&serverAddress, 0, sizeof(serverAddress));
	
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
	
	if (sendto(sockfd, &initPacket, sizeof(initPacket), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) 
	{
		perror("sendto failed");
		return 1;
	}
	
	printf("Init request sent\n");
	
	char msg[BUFSIZE];
	
	// Receive a message
	// TODO: this should be on a different thread, but since we know the server will 
	//       echo our message, this is ok.
	// This is bad, because what if the server doesn't reply?
	if (recvfrom(sockfd, msg, BUFSIZE, 0, (struct sockaddr *)&serverAddress,&len) < 0)
	{
		perror("recvfrom failed");
		return 1;
	}
	struct packet *p = (struct packet *)msg;
	if (p->eventId == 2)
	{
		// Init response
		struct initResponsePacket *irp = (struct initResponsePacket *)p;
		printf("Server gave me ID: %d\n", irp->givenPlayerId);
	}
	
	while (1)
	{
		memset(msg, 0, BUFSIZE);
		
		// Waiting for user input
		fgets (msg, BUFSIZE, stdin);

		// Send a message
		if (sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) 
		{
			perror("sendto failed");
			return 1;
		}
		
		// Clearing stuff
		memset(msg, 0, BUFSIZE);
		len = sizeof(serverAddress);
		memset(&serverAddress, 0, len);
		
		// Receive a message
		// TODO: this should be on a different thread, but since we know the server will 
		//       echo our message, this is ok.
		if (recvfrom(sockfd, msg, BUFSIZE, 0, (struct sockaddr *)&serverAddress,&len) < 0)
		{
			perror("recvfrom failed");
			return 1;
		}
		
		printf("%s\n", msg);
	}
	
}