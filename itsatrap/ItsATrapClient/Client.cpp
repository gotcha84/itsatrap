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
int					Client::channelingResourceId;
bool				Client::okChannel;

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
	bool good = false;
	while (!good)
	{
		Client::receiveMsg();

		struct packet *p = (struct packet *)c_msg;
		if (p->eventId == INIT_RESPONSE_EVENT)
		{
			// Init response
			struct initResponsePacket *irp = (struct initResponsePacket *)p;
			printf("[CLIENT]: Server gave me ID: %d\n", irp->givenPlayerId);
			playerId = irp->givenPlayerId;
			good = true;
		}
		else
		{
			Client::sendMsg((char *)&initPacket, sizeof(initPacket));
			printf("[CLIENT]: Init request sent again\n");
		}

		Sleep(1000);
	}
	

	okChannel = false;

	Client::startRefresherThread();
	Client::startReceiverThread();
	Client::startSenderThread();
	

	return 0;
	
}

void Client::startReceiverThread()
{
	DWORD tmp = 0;
	CreateThread(NULL, 0, Client::receiverThread, NULL, 0, &tmp);
	cout << "[CLIENT]: Receiver thread started" << endl;
}

void Client::startSenderThread()
{
	DWORD tmp = 0;
	CreateThread(NULL, 0, Client::senderThread, NULL, 0, &tmp);
	cout << "[CLIENT]: Sender thread started" << endl;
}

void Client::startRefresherThread()
{
	DWORD tmp = 0;
	CreateThread(NULL, 0, Client::refresherThread, NULL, 0, &tmp);
	cout << "[CLIENT]: Refresher thread started" << endl;
}

DWORD WINAPI Client::receiverThread(LPVOID param)
{
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
				if (client != nullptr) {
					handleUpdateWorldFromServer(&world);
				}
			}
			else if (p->eventId == RELOAD_CONFIG_FILE)
			{
				printf("[CLIENT]: Server told me to reload config file.\n");
				ConfigSettings::getConfig()->reloadSettingsFile();
			}
			else if (p->eventId == DISCONNECT_PLAYER_EVENT)
			{
				struct disconnectPlayerPacket *dcP = (struct disconnectPlayerPacket *)p;
				handleDisconnectPlayer(dcP->disconnectedPlayerId);
			}
			else if (p->eventId == HOT_SPOT_UPDATE)
			{
				struct resourceNodePacket *packet = (struct resourceNodePacket *) p;
				updateActiveResourceNode(packet->id);
			}
			else if (p->eventId == CHANNELING_PERMISSION)
			{
				struct resourceNodePacket *packet = (struct resourceNodePacket *) p;
				startChanneling(packet->id);
			}
			else if (p->eventId == NEW_OWNER_RESOURCE_UPDATE_EVENT)
			{
				struct resourceHitPacket *packet = (struct resourceHitPacket *) p;
				client->level.setOwnerToResourceNode(packet->resourceId, packet->playerId);
			}
			else if (p->eventId == CLEAR_CHANNEL_BAR)
			{
				struct refreshPacket *packet = (struct refreshPacket *) p;
				okChannel = false;
				client->root->m_hud->m_progressTime = -1;
			}
			else if (p->eventId == INFO_MESSAGE_EVENT)
			{
				struct infoMsgPacket *info = (struct infoMsgPacket *)p;
				client->root->getPlayer()->m_infoMsg.setMessage(info->msg);
			}
			else if (p->eventId == GAME_OVER_EVENT)
			{
				//int temp = client->root->m_elapsedGameTime;
				client->root->m_gameOver = true;
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
	char dummy[65536];

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

void Client::sendStaticObject(AABB objectBB, bool isDecoration)
{
	struct staticObjectPacket packet = {};
	packet.eventId = STATIC_OBJECT_CREATION_EVENT;
	packet.playerId = playerId;
	packet.object.aabb.minX = objectBB.minX;
	packet.object.aabb.minY = objectBB.minY;
	packet.object.aabb.minZ = objectBB.minZ;
	packet.object.aabb.maxX = objectBB.maxX;
	packet.object.aabb.maxY = objectBB.maxY;
	packet.object.aabb.maxZ = objectBB.maxZ;
	packet.object.isDecoration = isDecoration;

	sendMsg((char *)&packet, sizeof(struct staticObjectPacket));
}

void Client::sendStaticResourceObject(AABB resourceBB, int id)
{
	struct staticResourceObjectPacket packet = {};
	packet.eventId = STATIC_RESOURCE_OBJECT_CREATION_EVENT;
	packet.playerId = playerId;
	packet.object.aabb.minX = resourceBB.minX;
	packet.object.aabb.minY = resourceBB.minY;
	packet.object.aabb.minZ = resourceBB.minZ;
	packet.object.aabb.maxX = resourceBB.maxX;
	packet.object.aabb.maxY = resourceBB.maxY;
	packet.object.aabb.maxZ = resourceBB.maxZ;
	packet.object.id = id;

	sendMsg((char *)&packet, sizeof(struct staticResourceObjectPacket));
}

void Client::sendSpawnTrapEvent(struct trapObject t)
{
	struct spawnTrapPacket p;
	p.eventId = SPAWN_TRAP_REQUEST;
	memcpy(&p.trap, &t, sizeof(struct trapObject));

	printf("Sending trap with type %d\n", t.type);

	p.trap.ownerId = getPlayerId();
	sendMsg((char *)&p, sizeof(struct spawnTrapPacket));
}

void Client::sendKnifeHitEvent()
{
	struct knifeHitPacket p;
	p.eventId = KNIFE_HIT_EVENT;
	p.playerId = getPlayerId();

	sendMsg((char *)&p, sizeof(struct knifeHitPacket));
}

void Client::sendChannelAttemptEvent(int resourceId)
{
	struct resourceHitPacket p;
	p.eventId = RESOURCE_HIT_EVENT;
	p.playerId = getPlayerId();
	p.resourceId = resourceId;

	sendMsg((char *)&p, sizeof(struct resourceHitPacket));
}

void Client::updateActiveResourceNode(int id)
{
	if (client == nullptr)
	{
		return;
	}

	// Shut down the old node that was active
	client->level.disableCurrentResourceNode();
	client->level.activateResourceNode(id);
}

void Client::sendChannelCompletedEvent(int resourceId)
{
	struct resourceHitPacket p = {}; 
	p.eventId = CHANNELING_COMPLETE;
	p.playerId = getPlayerId();
	p.resourceId = resourceId;

	sendMsg((char*)&p, sizeof(struct resourceHitPacket));
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
	// TODO: Would cancel channeling if currently channeling
}

void Client::sendJumpEvent()
{
	jumpEvent = true;
	// TODO: Would cancel channeling if currently channeling
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

DWORD WINAPI Client::refresherThread(LPVOID)
{
	while (true)
	{
		int clientRefreshInterval = 200;
		ConfigSettings::getConfig()->getValue("ClientRefreshInterval", clientRefreshInterval);
		Sleep(clientRefreshInterval);
		struct refreshPacket p = {};
		p.eventId = REFRESH_EVENT;
		p.playerId = getPlayerId();
		sendMsg((char *)&p, sizeof(p));
	}
}

void Client::handleDisconnectPlayer(int id)
{
	printf("[CLIENT]: PLAYER %d HAS BEEN DISCONNECTED!\n", id);
	if (client->players[id] != nullptr)
	{
		client->root->removeChild(client->players[id]);
		client->players.erase(id);
		client->objects.erase(id);
	}
}

void Client::startChanneling(int resourceId) {
	channelingResourceId = resourceId;

	okChannel = true;
	DWORD tmp = 0;
	CreateThread(NULL, 0, Client::channelingThread, NULL, 0, &tmp);
	// ExitThread?
}

DWORD WINAPI Client::channelingThread(LPVOID arg) {
	int channelTime;
	int timeElapsed = 0;
	ConfigSettings::getConfig()->getValue("ChannelTime", channelTime);

	cout << "Channeling Node..." << endl;
	while (okChannel && timeElapsed < channelTime) {
		if (client->root->m_hud->m_progressTime == -1) {
			client->root->m_hud->m_progressTime = 0;
		}
		else {
			client->root->m_hud->m_progressTime+=10;
		}
		timeElapsed += channelTime / 10.0f;
		Sleep(channelTime/10.0f);
	}

	okChannel = false;
	client->root->m_hud->m_progressTime = -1;
	
	cout << "Channel Complete!" << endl;
	// Send Message to server
	Client::sendChannelCompletedEvent(channelingResourceId);
	channelingResourceId = -1;
	return 0;
}

void Client::sendAABBInfo(int type, AABB aabb)
{
	struct aabbInfoPacket p;
	p.eventId = AABB_INFO;
	p.playerId = getPlayerId();
	p.type = type;
	p.aabb = aabb;

	sendMsg((char *)&p, sizeof(p));
}