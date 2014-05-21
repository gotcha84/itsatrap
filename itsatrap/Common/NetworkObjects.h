#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

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
	bool			deathState;
	float			x, y, z;
	struct aabb		aabb;
	float			lookX, lookY, lookZ;
	float			upX, upY, upZ;
	float			xRotated, yRotated;
	float			xVel, yVel, zVel;
	int				stunDuration, slowDuration;
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
