#ifndef NETWORKOBJECTS_H
#define NETWORKOBJECTS_H

struct playerObject {
    int				id;
	float			x, y, z;
	float			minX, minY, minZ, maxX, maxY, maxZ;
	float			lookX, lookY, lookZ;
	float			upX, upY, upZ;
};

struct staticObject {
	float			minX, minY, minZ, maxX, maxY, maxZ;
};

#endif
