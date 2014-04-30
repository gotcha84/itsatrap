#include "DynamicWorld.h"

#define HEADER_SIZE (2 * sizeof(int))

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

	int numPlayers = ((int *)buf)[1];

	for (int i = 0; i < numPlayers; i++)
	{
		void *ptr = (struct playerObject *)(buf + HEADER_SIZE + i*sizeof(struct playerObject));
		struct playerObject tmp;
		memcpy(&tmp, ptr, sizeof(struct playerObject));
		playerMap[tmp.id] = tmp;
	}
}


void DynamicWorld::updatePlayer(struct playerObject e)
{
	vector<struct playerObject> players = getAllPlayers();
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].id != e.id && checkCollision(e.aabb, players[i].aabb))
		{
			printf("Collision: player %d with player %d\n", e.id, players[i].id);
			return;
		}
	}
	for (int i = 0; i < staticObjects.size(); i++)
	{
		printf("Check %d %.1f %.1f\n", i, staticObjects[i].aabb.minX, staticObjects[i].aabb.maxX);
		if (checkCollision(e.aabb, staticObjects[i].aabb))
		{
			printf("Collision: player %d with static object\n", e.id);
			return;
		}
	}

	playerMap[e.id] = e;
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
 *      4: number of players
 *      8: playerObjects (not being serialized)
 */
int DynamicWorld::serialize(char **ptr)
{
	int payloadSize = sizeof(struct playerObject) * playerMap.size();
	int totalSize = HEADER_SIZE + payloadSize;

	char *buf = (char *) malloc(totalSize);

	((int *)buf)[0] = 4;
	((int *)buf)[1] = playerMap.size();

	vector<struct playerObject> players = getAllPlayers();
	for (int i = 0; i < players.size(); i++)
	{
		memcpy(buf + 2*sizeof(int) + i * sizeof(struct playerObject), &players[i], sizeof(struct playerObject));
	}

	*ptr = buf;

	return totalSize;
}


/*
 * DynamicWorld::printWorld()
 *
 */
void DynamicWorld::printWorld()
{
	printf("[COMMON]: Printing world state:\n");

	vector<struct playerObject> vec = getAllPlayers();
	for (int i = 0; i < vec.size(); i++)
		printf("[COMMON]: playerObject %3d:   x:%4.1f   y:%4.1f   z:%4.1f\n", vec[i].id,
			vec[i].x, vec[i].y, vec[i].z);
}

vector<struct playerObject> DynamicWorld::getAllPlayers()
{
	vector<struct playerObject> vec;
	for(map<int,struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it) {
	  vec.push_back(it->second);
	}

	return vec;
}

int DynamicWorld::getNumPlayers()
{
	return playerMap.size();
}

bool DynamicWorld::checkCollision(struct aabb a, struct aabb b)
{
	return (a.maxX >= b.minX && a.minX <= b.maxX
		&& a.maxY >= b.minY && a.minY <= b.maxY
		&& a.maxZ >= b.minZ && a.minZ <= b.maxZ);
}

void DynamicWorld::addStaticObject(struct staticObject obj)
{
	staticObjects.push_back(obj);
}

int DynamicWorld::getNumStaticObjects() 
{
	return staticObjects.size();
}