#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

struct aabb {
	float			minX, minY, minZ, maxX, maxY, maxZ;
};

struct playerObject {
    int				id;
	float			x, y, z;
	struct			aabb aabb;
	float			lookX, lookY, lookZ;
	float			upX, upY, upZ;
};

struct trapObject {
    int				id;
	float			x, y, z;
	struct			aabb aabb;
};

struct staticObject {
	struct			aabb aabb;
};

#endif
