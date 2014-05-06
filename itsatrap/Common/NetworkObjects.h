#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

#define EVENT_ADD_TRAP 1
#define EVENT_REMOVE_TRAP 2

struct aabb {
	float			minX, minY, minZ, maxX, maxY, maxZ;
};

struct playerObject {
    int				id, health;
	float			x, y, z;
	struct aabb		aabb;
	float			lookX, lookY, lookZ;
	float			upX, upY, upZ;
	float			xRotated, yRotated;
};

struct trapObject {
    int				id;
	int				ownerId;
	int				eventCode;
	float			x, y, z;
	struct aabb		aabb;
};

struct staticObject {
	struct			aabb aabb;
};


#endif
