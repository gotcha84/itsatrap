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
		struct playerObject *entry = (struct playerObject *)(buf + 2*sizeof(int) + i*sizeof(struct playerObject));
		struct playerObject tmp;
		memcpy(&tmp, entry, sizeof(struct playerObject));
		playerObjects.push_back(tmp);
	}
}


/*
 * DynamicWorld::updateObject()
 *
 * Try to find an entry corresponding to 'e'. If found, update it, otherwise
 * create new entry.
 */
void DynamicWorld::updatePlayer(struct playerObject e)
{
	for (int i = 0; i < playerObjects.size(); i++)
	{
		if (playerObjects[i].id == e.id)
		{
			memcpy(&playerObjects[i], &e, sizeof(struct playerObject));
			return;
		}
	}

	// Entry not found, create a new one.
	playerObjects.push_back(e);	
}

/*
 * DynamicWorld::getsize()
 *
 */
int DynamicWorld::getNumPlayers()
{
	return playerObjects.size();
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
 *      8: playerObjects (not being serialized)
 */
int DynamicWorld::serialize(char **ptr)
{
	int size = 2*sizeof(int) + sizeof(struct playerObject) * getNumPlayers();
	char *buf = (char *) malloc(size);

	((int *)buf)[0] = 4;
	((int *)buf)[1] = getNumPlayers();

	for (int i = 0; i < getNumPlayers(); i++)
	{
		memcpy(buf + 2*sizeof(int) + i * sizeof(struct playerObject), &playerObjects[i], sizeof(struct playerObject));
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
	for (int i = 0; i < getNumPlayers(); i++)
	{
		printf("[COMMON]: playerObject %3d:   x:%4.1f   y:%4.1f   z:%4.1f\n", playerObjects[i].id,
			playerObjects[i].x, playerObjects[i].y, playerObjects[i].z);
	}
}

struct playerObject DynamicWorld::getObjectAt(int i)
{
	return playerObjects[i];
}