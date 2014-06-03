#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

#include <../glm/glm/glm.hpp>
#include <../glm/glm/ext.hpp>

#include "AABB.h"
#include "Stopwatch.h"

#define EVENT_ADD_TRAP 1
#define EVENT_REMOVE_TRAP 2
#define EVENT_UPDATE_TRAP 3

#define TYPE_FREEZE_TRAP 1
#define TYPE_TRAMPOLINE_TRAP 2
#define TYPE_SLOW_TRAP 3
#define TYPE_PUSH_TRAP 4
#define TYPE_LIGHTNING_TRAP 5
#define TYPE_PORTAL_TRAP 6
#define TYPE_FLASH_TRAP 7
#define TYPE_PLAYER 9

enum CameraStates {
	Client = 0,
	Server = 1
};

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
	Off = -1, // same as readjusted

	Starting = 1,
	Mid = 2,
	Waiting = 3,
	Ending = 4,

	Done = 5
};

//struct aabb {
//	float			minX, minY, minZ, maxX, maxY, maxZ;
//};

struct miniStateInfo {
	glm::vec3 camZ;
	glm::vec3 camX;
	float lookX, lookY;
	glm::vec3 camUp;
	float fraction;
	float lookXIncrement, lookYIncrement;
	glm::vec3 camUpIncrement;
	glm::vec3 velocityDiff;
	int counter;
};

struct HolderInfo {
	float			xRotated, yRotated;
	glm::vec3		position, cameraLookAt, cameraCenter, cameraUp, velocityDiff, velocity, camZ, camX;
};

struct stateInfo {
	float initialX, initialY;
	glm::vec3 initialUp;

	float numFrames;

	struct miniStateInfo Start;
	//struct miniStateInfo startAdjust;
	//struct miniStateInfo startReadjust;
	struct miniStateInfo Mid;
	struct miniStateInfo End;
	//struct miniStateInfo endAdjust;
	//struct miniStateInfo readAjust;

	float goalCamX, goalCamY;
	glm::vec3 goalCamUp;
	float goalVelo;

	struct HolderInfo Holder;
};

struct physicsObject {
	clock_t stateStart;
	clock_t lastTeleported;
	clock_t lastSlid;

	glm::vec3 velocity;
	glm::vec3 velocityDiff;
	glm::vec3 position;
	glm::vec3 lastMoved;

	bool triedForward;
	bool canJump; // not used, same purpose as below anyway
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
	int				stunDuration, slowDuration, flashDuration, hitCrosshair;
	int				timeUntilRespawn, timeUntilRegen;
	int				knifeDelay;
	int				resources;
	int				onTopOfBuildingId;
	bool			deathState;
	AABB			aabb;
	Stopwatch		stopwatch;

	struct cameraObject			cameraObject;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 velocityDiff;
	bool feetPlanted;
	bool canClimb;
	bool triedForward;
	bool triedBackward;
	bool triedLeft;
	bool triedRight;
	PhysicsStates currPhysState;
	PhysicsStates oldPhysState;
	innerStates currInnerState;
	CameraStates currCamState;
	int interactingWithBuildingId;
	int interactingWithBuildingFace; //-x = 0, +x = 1 etc. will be used for ramps
	int interactingWithRampId;

	float xRotatedOffset, yRotatedOffset;
};

struct trapObject {
    int				id;
	int				ownerId;
	int				timeTillActive;
	int				type;
	int				eventCode;
	glm::vec3		pos;
	float			rotationAngle;
	AABB			aabb;
};

struct staticObject {
	AABB			aabb;
};

struct staticRampObject {
	AABB			aabb;
	float			slope;
};

struct staticResourceObject {
	AABB			aabb;
	int				id;
};

#endif
