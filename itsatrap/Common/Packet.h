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
#define MOVE_EVENT 10
#define JUMP_EVENT 11


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

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

struct moveEventPacket
{
	int eventId;
	int playerId;
	Direction direction;
	float xRotation, yRotation;
	glm::vec3 test;
};

struct jumpEventPacket
{
	int eventId;
	int playerId;
};

#endif
