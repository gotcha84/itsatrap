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
Stopwatch			Server::stopwatch;
int					Server::timeUntilResourceBonus;
int					Server::timeUntilHotSpotChange;
int					Server::resourcePerInterval;
int					Server::resourceHotSpotBonusPerInterval;
int					Server::resourceInterval;
int					Server::hotSpotChangeInterval;
vector<glm::vec3>	Server::hotSpotLocations;
glm::vec3			Server::currentHotSpot;
int					Server::currentHotSpotIndex;

// Private Vars
HANDLE		packetBufMutex;

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
	packetBufMutex = CreateMutex(NULL, true, NULL);


	// GAME MECHANICS
	ConfigSettings::getConfig()->getValue("ResourcePerInterval", resourcePerInterval);
	ConfigSettings::getConfig()->getValue("ResourceHotSpotBonusPerInterval", resourceHotSpotBonusPerInterval);
	ConfigSettings::getConfig()->getValue("ResourceInterval", resourceInterval);
	ConfigSettings::getConfig()->getValue("HotSpotChangeInterval", hotSpotChangeInterval);

	timeUntilResourceBonus = resourcePerInterval;
	timeUntilHotSpotChange = hotSpotChangeInterval;

	// Resource Locations
	hotSpotLocations.push_back(glm::vec3(75, 0, 0));
	hotSpotLocations.push_back(glm::vec3(35, 0, 0));
	hotSpotLocations.push_back(glm::vec3(105, 0, 0));

	currentHotSpotIndex = 0;
	currentHotSpot = hotSpotLocations[currentHotSpotIndex];

	// Load Height Map
	string heightMapFile;
	ConfigSettings::getConfig()->getValue("HeightMapFile", heightMapFile);
	//World::readInHeightMapFromFile(heightMapFile);
	
	return 0;
}

// Server processes a given message
void Server::processIncomingMsg(char * msg, struct sockaddr_in *source) {
	struct packet *p = (struct packet *) msg;
	//printPacket(p);
	//printf("[SERVER]: Received a packet. eventId: %d\n", p->eventId);

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

			sendHotSpotUpdate(currentHotSpot.x, currentHotSpot.y, currentHotSpot.z);
		}
		else
		{
			printf("[SERVER]: Too many players!\n");
		}
	}
	else if (p->eventId == STATIC_OBJECT_CREATION_EVENT)
	{
		struct staticObjectPacket *staticObjPkt = (struct staticObjectPacket *)p;
		if (staticObjPkt->playerId == 0) // only first player is authorized to create static objects
		{
			struct staticObject tmp;
			memcpy(&tmp, &staticObjPkt->object, sizeof(struct staticObject));
			dynamicWorld.addStaticObject(tmp);
			printf("[SERVER]: Added a static object. Now have %d static objects\n", dynamicWorld.getNumStaticObjects());
			tmp.aabb.print();
		}
	}
	else if (p->eventId == RELOAD_CONFIG_FILE)
	{
		ConfigSettings::getConfig()->reloadSettingsFile();
		printf("[SERVER]: Config file reloaded.\n");

		struct packet reloadPkt;
		reloadPkt.eventId = RELOAD_CONFIG_FILE;

		// Send reload config file to everyone
		for (int i = 0; i < playerCount; i++)
			Server::sendMsg((char *) &reloadPkt, sizeof(reloadPkt), &players[i].clientAddress);
	}
	else
	{
		if (packetBufferCount < PACKET_BUFFER_SIZE)
		{
			// Lock Mutex: Writing into packet buffer 
			WaitForSingleObject(packetBufMutex, MAX_SERVER_PROCESS_RATE);

			memcpy(packetBuffer[packetBufferCount].msg, msg, BUFSIZE);
			packetBufferCount++;

			// Releaes Mutex 
			ReleaseMutex(packetBufMutex);
		}
	}
}

DWORD WINAPI Server::bufferProcessorThread(LPVOID param)
{
	long elapsed;
	printf("[SERVER]: Process buffer thread started\n");

	while (1)
	{
		elapsed = 0;
		stopwatch.reset();

		processBuffer();

		elapsed = MAX_SERVER_PROCESS_RATE - stopwatch.getElapsedMilliseconds();
		if (elapsed >= 0)
		{
			Sleep(elapsed);
		}
		/*
		else
		{
			// Note: Hopefully this case doesn't happen, means processing time is taking longer than rate
			Sleep(MAX_SERVER_PROCESS_RATE);	
		}
		*/
	}
}

void Server::broadcastDynamicWorld()
{
	char buf[BUFSIZE];
	memset(buf, 0, BUFSIZE);
	int size = dynamicWorld.serialize(buf);

	for (int i = 0; i < playerCount; i++)
	{
		Server::sendMsg(buf, size, &players[i].clientAddress);
	}
}



void Server::processBuffer()
{

	dynamicWorld.updatePlayerBuffs(MAX_SERVER_PROCESS_RATE);
	//dynamicWorld.applyPhysics();
	dynamicWorld.applyGravity();
	dynamicWorld.checkPlayersCollideWithTrap();
	updateResources();
	
	// Lock Mutex: Process exisiting packet buf without adding more packets 
	WaitForSingleObject(packetBufMutex, MAX_SERVER_PROCESS_RATE);

	for (int i = 0; i < packetBufferCount; i++)
	{
		struct packet *p = (struct packet *) packetBuffer[i].msg;
		//printf("[SERVER]: Now processing packet at buffer %d with eventId: %d\n", i, p->eventId);
		
		switch (p->eventId)
		{
			case PLAYER_UPDATE_EVENT:
			{
				// Update player state
				struct playerUpdatePacket *updatePacket = (struct playerUpdatePacket *)p;
				dynamicWorld.updatePlayer(updatePacket->playerObj);
				break;
			}

			case MOVE_EVENT:
			{
				struct moveEventPacket *movePkt = (struct moveEventPacket *)p;
				dynamicWorld.processMoveEvent(movePkt);
				break;
			}
			case LOOK_EVENT:
			{
				struct lookEventPacket *lookPkt = (struct lookEventPacket *)p;
				dynamicWorld.processLookEvent(lookPkt);
				break;
			}

			case JUMP_EVENT:
			{
				struct jumpEventPacket *jumpPkt = (struct jumpEventPacket*)p;
				dynamicWorld.processJumpEvent(jumpPkt);
				break;
			}

			case SPAWN_TRAP_REQUEST:
			{
				struct spawnTrapPacket *trapPkt = (struct spawnTrapPacket *)p;
				dynamicWorld.addTrap(trapPkt->trap);
				break;
			}
			case KNIFE_HIT_EVENT:
			{
				struct knifeHitPacket *knifePkt = (struct knifeHitPacket *)p;

				// TODO (ktngo): Maybe have a more general player interaction method?
				playerObject *player = &dynamicWorld.playerMap[knifePkt->playerId];
				playerObject *target = &dynamicWorld.playerMap[knifePkt->targetId];

				float knifeRange = 0.0f;
				ConfigSettings::getConfig()->getValue("KnifeRange", knifeRange);

				glm::vec3 lookAt = player->cameraObject.cameraLookAt;
				glm::vec3 center = player->cameraObject.cameraCenter;
				glm::vec3 difVec = lookAt - center;
				glm::vec3 hitPt = center + (knifeRange * difVec);

				if (hitPt.x >= target->aabb.minX && hitPt.x <= target->aabb.maxX
					&& hitPt.y >= target->aabb.minY && hitPt.y <= target->aabb.maxY
					&& hitPt.z >= target->aabb.minZ && hitPt.z <= target->aabb.maxZ)
				{
					dynamicWorld.playerDamage(player, target, KNIFE_HIT_DMG);
				}
				break;
			}

			default:
				printf("[SERVER]: Unknown event at buffer %d, eventId: %d\n", i, p->eventId);
				break;
		}
	}

	packetBufferCount = 0;

	if (playerCount > 0) {
		broadcastDynamicWorld();
	}

	// Release Mutex
	ReleaseMutex(packetBufMutex);
}

// Server receives a message
int Server::receiveMsg(char * msg, struct sockaddr_in *source) {
	int len = sizeof(struct sockaddr_in);

	if (recvfrom(i_sockfd, msg, BUFSIZE, 0, (struct sockaddr *)source, &len) < 0) {
		int error = WSAGetLastError();
		printf("[SERVER]: server.cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	//printf("[SERVER]: Message received.\n");

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

void Server::updateResources()
{
	timeUntilHotSpotChange -= MAX_SERVER_PROCESS_RATE;
	timeUntilResourceBonus -= MAX_SERVER_PROCESS_RATE;
	if (timeUntilResourceBonus < 0)
	{
		timeUntilResourceBonus = resourceInterval;

		for(map<int,struct playerObject>::iterator it = dynamicWorld.playerMap.begin(); it != dynamicWorld.playerMap.end(); ++it)
		{
			it->second.resources += resourcePerInterval;

			// Hot spot bonus
			glm::vec3 pos = it->second.position;

			// TODO: 
			if (glm::distance(pos, currentHotSpot) < 10)
				it->second.resources += resourceHotSpotBonusPerInterval;
		}
	}

	if (timeUntilHotSpotChange < 0)
	{
		timeUntilHotSpotChange = hotSpotChangeInterval;
		currentHotSpotIndex += 1; 
		currentHotSpotIndex = currentHotSpotIndex % hotSpotLocations.size();

		currentHotSpot = hotSpotLocations[currentHotSpotIndex];
		sendHotSpotUpdate(currentHotSpot.x, currentHotSpot.y, currentHotSpot.z);
	}
}

void Server::sendHotSpotUpdate(int x, int y, int z)
{
	struct hotSpotPacket p;
	p.eventId = HOT_SPOT_UPDATE;
	p.x = x;
	p.y = y;
	p.z = z;
	
	// Send Message
	for (int i = 0; i < playerCount; i++)
		Server::sendMsg((char *) &p, sizeof(p), &players[i].clientAddress);

}

void Server::printPacket(struct packet *p)
{
	switch (p->eventId)
	{
		case INIT_REQUEST_EVENT:
		{
			printf("=====================================\n");
			printf("INIT REQUEST PACKET\n");
			printf("=====================================\n");
			break;
		}
		case PLAYER_UPDATE_EVENT:
		{
			struct playerObject *player = &((struct playerUpdatePacket *)p)->playerObj;
			printf("=====================================\n");
			printf("PLAYER UPDATE PACKET\n");
			printf("ID: %d\n", player->id);
			printf("x: %.1f, y:%.1f, z:%.1f\n", player->position.x, player->position.y, player->position.z);
			printf("=====================================\n");
			
			break;
		}
		case MOVE_EVENT:
		{
			struct moveEventPacket *movePkt = (struct moveEventPacket *)p;
			printf("=====================================\n");
			printf("MOVE EVENT PACKET\n");
			printf("=====================================\n");
			break;
		}
		case JUMP_EVENT: 
		{
			struct jumpEventPacket *jumpPkt = (struct jumpEventPacket *)p;
			printf("=====================================\n");
			printf("JUMP EVENT PACKET\n");
			printf("=====================================\n");
			break;
		}
		default:
		{
			printf("=====================================\n");
			printf("Unknown eventId: %d\n", p->eventId);
			printf("=====================================\n");
			break;
		}
	}
}
