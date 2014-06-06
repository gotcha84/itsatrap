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
int					Server::maxServerProcessRate;
int					Server::elapsedGameTimeMS;

vector<int>			Server::resourceNodeLocations;
int					Server::currentActiveResourceNodeIndex;
int					Server::currentResourceOwner;
int					Server::channelingPlayer;
bool				Server::isChanneling;

bool				Server::physicsReady;
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

	// Load Height Map
	string heightMapFile;
	ConfigSettings::getConfig()->getValue("HeightMapFile", heightMapFile);
	//World::readInHeightMapFromFile(heightMapFile); // ANURAG
	printf("[SERVER]: Height map loaded.\n");

	while(1) {
		struct sockaddr_in source;
		if (Server::receiveMsg(c_msg, &source) == 0)	
			Server::processIncomingMsg(c_msg, &source);
	}
}

// Inital setup for the server
int Server::initialize() {

	ConfigSettings::getConfig()->getValue("MaxServerProcessRate", maxServerProcessRate);

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
	currentActiveResourceNodeIndex = 0;
	currentResourceOwner = -1;
	channelingPlayer = -1;
	isChanneling = false;
	physicsReady = false;

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
			player.active = true;
			player.timeUntilInactive = 20000;
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
		if (staticObjPkt->playerId == 0) // only first player is authorized to create static objects
		{
			struct staticObject tmp;
			memcpy(&tmp, &staticObjPkt->object, sizeof(struct staticObject));
			dynamicWorld.addStaticObject(tmp);

			//int id = dynamicWorld.getNumStaticObjects() - 1;
			//World::updateStructuresMap(dynamicWorld.getStaticObjectBB(id), id);

			printf("[SERVER]: Added a static object. Now have %d static objects\n", dynamicWorld.getNumStaticObjects());
			tmp.aabb.print();
		}

		elapsedGameTimeMS = 0;
		int MaxBuildings = 0;
		ConfigSettings::getConfig()->getValue("MaxBuildings", MaxBuildings);
		if (dynamicWorld.getNumStaticObjects() >= MaxBuildings) {
			cout << "physics ready yo" << endl;
			physicsReady = true;
		}
	}
	else if (p->eventId == STATIC_RESOURCE_OBJECT_CREATION_EVENT)
	{
		struct staticResourceObjectPacket *staticObjPkt = (struct staticResourceObjectPacket *)p;
		if (staticObjPkt->playerId == 0) // only first player is authorized to create static objects
		{
			struct staticResourceObject tmp;
			memcpy(&tmp, &staticObjPkt->object, sizeof(struct staticResourceObject));
			dynamicWorld.addStaticResourceObject(tmp);
			printf("[SERVER]: Added a static resource object. Now have %d static resource objects\n", dynamicWorld.getNumStaticResourceObjects());
			tmp.aabb.print();
			printf("ResourceId: %d\n", tmp.id);

			resourceNodeLocations.push_back(tmp.id);
			sendActiveNodeUpdate(resourceNodeLocations[currentActiveResourceNodeIndex]);
		}
	}
	else if (p->eventId == RELOAD_CONFIG_FILE)
	{
		ConfigSettings::getConfig()->reloadSettingsFile();
		printf("[SERVER]: Config file reloaded.\n");

		ConfigSettings::getConfig()->getValue("MaxServerProcessRate", maxServerProcessRate);

		struct packet reloadPkt;
		reloadPkt.eventId = RELOAD_CONFIG_FILE;

		// Send reload config file to everyone
		Server::broadcastMsg((char *) &reloadPkt, sizeof(reloadPkt));
	}
	else if (p->eventId == REFRESH_EVENT)
	{
		struct refreshPacket *rPkt = (struct refreshPacket *)p;
		ConfigSettings::getConfig()->getValue("TimeUntilDisconnect", players[rPkt->playerId].timeUntilInactive);
	}
	else if (p->eventId == AABB_INFO)
	{
		struct aabbInfoPacket *aabbPkt = (struct aabbInfoPacket *)p;
		if (aabbPkt->playerId == 0)
			dynamicWorld.addAABBInfo(aabbPkt->type, aabbPkt->aabb);
	}
	else
	{
		if (packetBufferCount < PACKET_BUFFER_SIZE)
		{
			// Lock Mutex: Writing into packet buffer 
			WaitForSingleObject(packetBufMutex, maxServerProcessRate);

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

	while (1) {
		elapsed = 0;
		stopwatch.reset();

		processBuffer();

		elapsed = maxServerProcessRate - stopwatch.getElapsedMilliseconds();
		if (elapsed >= 0) {
			Sleep(elapsed);
		}
	}
}

void Server::broadcastDynamicWorld()
{
	char buf[BUFSIZE];
	memset(buf, 0, BUFSIZE);
	int size = dynamicWorld.serialize(buf);

	Server::broadcastMsg(buf, size);
}

void Server::processBuffer()
{
	checkConnection();
	elapsedGameTimeMS += maxServerProcessRate;
	dynamicWorld.updateTimings(maxServerProcessRate, elapsedGameTimeMS);		
	checkGameOver();
	updateResources();

	dynamicWorld.resetWorldInfo();

	// Lock Mutex: Process exisiting packet buf without adding more packets 
	WaitForSingleObject(packetBufMutex, maxServerProcessRate);

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

			case PLAYER_ACTION_EVENT:
			{
				struct playerActionPacket *actPkt = (struct playerActionPacket *)p;

				// Movements
				for (int i = 0; i < NUM_DIRECTIONS; i++)
				{
					if (actPkt->moveEvents[i]) {
						if (isChanneling && actPkt->playerId == channelingPlayer) resetChanneling();

						dynamicWorld.processMoveEvent(actPkt->playerId, (Direction)i);
						dynamicWorld.playerMap[actPkt->playerId].isRecalling = false;
					}
				}

				// Jump
				if (actPkt->jump) {
					if (isChanneling && actPkt->playerId == channelingPlayer) resetChanneling();

					dynamicWorld.processJumpEvent(actPkt->playerId);
					dynamicWorld.playerMap[actPkt->playerId].isRecalling = false;
				}

				// Camera
				if (actPkt->cameraChanged)
					dynamicWorld.processLookEvent(actPkt->playerId, &actPkt->cam);

				if (actPkt->recall)
					dynamicWorld.playerMap[actPkt->playerId].isRecalling = true;

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
				dynamicWorld.handleKnifeEvent(knifePkt->playerId);
				break;
			}
			case RESOURCE_HIT_EVENT:
			{
				struct resourceHitPacket *hitPkt = (struct resourceHitPacket *)p;
				playerObject *player = &dynamicWorld.playerMap[hitPkt->playerId];

				// Check if active node
				if (player->knifeDelay <= 0 && hitPkt->resourceId == resourceNodeLocations[currentActiveResourceNodeIndex]) {
					ConfigSettings::getConfig()->getValue("KnifeDelay", player->knifeDelay);

					if (!isChanneling) {
						channelingPlayer = player->id;

						if (currentResourceOwner != channelingPlayer || currentResourceOwner % 2 != channelingPlayer % 2) {
							isChanneling = true;

							float knifeRange = 0.0f;
							ConfigSettings::getConfig()->getValue("KnifeRange", knifeRange);

							glm::vec3 lookAt = player->cameraObject.cameraLookAt;
							glm::vec3 center = player->cameraObject.cameraCenter;
							glm::vec3 difVec = lookAt - center;
							glm::vec3 hitPt = center + (knifeRange * difVec);

							AABB target = dynamicWorld.getStaticResourceBB(hitPkt->resourceId);
							if (hitPt.x >= target.minX && hitPt.x <= target.maxX
								&& hitPt.y >= target.minY && hitPt.y <= target.maxY
								&& hitPt.z >= target.minZ && hitPt.z <= target.maxZ)
							{
								// send resourceNodePacket to client, telling them it is ok to channel
								sendPermissionToChannel(channelingPlayer, resourceNodeLocations[currentActiveResourceNodeIndex]);
							}
							else
							{
								resetChanneling();
							}
						}
					}
				}

				break;
			}
			case CHANNELING_COMPLETE:
			{
				struct resourceHitPacket *hitPkt = (struct resourceHitPacket *)p;
				if (hitPkt->playerId == channelingPlayer
					&& hitPkt->resourceId == resourceNodeLocations[currentActiveResourceNodeIndex]
					&& isChanneling) {
					currentResourceOwner = channelingPlayer;
					resetChanneling();

					sendNewResourceOwnerUpdate(currentResourceOwner, resourceNodeLocations[currentActiveResourceNodeIndex]);
				}

				break;
			}
			case REQUEST_ACTIVE_NODE_EVENT:
			{
				struct knifeHitPacket *requestPkt = (struct knifeHitPacket *)p;

				if (resourceNodeLocations.size() > 0) {
					sendActiveNodeUpdate(resourceNodeLocations[currentActiveResourceNodeIndex], requestPkt->playerId);
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
	
	if (physicsReady) {
		dynamicWorld.applyMoveEvents();
		dynamicWorld.applyTrapGravity();
		dynamicWorld.applyCollisions();
		dynamicWorld.applyPhysics();
		dynamicWorld.applyGravity();
		dynamicWorld.applyAdjustments();
		dynamicWorld.checkPlayersCollideWithTrap();
		dynamicWorld.manuallyUncollide();
	}
	// Send info messages
	sendInfoMessages();
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
		default:
		{
			printf("=====================================\n");
			printf("Unknown eventId: %d\n", p->eventId);
			printf("=====================================\n");
			break;
		}
	}
}

int Server::broadcastMsg(char * msg, int size)
{
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i].active)
			Server::sendMsg(msg, size, &players[i].clientAddress);
	}

	return 0;
}

void Server::checkConnection()
{
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i].timeUntilInactive > 0)
		{
			players[i].timeUntilInactive -= maxServerProcessRate;

			if (players[i].timeUntilInactive <= 0) {
				disconnectPlayer(i);
				//int k = 0;
			}
		}
	}
}

void Server::disconnectPlayer(int id)
{
	printf("[SERVER]: PLAYER %d HAS BEEN DISCONNECTED!\n", id);
	players[id].active = false;
	
	struct disconnectPlayerPacket p = {};
	p.eventId = DISCONNECT_PLAYER_EVENT;
	p.disconnectedPlayerId = id;
	dynamicWorld.playerMap.erase(id);

	broadcastMsg((char *)&p, sizeof(p));
}

// Called everytime the server processes a buffer (Once per cycle)
// Determines the active resource node and gives the player(owner) resources
void Server::updateResources()
{
	timeUntilHotSpotChange -= maxServerProcessRate;
	timeUntilResourceBonus -= maxServerProcessRate;

	// Distributes the resources TODO
	if (timeUntilResourceBonus < 0)
	{
		timeUntilResourceBonus = resourceInterval;

		for (map<int, struct playerObject>::iterator it = dynamicWorld.playerMap.begin(); it != dynamicWorld.playerMap.end(); ++it)
		{
			if (currentResourceOwner != -1 && it->second.id % 2 == currentResourceOwner % 2) {
				//it->second.resources += resourcePerInterval;
				it->second.resources += resourceHotSpotBonusPerInterval;
			}
		}
	}

	// Change the active node
	if (timeUntilHotSpotChange < 0 && resourceNodeLocations.size() > 0)
	{
		timeUntilHotSpotChange = hotSpotChangeInterval;		// Reset timer

		++currentActiveResourceNodeIndex;
		currentActiveResourceNodeIndex = currentActiveResourceNodeIndex % resourceNodeLocations.size();

		sendActiveNodeUpdate(resourceNodeLocations[currentActiveResourceNodeIndex]);
		currentResourceOwner = -1;
		resetChanneling();
	}
}

// Sends messages to clients about new location of resource node
void Server::sendActiveNodeUpdate(int resourceId)
{
	struct resourceNodePacket p;
	p.eventId = HOT_SPOT_UPDATE;
	p.id = resourceId;

	// Send Message
	Server::broadcastMsg((char *)&p, sizeof(p));
}

void Server::sendActiveNodeUpdate(int resourceId, int playerId)
{
	struct resourceNodePacket p;
	p.eventId = HOT_SPOT_UPDATE;
	p.id = resourceId;

	Server::sendMsg((char *)&p, sizeof(p), &players[playerId].clientAddress);
}

void Server::sendPermissionToChannel(int playerId, int resourceId)
{
	struct resourceNodePacket p;
	p.eventId = CHANNELING_PERMISSION;
	p.id = resourceId;

	Server::sendMsg((char *)&p, sizeof(p), &players[playerId].clientAddress);
}

void Server::sendNewResourceOwnerUpdate(int playerId, int resourceId)
{
	struct resourceHitPacket p;
	p.eventId = NEW_OWNER_RESOURCE_UPDATE_EVENT;
	p.playerId = playerId;
	p.resourceId = resourceId;

	Server::broadcastMsg((char *)&p, sizeof(p));
}

void Server::sendClearChannelingProgressBar(int playerId)
{
	struct refreshPacket p;
	p.eventId = CLEAR_CHANNEL_BAR;
	p.playerId = playerId;

	Server::sendMsg((char *)&p, sizeof(p), &players[playerId].clientAddress);
}

void Server::resetChanneling() {
	if (channelingPlayer != -1) {
		sendClearChannelingProgressBar(channelingPlayer);
	}

	isChanneling = false;
	channelingPlayer = -1;
}

void Server::sendInfoMessage(int destination, string msg)
{
	struct infoMsgPacket p = {};
	p.eventId = INFO_MESSAGE_EVENT;

	strncpy(p.msg, msg.c_str(), sizeof(p.msg));
	p.msg[sizeof(p.msg)-1] = 0;

	Server::sendMsg((char *)&p, sizeof(p), &players[destination].clientAddress);
}

void Server::sendInfoMessages()
{
	for (int i = 0; i < dynamicWorld.infoMsgQueue.size(); i++)
		sendInfoMessage(dynamicWorld.infoMsgQueue[i].destination, dynamicWorld.infoMsgQueue[i].msg);

	dynamicWorld.infoMsgQueue.clear();
}

void Server::sendGameOverUpdate()
{
	struct packet p = {};
	p.eventId = GAME_OVER_EVENT;

	Server::broadcastMsg((char *)&p, sizeof(p));
}

void Server::checkGameOver()
{
	// Check if the game is over or not
	int gameOverTime;
	ConfigSettings::getConfig()->getValue("GameDuration", gameOverTime);
	if (elapsedGameTimeMS >= gameOverTime)
	{
		// Broadcast Game Over message
		sendGameOverUpdate();
	}
}
