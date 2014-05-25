#include "Client.h"

extern ClientInstance *client;

// Variables
struct sockaddr_in	Client::myAddress, Client::serverAddress;
int					Client::len;
int 				Client::i_sockfd;
char 				Client::c_msg[BUFSIZE];
WSADATA				Client::wsaData;
int					Client::playerId;
bool				Client::moveEvents[NUM_DIRECTIONS] = {};
bool				Client::jumpEvent, Client::cameraChanged;
cameraObject		Client::playerCam = {};

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
	string serverIP;
	ConfigSettings::getConfig()->getValue("ServerAddress", serverIP);
	cout << "[CLIENT]: Server's IP address: " << serverIP << endl;
	serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
	serverAddress.sin_port = htons(SERVER_PORT);
	
	// Send init event
	struct packet initPacket;
	initPacket.eventId = INIT_REQUEST_EVENT;
	Client::sendMsg((char *) &initPacket, sizeof(initPacket));
	printf("[CLIENT]: Init request sent\n");
	
	// Receives Server response
	Client::receiveMsg(); // TODO: what if server doesn't respond?

	struct packet *p = (struct packet *)c_msg;
	if (p->eventId == INIT_RESPONSE_EVENT)
	{
		// Init response
		struct initResponsePacket *irp = (struct initResponsePacket *)p;
		printf("[CLIENT]: Server gave me ID: %d\n", irp->givenPlayerId);
		playerId = irp->givenPlayerId;
	}

	Client::startReceiverThread();
	Client::startSenderThread();

	return 0;
	
}

void Client::startReceiverThread()
{
	DWORD tmp = 0;
	CreateThread(NULL, 0, Client::receiverThread, NULL, 0, &tmp);
}

void Client::startSenderThread()
{
	DWORD tmp = 0;
	CreateThread(NULL, 0, Client::senderThread, NULL, 0, &tmp);
}

DWORD WINAPI Client::receiverThread(LPVOID param)
{
	
	printf("[CLIENT]: Receiver thread started\n");
	while (1)
	{
		if (Client::receiveMsg() == 0)
		{
			struct packet *p = (struct packet *) c_msg;

			if (p->eventId == WORLD_UPDATE_EVENT)
			{
				
				// World update. 
				// This variable 'world' is the world given by the server
				DynamicWorld world(p);
				handleUpdateWorldFromServer(&world);
			}
			else if (p->eventId == HOT_SPOT_UPDATE)
			{
				struct hotSpotPacket *hsp = (struct hotSpotPacket *) p;
				updateHotSpot(hsp->x, hsp->y, hsp->z);
			}
			else if (p->eventId == RELOAD_CONFIG_FILE)
			{
				printf("[CLIENT]: Server told me to reload config file.\n");
				ConfigSettings::getConfig()->reloadSettingsFile();
			}
		}
	}
}


void Client::sendPlayerUpdate(struct playerObject player)
{
	printf("[CLIENT]: WARNING! sendPlayerUpdate is called! This method is deprecated\n");
	struct playerUpdatePacket p;
	p.eventId = PLAYER_UPDATE_EVENT;
	memcpy(&p.playerObj, &player, sizeof(struct playerObject));
	Client::sendMsg((char *)&p, sizeof(p));
}


// Client receives messages from the server
int Client::receiveMsg() {

	int bytesReceived = 0;
	bytesReceived = recvfrom(i_sockfd, c_msg, BUFSIZE, 0, (struct sockaddr *) &serverAddress, &len);

	if (bytesReceived < 0) {
		int error = WSAGetLastError();
		//printf("[CLIENT]: client->cpp - recvfrom failed with error code %d\n", error);
		return 1;
	}

	//printf("[CLIENT]: received message of size %d\n", bytesReceived);

	return 0;
}

// Client sends message to the server
int Client::sendMsg(char * msg, int len) {
	if (sendto(i_sockfd, msg, len, 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		int error = WSAGetLastError();
		printf("[CLIENT]: client.cpp - sendto failed with error code %d\n", error);
		return 1;
	}

	//printf("[CLIENT]: Message of length %d sent.\n", len);

	return 0;
}

int Client::getPlayerId()
{
	return playerId;
}

void Client::sendStaticObject(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	struct staticObjectPacket packet = {};
	packet.eventId = STATIC_OBJECT_CREATION_EVENT;
	packet.playerId = playerId;
	packet.object.aabb.minX = minX;
	packet.object.aabb.minY = minY;
	packet.object.aabb.minZ = minZ;
	packet.object.aabb.maxX = maxX;
	packet.object.aabb.maxY = maxY;
	packet.object.aabb.maxZ = maxZ;

	sendMsg((char *)&packet, sizeof(struct staticObjectPacket));
}

void Client::sendStaticRampObject(AABB rampBB)
{
	struct staticRampObjectPacket packet = {};
	packet.eventId = STATIC_RAMP_OBJECT_CREATION_EVENT;
	packet.playerId = playerId;
	packet.object.aabb.minX = rampBB.minX;
	packet.object.aabb.minY = rampBB.minY;
	packet.object.aabb.minZ = rampBB.minZ;
	packet.object.aabb.maxX = rampBB.maxX;
	packet.object.aabb.maxY = rampBB.maxY;
	packet.object.aabb.maxZ = rampBB.maxZ;

	sendMsg((char *)&packet, sizeof(struct staticRampObjectPacket));
}

void Client::sendSpawnTrapEvent(struct trapObject t)
{
	struct spawnTrapPacket p;
	p.eventId = SPAWN_TRAP_REQUEST;
	memcpy(&p.trap, &t, sizeof(struct trapObject));

	printf("Sending trap with type %d\n", t.type);

	sendMsg((char *)&p, sizeof(struct spawnTrapPacket));
}

void Client::sendKnifeHitEvent(int targetId)
{
	struct knifeHitPacket p;
	p.eventId = KNIFE_HIT_EVENT;
	p.playerId = getPlayerId();
	p.targetId = targetId;

	sendMsg((char *)&p, sizeof(struct knifeHitPacket));
}

void Client::updateHotSpot(int x, int y, int z)
{
	if (client == nullptr)
		return;

	if (client->hotSpot != nullptr)
	{
		client->root->removeChild(client->hotSpot);
	}

	// CONE node
	sg::MatrixTransform *mt = new sg::MatrixTransform();
	client->root->addChild(mt);
	client->hotSpot = mt;

	sg::Cone *cone = new sg::Cone();
	mt->addChild(cone);
	mt->setMatrix(glm::translate(glm::vec3(x,y,z)) * glm::scale(glm::vec3(10,10,10)));
}

void Client::sendReloadConfigFile()
{
	struct packet p = {};
	p.eventId = RELOAD_CONFIG_FILE;
	sendMsg((char *)&p, sizeof(struct packet));

	printf("[CLIENT]: Sent reload config file\n");
}

void Client::sendMoveEvent(Direction dir)
{
	moveEvents[dir] = true;
}

void Client::sendJumpEvent()
{
	jumpEvent = true;
}

void Client::sendLookEvent(struct cameraObject cam)
{
	playerCam = cam;
	cameraChanged = true;
}

DWORD WINAPI Client::senderThread(LPVOID)
{
	while (1)
	{
		int sleep = 33;
		ConfigSettings::getConfig()->getValue("ClientSendRate", sleep);
		Sleep(sleep);

		bool sendUpdate = false;
		for (int i = 0; i < NUM_DIRECTIONS; i++)
		{
			if (moveEvents[i])
				sendUpdate = true;
		}
		if (jumpEvent)
			sendUpdate = true;
		if (cameraChanged)
			sendUpdate = true;

		if (sendUpdate)
		{
			// Send packet
			struct playerActionPacket p = {};
			p.eventId = PLAYER_ACTION_EVENT;
			p.playerId = getPlayerId();
			memcpy(p.moveEvents, moveEvents, sizeof(moveEvents));
			p.jump = jumpEvent;
			p.cameraChanged = cameraChanged;
			p.cam = playerCam;
			sendMsg((char *)&p, sizeof(p));
		}

		// Reset static vars
		memset(moveEvents, 0, sizeof(moveEvents));
		jumpEvent = false;
		cameraChanged = false;
	}
}