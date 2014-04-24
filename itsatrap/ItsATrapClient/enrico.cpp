
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance *client; // 'client' is a global var in main.cpp

void testAddObject(int id, float x, float y, float z, int type)
{
	// MEMORY LEAK POSSIBILITY!
	if (type == 0) {
		sg::Player *player = new sg::Player(glm::vec3(x, y, z));
		player->setPlayerID(id);
		player->setColor(glm::vec3(1,1,1));
		//player->moveTo(glm::vec3(x, y, z));
		player->lookIn(glm::vec3(0.0f, 0.0f, 1.0f));
		client->addPlayer(player);

		client->players[id] = player;
		client->objects[id] = player;

		client->printSceneGraph();
	}
	else if (type == 1)
	{
		// it's a trap!! (remember to add to map)
	}
}

// Returns 0 if successful
void testUpdate(int id, float x, float y, float z, int type)
{
	if (client->objects[id] == nullptr) {
		testAddObject(id, x, y, z, type);
		cout << "player added with id " << id << endl;
		cout << glm::to_string(client->root->getPosition()) << endl;
	}

	if (type == 0) {
		if (glm::vec3(x,y,z) == client->players[id]->getPosition()) {
			client->players[id]->moveTo(glm::vec3(x,y,z));
		}
	}
	else if (type == 1) {
		// ITS A TRAP!!
	}
}

// The argument 'world' contains client->objects information, such as id, x, y, z.
// This function works like this:
// If the object's id exists, update its location
// If the object's id doesn't exist, create a new one object with that id
void testUpdateWorld(WorldState *world)
{
	//world->printWorld();

	for (int i = 0; i < world->getSize(); i++)
	{
		int id = world->getEntryAt(i).objectId;
		float x = world->getEntryAt(i).x;
		float y = world->getEntryAt(i).y;
		float z = world->getEntryAt(i).z;

		testUpdate(id, x, y, z, 0);
		//{
			// If the id is playerId, do not render because we don't want to render our own player.
			// This is just temporary. playerId should not be used as objectId!
			//if (id != Client::getPlayerId()) 
				//testAddCube(id, x, y, z);
		//}
	}
}