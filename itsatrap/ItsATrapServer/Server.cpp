#include <sys/types.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "packet.h"
#include "NetworkConfig.h"

// Function Prototypes
int initialize();
void processMsg(char *);
int recieveMsg(char *);
int sendMsg(char *);

// Global Vars
struct 	sockaddr_in myAddress, clientAddress;
int 	i_sockfd;
char 	c_msg[BUFSIZE];
WSADATA	wsaData;


int main(int argc, char ** argv) {

	initialize();
	printf("Server is running.\n");

	while(1) {
		recieveMsg(c_msg);	
		processMsg(c_msg);
	}

	return 0;
}


// Inital setup for the server
int initialize() {

	// Load WinSock
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) 
	{ 
		fprintf(stderr, "WSAStartup() failed"); 
		exit(1); 
	} 

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

	return 0;
}

// Server processes a given message
void processMsg(char * msg) {
	// TODO (ktngo): Bad practice. Move away from using structs
	// and serialize messages.
	struct packet *p = (struct packet *) msg;
	
	printf("eventId: %d\n", p->eventId);

	// Task Deliberation
	switch (p -> eventId) {
		case 1:
			// Init event
			struct initResponsePacket response;
			response.eventId = 2;
			response.givenPlayerId = 7;
	
			// Send Message
			sendMsg((char *) &response);
		
			printf("[SERVER]: Init Response sent\n");
			break;
		case 2:
			break;
		default:
			// Echo the message
			char buf[BUFSIZE];
			memset(buf, 0, BUFSIZE);
			sprintf(buf, "I'm server, I received your %s", msg);

			// Send Message
			sendMsg(buf);
			break;
	}
}

// Server recieves a message
int recieveMsg(char * msg) {
	int len = sizeof(clientAddress);

	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *) &clientAddress, &len) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	return 0;
}

// Server sends a message to the current client
int sendMsg(char * msg) {
	if (sendto(i_sockfd, msg, sizeof(msg), 0, (struct sockaddr *) &clientAddress, sizeof(clientAddress)) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	return 0;
}
