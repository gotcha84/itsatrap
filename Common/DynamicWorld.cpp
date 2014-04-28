#include "DynamicWorld.h"

/*
 * default constructor DynamicWorld
 *
 */
DynamicWorld::DynamicWorld()
{
	
}

/*
 * constructor DynamicWorld
 *
 * Deserialize 'packet' and construct DynamicWorld object
 */
DynamicWorld::DynamicWorld(struct packet *packet)
{
	if (packet->eventId != WORLD_UPDATE_EVENT)
	{
		printf("[COMMON]: ERROR: Invalid packet!\n");
		return;
	}

	char *buf = (char *)packet;

	int numUpdates = ((int *)buf)[1];

	for (int i = 0; i < numUpdates; i++)
	{
		struct dynamicObject *entry = (struct dynamicObject *)(buf + 2*sizeof(int) + i*sizeof(struct dynamicObject));
		struct dynamicObject tmp;
		memcpy(&tmp, entry, sizeof(struct dynamicObject));
		objects.push_back(tmp);
	}
}


/*
 * DynamicWorld::updateObject()
 *
 * Try to find an entry corresponding to 'e'. If found, update it, otherwise
 * create new entry.
 */
void DynamicWorld::updateObject(struct dynamicObject e)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].objectId == e.objectId)
		{
			memcpy(&objects[i], &e, sizeof(struct dynamicObject));
			return;
		}
	}

	// Entry not found, create a new one.
	objects.push_back(e);	
}

/*
 * DynamicWorld::getsize()
 *
 */
int DynamicWorld::getSize()
{
	return objects.size();
}

/*
 * DynamicWorld::serialize()
 *
 * Serializes this DynamicWorld. Pointer to the serialization result is stored in 'ptr'.
 * Returns the resulting size in bytes.
 *
 * NOTE: Whoever called this method is responsible to FREE 'ptr'.
 *
 * Serialization policy:
 * byte 0: always filled with 4 (eventId)
 *      4: size of payload
 *      8: objects (not being serialized)
 */
int DynamicWorld::serialize(char **ptr)
{
	int size = 2*sizeof(int) + sizeof(struct dynamicObject) * getSize();
	char *buf = (char *) malloc(size);

	((int *)buf)[0] = 4;
	((int *)buf)[1] = getSize();

	for (int i = 0; i < getSize(); i++)
	{
		memcpy(buf + 2*sizeof(int) + i * sizeof(struct dynamicObject), &objects[i], sizeof(struct dynamicObject));
	}

	*ptr = buf;

	return size;
}


/*
 * DynamicWorld::printWorld()
 *
 */
void DynamicWorld::printWorld()
{
	printf("[COMMON]: Printing world state:\n");
	for (int i = 0; i < getSize(); i++)
	{
		printf("[COMMON]: Object %3d:   x:%4.1f   y:%4.1f   z:%4.1f\n", objects[i].objectId,
			objects[i].prop.x, objects[i].prop.y, objects[i].prop.z);
	}
}

struct dynamicObject DynamicWorld::getObjectAt(int i)
{
	return objects[i];
}