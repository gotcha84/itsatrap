
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance *client; // 'client' is a global var in main.cpp

void handleNewPlayer(struct playerObject p)
{
	printf("[CLIENT]: New player (ID:%d) has joined!\n", p.id);
	sg::Player *player = new sg::Player();
	
	player->setPlayerID(p.id);
	player->moveTo(p.position);
	player->lookAt(p.cameraObject.cameraLookAt);
	player->setUp(p.cameraObject.cameraUp);
	player->getCamera()->setXRotated(p.cameraObject.xRotated);
	player->getCamera()->setYRotated(p.cameraObject.yRotated);

	client->addPlayer(player);
	client->players[p.id] = player;
	client->objects[p.id] = player;
}

void handlePlayerUpdate(struct playerObject p)
{
	if (client->objects[p.id] == nullptr) {
		handleNewPlayer(p);
	}
	else {
		
		// HEALTH
		if (client->players[p.id]->m_player->m_health != p.health) {
			cout << "[CLIENT]: HIT! Player " << p.id << "'s health is now " << p.health << endl;
			client->players[p.id]->setHealth(p.health);
		}
		client->players[p.id]->m_player->m_deathState = p.deathState;

		// BUFFS
		client->players[p.id]->m_player->m_stunDuration = p.stunDuration;
		client->players[p.id]->m_player->m_slowDuration = p.slowDuration;
		client->players[p.id]->m_player->m_timeUntilRespawn = p.timeUntilRespawn;

		// RESOURCES
		client->players[p.id]->m_player->m_resources = p.resources;

		// POSITION & GRAPHIC
		client->players[p.id]->moveTo(p.position);
		client->players[p.id]->getPlayer()->setAABB(&p.aabb);
		//cout << "pos: " << glm::to_string(p.position) << endl;

		if (p.position != client->players[p.id]->getPosition()) {
			//cout << "updating position in enrico.cpp\n";
			client->players[p.id]->moveTo(p.position);
			//Client::sendPlayerUpdate(client->players[p.id]->getPlayerObjectForNetworking());
		}
		if (client->root->getPlayerID() != p.id) {
			glm::vec3 pCenter = p.cameraObject.cameraCenter;
			glm::vec3 pLookAt = p.cameraObject.cameraLookAt;

			if (p.cameraObject.cameraUp != glm::vec3(0.0f, 1.0f, 0.0f) || client->players[p.id]->getCamera()->getCameraUp() != glm::vec3(0.0f, 1.0f, 0.0f)) {
				//cout << "currstate: " << p.currPhysState << endl;
				if (p.currPhysState != PhysicsStates::WallRunning /*|| p.oldPhysState != PhysicsStates::WallRunning*/) {
					client->players[p.id]->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else {
					client->players[p.id]->setUp(p.cameraObject.cameraUp);
				}
				//cout << "curr up: " << glm::to_string(p.cameraObject.cameraUp) << endl;
				client->players[p.id]->setUp(p.cameraObject.cameraUp);
				//cout << "updated up: " << glm::to_string(client->players[p.id]->getCamera()->getCameraUp()) << endl;
			}

			glm::vec3 pLookIn = pLookAt - pCenter;

			glm::vec3 clientLookIn = client->players[p.id]->getCamera()->getCameraLookAt() - client->players[p.id]->getCamera()->getCameraCenter();
			
			if (pLookIn != clientLookIn) {
				client->players[p.id]->lookIn(pLookIn);
				//cout << "[" << client->root->getPlayerID() << "] p" << p.id << " now looking in " << glm::to_string(pLookIn) << endl;
			}

			if (p.cameraObject.xRotated != client->players[p.id]->getCamera()->getXRotated()) {
				//cout << "updated xrotated: " << client->players[p.id]->getCamera()->getXRotated() << endl;
				client->players[p.id]->getCamera()->setXRotated(p.cameraObject.xRotated);
			}
			
			if (p.cameraObject.yRotated != client->players[p.id]->getCamera()->getYRotated()) {
				client->players[p.id]->getCamera()->setYRotated(p.cameraObject.yRotated);
			}

		}
		// if curr player
		else {
			if (p.cameraObject.cameraUp != glm::vec3(0.0f, 1.0f, 0.0f) || client->players[p.id]->getCamera()->getCameraUp() != glm::vec3(0.0f, 1.0f, 0.0f)) {
				if (p.currPhysState != PhysicsStates::WallRunning /*|| p.oldPhysState != PhysicsStates::WallRunning*/) {
					client->players[p.id]->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else {
					client->players[p.id]->setUp(p.cameraObject.cameraUp);
				}
				client->players[p.id]->setUp(p.cameraObject.cameraUp);
			}

			if (p.currCamState == CameraStates::Server) {
				glm::vec3 pCenter = p.cameraObject.cameraCenter;
				glm::vec3 pLookAt = p.cameraObject.cameraLookAt;

				glm::vec3 pLookIn = pLookAt - pCenter;

				glm::vec3 clientLookIn = client->players[p.id]->getCamera()->getCameraLookAt() - client->players[p.id]->getCamera()->getCameraCenter();

				if (pLookIn != clientLookIn) {
					client->players[p.id]->lookIn(pLookIn);
					//cout << "[" << client->root->getPlayerID() << "] p" << p.id << " now looking in " << glm::to_string(pLookIn) << endl;
				}

				if (p.cameraObject.xRotated != client->players[p.id]->getCamera()->getXRotated()) {
					client->players[p.id]->getCamera()->setXRotated(p.cameraObject.xRotated);
				}

				if (p.cameraObject.yRotated != client->players[p.id]->getCamera()->getYRotated()) {
					client->players[p.id]->getCamera()->setYRotated(p.cameraObject.yRotated);
				}
			}
		}

	}
}

void handleAddTrap(struct trapObject t)
{
	if (client->objects[t.id] != nullptr) {
		return;
	}

	string filename = TRAMPOLINE_TRAP_OBJ;
	switch (t.type)
	{
	case TYPE_FREEZE_TRAP:
		break;
	case TYPE_TRAMPOLINE_TRAP:
		filename = TRAMPOLINE_TRAP_OBJ;
		break;
	case TYPE_SLOW_TRAP:
		break;
	case TYPE_PUSH_TRAP:
		break;
	case TYPE_LIGHTNING_TRAP:
		filename = DEATH_TRAP_OBJ;
		break;
	default:
		break;
	}

	sg::Trap *newTrap;
	newTrap = new sg::Trap(t.ownerId, t.pos, t.rotationAngle, TRAP_DIR + filename);
	
	switch (t.type) {
		case TYPE_TRAMPOLINE_TRAP:
			//newTrap->loadModel("../Models/Can.obj", "../Models/");
			break;
		default:
			//newTrap->loadModel("../Models/Polynoid.obj", "../Models/");
			break;
	}

	client->root->addChild(newTrap);
	client->objects[t.id] = newTrap;
}

void handleRemoveTrap(struct trapObject t)
{
	printf("[CLIENT]: Removing trap %d\n", t.id);
	if (client->objects[t.id] != nullptr)
	{
		client->root->removeChild(client->objects[t.id]);
		client->objects[t.id] = nullptr;
	}
}

// This will get called everytime server sends an update
void handleUpdateWorldFromServer(DynamicWorld *world)
{
	for (map<int, struct playerObject>::iterator it = world->playerMap.begin(); it != world->playerMap.end(); ++it) {
			handlePlayerUpdate(it->second);
	}

	for (map<int, struct trapObject>::iterator it = world->trapMap.begin(); it != world->trapMap.end(); ++it) {
		if (it->second.eventCode == EVENT_ADD_TRAP)
			handleAddTrap(it->second);
		else if (it->second.eventCode == EVENT_REMOVE_TRAP)
			handleRemoveTrap(it->second);
	}

	//Window::displayCallback();
}
