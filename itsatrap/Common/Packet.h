#ifndef PACKET_H
#define PACKET_H

#include <../glm/glm/glm.hpp>
#include <../glm/glm/ext.hpp>


#include "NetworkObjects.h"

#define INIT_REQUEST_EVENT 1
#define INIT_RESPONSE_EVENT 2
#define PLAYER_UPDATE_EVENT 3
#define WORLD_UPDATE_EVENT 4
#define STATIC_OBJECT_CREATION_EVENT 5
#define SPAWN_TRAP_REQUEST 6
#define KNIFE_HIT_EVENT 7
#define HOT_SPOT_UPDATE 8
#define RELOAD_CONFIG_FILE 9
#define STATIC_RAMP_OBJECT_CREATION_EVENT 13
#define PLAYER_ACTION_EVENT 14


// Superclass of all packets
struct packet {
	int eventId;	
};


// Initial connection between client/server
struct initResponsePacket
{
	int eventId;
	int givenPlayerId;
};


struct playerUpdatePacket
{
	int eventId;
	struct playerObject playerObj;
};

struct staticObjectPacket
{
	int eventId;
	int playerId;
	struct staticObject object;
};

struct staticRampObjectPacket
{
	int eventId;
	int playerId;
	struct staticObject object;
};

struct spawnTrapPacket
{
	int eventId;
	struct trapObject trap;
};

struct knifeHitPacket
{
	int eventId;
	int playerId;
	int targetId;
};

struct hotSpotPacket
{
	int eventId;
	int x,y,z;
};


#define NUM_DIRECTIONS 4
enum Direction {
	// MUST BE 0,1,2,...
	FORWARD = 0, BACKWARD = 1, LEFT= 2, RIGHT = 3
};

struct playerActionPacket
{
	int eventId;
	int playerId;
	bool moveEvents[NUM_DIRECTIONS];
	bool jump, cameraChanged;
	struct cameraObject cam;
};

#endif
