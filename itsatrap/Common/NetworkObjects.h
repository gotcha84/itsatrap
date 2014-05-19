#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

#include <../glm/glm/glm.hpp>
#include <../glm/glm/ext.hpp>

#define EVENT_ADD_TRAP 1
#define EVENT_REMOVE_TRAP 2

#define TYPE_FREEZE_TRAP 1
#define TYPE_TRAMPOLINE_TRAP 2
#define TYPE_SLOW_TRAP 3
#define TYPE_PUSH_TRAP 4
#define TYPE_LIGHTNING_TRAP 5

struct aabb {
	float			minX, minY, minZ, maxX, maxY, maxZ;
};

struct playerObject {
    int				id, health, numKills, numDeaths;
	glm::vec3		position, lookAt, center, up, velDiff, vel, camZ, toAdd;
	struct aabb		aabb;
	float			xRotated, yRotated;
	int				stunDuration, slowDuration;
	int				timeUntilRespawn;
	int				resources;
	int				onTopOfBuildingId;
	bool			deathState;
};

struct trapObject {
    int				id;
	int				ownerId;
	int				type;
	int				eventCode;
	float			x, y, z;
	float			rotationAngle;
	struct aabb		aabb;
};

struct staticObject {
	struct			aabb aabb;
};


#endif
