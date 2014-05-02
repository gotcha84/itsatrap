
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance *client; // 'client' is a global var in main.cpp

void handleNewPlayer(struct playerObject p)
{
	printf("[CLIENT]: New player (ID:%d) has joined!\n", p.id);
	sg::Player *player = new sg::Player(glm::vec3(p.x, p.y, p.z));
	player->setPlayerID(p.id);
	player->setColor(glm::vec3(1,1,1));
	player->lookIn(glm::vec3(0.0f, 0.0f, 1.0f));
	client->addPlayer(player);

	client->players[p.id] = player;
	client->objects[p.id] = player;

	client->players[p.id]->moveTo(glm::vec3(p.x, p.y, p.z));
}

// Returns 0 if successful
void handlePlayerUpdate(struct playerObject p)
{
	if (client->objects[p.id] == nullptr)
		handleNewPlayer(p);
	
	if (glm::vec3(p.x, p.y, p.z) != client->players[p.id]->getPosition())
		client->players[p.id]->moveTo(glm::vec3(p.x, p.y, p.z));

	if (client->players[p.id]->m_player->m_health != p.health)
	{
		printf("HIT! Player %d's health is now %d\n", p.id, p.health);
		client->players[p.id]->m_player->m_health = p.health;
	}
}

void handleAddTrap(struct trapObject t)
{
	sg::Trap *newTrap = new sg::Trap(t.ownerId, glm::vec3(t.x,t.y,t.z));
	client->root->addChild(newTrap);

	client->objects[t.id] = newTrap;
}

void handleRemoveTrap(struct trapObject t)
{
	if (client->objects[t.id] != nullptr)
		((sg::Trap *) client->objects[t.id])->setColor(glm::vec3(0,0,0));
}

// This will get called everytime server sends an update
void handleUpdateWorldFromServer(DynamicWorld *world)
{
	vector<struct playerObject> players = world->getAllPlayers();
	for (int i = 0; i < players.size(); i++)
		handlePlayerUpdate(players[i]);

	for (map<int, struct trapObject>::iterator it = world->trapMap.begin(); it != world->trapMap.end(); it++)
	{
		if (it->second.eventCode == EVENT_ADD_TRAP)
			handleAddTrap(it->second);
		else if (it->second.eventCode == EVENT_REMOVE_TRAP)
			handleRemoveTrap(it->second);
	}
}