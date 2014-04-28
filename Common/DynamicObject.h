#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#define TYPE_PLAYER 0
#define TYPE_TRAP 1

struct objectProperty {
	float			x, y, z;
	float			minX, minY, minZ, maxX, maxY, maxZ;
	float			lookX, lookY, lookZ;
	float			upX, upY, upZ;
};

struct dynamicObject {
    int				objectId;
	int				type;
    struct objectProperty prop;
};

#endif
