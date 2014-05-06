
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance *client; // 'client' is a global var in main.cpp

void handleNewPlayer(struct playerObject p)
{
	printf("[CLIENT]: New player (ID:%d) has joined!\n", p.id);
	sg::Player *player = new sg::Player();
	
	player->setPlayerID(p.id);
	player->moveTo(glm::vec3(p.x, p.y, p.z));
	player->lookAt(glm::vec3(p.lookX, p.lookY, p.lookZ));
	player->setUp(glm::vec3(p.upX, p.upY, p.upZ));
	player->getCamera()->setXRotated(p.xRotated);
	player->getCamera()->setYRotated(p.yRotated);

	client->addPlayer(player);
	client->players[p.id] = player;
	client->objects[p.id] = player;
}

// Returns 0 if successful
void handlePlayerUpdate(struct playerObject p)
{
	if (client->objects[p.id] == nullptr) {
		handleNewPlayer(p);
	}
	else {	
		if (glm::vec3(p.x, p.y, p.z) != client->players[p.id]->getPosition()) {
			client->players[p.id]->moveTo(glm::vec3(p.x, p.y, p.z));
		}

		if (client->root->getPlayerID() != p.id) {
			glm::vec3 pCenter = glm::vec3(p.x, p.y, p.z);
			glm::vec3 pLookAt = glm::vec3(p.lookX, p.lookY, p.lookZ);
			glm::vec3 pLookIn = pLookAt - pCenter;

			glm::vec3 clientLookIn = client->players[p.id]->getCamera()->getCameraLookAt() - client->players[p.id]->getCamera()->getCameraCenter();
			
			if (pLookIn != clientLookIn) {
				client->players[p.id]->lookIn(pLookIn);
				//cout << "[" << client->root->getPlayerID() << "] p" << p.id << " now looking in " << glm::to_string(pLookIn) << endl;
			}

			if (glm::vec3(p.upX, p.upY, p.upZ) != client->players[p.id]->getCamera()->getCameraUp()) {
				client->players[p.id]->setUp(glm::vec3(p.upX, p.upY, p.upZ));
			}

			if (p.xRotated != client->players[p.id]->getCamera()->getXRotated()) {
				client->players[p.id]->getCamera()->setXRotated(p.xRotated);
			}
			
			if (p.yRotated != client->players[p.id]->getCamera()->getYRotated()) {
				client->players[p.id]->getCamera()->setYRotated(p.yRotated);
			}
		}

		if (client->players[p.id]->m_player->m_health != p.health) {
			printf("[CLIENT]: HIT! Player %d's health is now %d\n", p.id, p.health);
			client->players[p.id]->m_player->m_health = p.health;
		}
	}
}

void handleAddTrap(struct trapObject t)
{
	sg::Trap *newTrap = new sg::Trap(t.ownerId, glm::vec3(t.x,t.y,t.z));
	client->root->addChild(newTrap);

	client->objects[t.id] = newTrap;

	delete newTrap;
	newTrap = nullptr;
}

void handleRemoveTrap(struct trapObject t)
{
	if (client->objects[t.id] != nullptr) {
		dynamic_cast<sg::Trap*>(client->objects[t.id])->setColor(glm::vec3(0,0,0));
	}
}

// This will get called everytime server sends an update
void handleUpdateWorldFromServer(DynamicWorld *world)
{
	vector<struct playerObject> players = world->getAllPlayers();
	for (int i = 0; i < players.size(); i++) {
		handlePlayerUpdate(players[i]);
	}

	for (map<int, struct trapObject>::iterator it = world->trapMap.begin(); it != world->trapMap.end(); it++) {
		if (it->second.eventCode == EVENT_ADD_TRAP)
			handleAddTrap(it->second);
		else if (it->second.eventCode == EVENT_REMOVE_TRAP)
			handleRemoveTrap(it->second);
	}
}
