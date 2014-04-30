
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance *client; // 'client' is a global var in main.cpp

void handleNewPlayer(struct playerObject p)
{

	sg::Player *player = new sg::Player(glm::vec3(p.x, p.y, p.z));
	player->setPlayerID(p.id);
	player->setColor(glm::vec3(1,1,1));
	player->lookIn(glm::vec3(0.0f, 0.0f, 1.0f));
	client->addPlayer(player);

	client->players[p.id] = player;
	client->objects[p.id] = player;
}

// Returns 0 if successful
void handlePlayerUpdate(struct playerObject p)
{
	if (client->objects[p.id] == nullptr)
		handleNewPlayer(p);
	
	if (glm::vec3(p.x, p.y, p.z) != client->players[p.id]->getPosition()) {
		int collision = -1;

		for (unordered_map<int,sg::Player*>::iterator it = client->players.begin(); it != client->players.end(); it++) {
			if (it->first != p.id) {
				if (client->players[p.id]->collidesWith(it->second)) {
					collision = it->first;
					break;
				}
			}
		}
			
		client->players[p.id]->moveTo(glm::vec3(p.x, p.y, p.z));
		if (collision != -1) {
			cout << "p" << p.id << " collided with p" << collision << endl;
		}
	}

}

// This will get called everytime server sends an update
void handleUpdateWorldFromServer(DynamicWorld *world)
{

	for (int i = 0; i < world->getNumPlayers(); i++)
		handlePlayerUpdate(world->getObjectAt(i));

}