
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance *client; // 'client' is a global var in main.cpp

void handleNewObjectFromServer(int id, float x, float y, float z, int type)
{
	// MEMORY LEAK POSSIBILITY!
	if (type == TYPE_PLAYER) {
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
	else if (type == TYPE_TRAP)
	{
		// it's a trap!! (remember to add to map)
	}
}

// Returns 0 if successful
void handleUpdateObjectFromServer(int id, float x, float y, float z, int type)
{
	if (client->objects[id] == nullptr) {
		// todo check type to add player vs trap
		handleNewObjectFromServer(id, x, y, z, type);
		cout << "player added with id " << id << endl;
		cout << glm::to_string(client->root->getPosition()) << endl;
	}

	if (type == TYPE_PLAYER) {
		if (glm::vec3(x,y,z) != client->players[id]->getPosition()) {
			int collision = -1;

			for (unordered_map<int,sg::Player*>::iterator it = client->players.begin(); it != client->players.end(); it++) {
				if (it->first != id) {
					if (client->players[id]->collidesWith(it->second)) {
						collision = it->first;
						break;
					}
				}
			}
			
			if (collision == -1) {
				client->players[id]->moveTo(glm::vec3(x,y,z));
				printf("[CLIENT]: Player %d move to %.1f %.1f %.1f\n", id, x, y, z);
			}
			else {
				// TODO maybe colliding with wrong player?
				cout << "COLLISION with player " << collision << endl;
			}
		}
	}
	else if (type == TYPE_TRAP) {
		// ITS A TRAP!!
	}
}

// The argument 'world' contains client->objects information, such as id, x, y, z.
// This function works like this:
// If the object's id exists, update its location
// If the object's id doesn't exist, create a new one object with that id
void handleUpdateWorldFromServer(DynamicWorld *world)
{
	//world->printWorld();

	for (int i = 0; i < world->getSize(); i++)
	{
		int id = world->getObjectAt(i).objectId;
		float x = world->getObjectAt(i).prop.x;
		float y = world->getObjectAt(i).prop.y;
		float z = world->getObjectAt(i).prop.z;

		handleUpdateObjectFromServer(id, x, y, z, 0);
		//{
			// If the id is playerId, do not render because we don't want to render our own player.
			// This is just temporary. playerId should not be used as objectId!
			//if (id != Client::getPlayerId()) 
				//testAddCube(id, x, y, z);
		//}
	}
}