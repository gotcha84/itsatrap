#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

#include <../glm/glm/glm.hpp>
#include <../glm/glm/ext.hpp>

#include "AABB.h"

#define EVENT_ADD_TRAP 1
#define EVENT_REMOVE_TRAP 2

#define TYPE_FREEZE_TRAP 1
#define TYPE_TRAMPOLINE_TRAP 2
#define TYPE_SLOW_TRAP 3
#define TYPE_PUSH_TRAP 4
#define TYPE_LIGHTNING_TRAP 5


enum PhysicsStates {
	None = 0,
	Colliding = 1,
	Moving = 2,
	Climbing = 3,
	Sliding = 4,
	WallJumping = 5,
	PullingUp = 6,
	HoldingEdge = 7,
	WallRunning = 8
};

enum innerStates {
	Off = 0, // same as readjusted

	StartAdjusting = 1,
	StartAdjusted = 2,
	StartReadjusting = 3,
	StartReadjusted = 4,

	MidAdjusting = 5,
	MidAdjusted = 6,
	MidReadjusting = 7,
	MidReadjusted = 8,

	EndAdjusting = 9,
	EndAdjusted = 10,
	EndReadjusting = 11,
	EndReadjusted = 12,

	Done = 13
};

//struct aabb {
//	float			minX, minY, minZ, maxX, maxY, maxZ;
//};

struct miniStateInfo {
	float lookX, lookY;
	float lookXNumFramesFraction, lookYNumFramesFraction;
	float lookXIncrement, lookYIncrement;
	glm::vec3 veloDiff;
	int counter;

};

struct holderInfo {
	glm::vec3		position, lookAt, center, up, velDiff, vel, xRotated, yRotated;
};

struct stateInfo {
	float initialX, intialY;

	float lookXNumFrames, lookYNumFrames;

	struct miniStateInfo start;
	//struct miniStateInfo startAdjust;
	//struct miniStateInfo startReadjust;
	struct miniStateInfo middle;
	struct miniStateInfo end;
	//struct miniStateInfo endAdjust;
	//struct miniStateInfo readAjust;

	float goalCamX, goalCamY;
	float goalVelo;

	struct holderInfo holder;

	innerStates innerState;	
};

struct physicsObject {
	clock_t stateStart;
	clock_t lastTeleported;
	clock_t lastSlid;

	glm::vec3 velocity;
	glm::vec3 velocityDiff;
	glm::vec3 position;
	glm::vec3 lastMoved;

	bool triedToRun;
	bool canJump;
	bool feetPlanted;

	PhysicsStates currentState;
};

struct cameraObject {

	glm::vec3 cameraCenter;
	glm::vec3 cameraLookAt;
	glm::vec3 cameraUp;
	glm::vec3 camZ;
	glm::vec3 camX;

	float xRotated;
	float yRotated;
	
};

struct playerObject {
    int				id, health, numKills, numDeaths;
	int				stunDuration, slowDuration;
	int				timeUntilRespawn;
	int				resources;
	int				onTopOfBuildingId;
	bool			deathState;
	AABB			aabb;

	struct cameraObject			cameraObject;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 velocityDiff;
	bool feetPlanted;
	int currState;
	int currminiState;
	int interactingWithBuildingId;
};

struct trapObject {
    int				id;
	int				ownerId;
	int				type;
	int				eventCode;
	float			x, y, z;
	float			rotationAngle;
	AABB			aabb;
};

struct staticObject {
	AABB			aabb;
};


#endif
