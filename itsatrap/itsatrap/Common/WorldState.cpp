#include "WorldState.h"

/*
 * default constructor WorldState
 *
 */
WorldState::WorldState()
{
	
}

/*
 * constructor WorldState
 *
 * Deserialize 'packet' and construct WorldState object
 */
WorldState::WorldState(struct packet *packet)
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
		struct stateEntry *entry = (struct stateEntry *)(buf + 2*sizeof(int) + i*sizeof(struct stateEntry));
		struct stateEntry tmp;
		memcpy(&tmp, entry, sizeof(struct stateEntry));
		entries.push_back(tmp);
	}
}


/*
 * WorldState::updateEntry()
 *
 * Try to find an entry corresponding to 'e'. If found, update it, otherwise
 * create new entry.
 */
void WorldState::updateEntry(struct stateEntry e)
{
	for (int i = 0; i < entries.size(); i++)
	{
		if (entries[i].objectId == e.objectId)
		{
			entries[i].x = e.x;
			entries[i].y = e.y;
			entries[i].z = e.z;
			return;
		}
	}

	// Entry not found, create a new one.
	entries.push_back(e);	
}

/*
 * WorldState::getsize()
 *
 */
int WorldState::getSize()
{
	return entries.size();
}

/*
 * WorldState::serialize()
 *
 * Serializes this WorldState. Pointer to the serialization result is stored in 'ptr'.
 * Returns the resulting size in bytes.
 *
 * NOTE: Whoever called this method is responsible to FREE 'ptr'.
 *
 * Serialization policy:
 * byte 0: always filled with 4 (eventId)
 *      4: size of payload
 *      8: entries (not being serialized)
 */
int WorldState::serialize(char **ptr)
{
	int size = 2*sizeof(int) + sizeof(struct stateEntry) * getSize();
	char *buf = (char *) malloc(size);

	((int *)buf)[0] = 4;
	((int *)buf)[1] = getSize();

	for (int i = 0; i < getSize(); i++)
	{
		memcpy(buf + 2*sizeof(int) + i * sizeof(struct stateEntry), &entries[i], sizeof(struct stateEntry));
	}

	*ptr = buf;

	return size;
}


/*
 * WorldState::printWorld()
 *
 */
void WorldState::printWorld()
{
	printf("[COMMON]: Printing world state:\n");
	for (int i = 0; i < getSize(); i++)
	{
		printf("[COMMON]: Object %3d:   x:%4.1f   y:%4.1f   z:%4.1f\n", entries[i].objectId,
			entries[i].x, entries[i].y, entries[i].z);
	}
}

struct stateEntry WorldState::getEntryAt(int i)
{
	return entries[i];
}