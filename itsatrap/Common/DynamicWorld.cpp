#include "DynamicWorld.h"
#define XOFFSET 10.0f
#define YOFFSET 13.0f
#define ZOFFSET 8.0f
#define SMALLESTWIDTH 1.0f

#define HEADER_SIZE (3 * sizeof(int))

/*
 * default constructor DynamicWorld
 *
 */
DynamicWorld::DynamicWorld()
{
	currentId = 100;
	memset(playerLock, 0, sizeof(playerLock));
	for (int i = 0; i < MAX_PLAYERS; i++) {
		cleanStateInfo(i);
	}

	team1RespawnPoints.push_back(glm::vec3(200, 200, -300));
	team2RespawnPoints.push_back(glm::vec3(200, 200, -300));

	portalMap.clear();
	playerMap.clear();
	trapMap.clear();

}

/*
 * constructor DynamicWorld
 *
 * Deserialize 'packet' and construct DynamicWorld object
 */
DynamicWorld::DynamicWorld(struct packet *packet)
{
	memset(playerLock, 0, sizeof(playerLock));

	if (packet->eventId != WORLD_UPDATE_EVENT)
	{
		printf("[COMMON]: ERROR: Invalid packet!\n");
		return;
	}

	char *buf = (char *)packet;
	char *movingPtr = buf + HEADER_SIZE;

	int numPlayers = ((int *)buf)[1];
	int numTrapUpdates = ((int *)buf)[2];

	for (int i = 0; i < numPlayers; i++)
	{
		void *ptr = (struct playerObject *)movingPtr;
		movingPtr += sizeof(struct playerObject);
		struct playerObject tmp = {};
		memcpy(&tmp, ptr, sizeof(struct playerObject));
		playerMap[tmp.id] = tmp;
	}
	for (int i = 0; i < numTrapUpdates; i++)
	{
		void *ptr = (struct trapObject *)movingPtr;
		movingPtr += sizeof(struct trapObject);
		struct trapObject tmp;
		memcpy(&tmp, ptr, sizeof(struct trapObject));
		trapMap[tmp.id] = tmp;
	}
	for (int i = 0; i < MAX_PLAYERS; i++) {
		cleanStateInfo(i);
	}
}

void DynamicWorld::cleanStateInfo(int id) {
	// TODO: implement if needed
}

void DynamicWorld::addNewPlayer(struct playerObject p)
{
	// Uncomment this to spawn at team base
	// respawnPlayer(&p);
	// and delete this while loop to spawn at team base
	while (checkCollisionsWithAllNonTraps(&p) != -1)
	{
		p.position.x += 10;
		computeAABB(&p);
		
	}
	cout << "pos: " << glm::to_string(p.position) << endl;
	// why not update resources?
	p.health = 100;
	p.numKills = 0;
	p.numDeaths = 0;
	p.stunDuration = 0;
	p.slowDuration = 0;
	p.feetPlanted = true;
	p.interactingWithBuildingId = -1;
	p.interactingWithBuildingFace = -1;
	p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	p.feetPlanted = true;
	p.triedForward = false;
	p.currPhysState = PhysicsStates::None;
	p.currInnerState = innerStates::Off;
	p.currCamState = CameraStates::Client;
	p.stopwatch = Stopwatch();
	p.canClimb = true;
	ConfigSettings::getConfig()->getValue("StartingResources", p.resources);
	p.knifeDelay = 0;
	p.timeUntilRegen = 0;
	p.flashDuration = 0;
	p.hitCrosshair = 0;
	//p.position.x = 0.0f;
	//p.position.y = 500.0f;

	playerMap[p.id] = p;
	//cout << "newplayer aabb: ";
	//p.aabb.print();
}


void DynamicWorld::updatePlayer(struct playerObject p)
{
	
	if (playerLock[p.id] == true)
		return;

	if (playerMap.find(p.id) == playerMap.end())
	{
		addNewPlayer(p);
		return;
	}

	// You're dead, you shall not update yourself.
	if (playerMap[p.id].deathState)
		return;

	// OVERRIDE STUFF FROM CLIENT. why not override everything?
	p.health = playerMap[p.id].health;
	p.stunDuration = playerMap[p.id].stunDuration;
	p.slowDuration = playerMap[p.id].slowDuration;
	p.numKills = playerMap[p.id].numKills;
	p.numDeaths = playerMap[p.id].numDeaths;
	p.resources = playerMap[p.id].resources;
	p.velocityDiff = playerMap[p.id].velocityDiff;
	p.velocity = playerMap[p.id].velocity;
	p.feetPlanted = playerMap[p.id].feetPlanted;
	p.triedForward = playerMap[p.id].triedForward;
	p.interactingWithBuildingId = playerMap[p.id].interactingWithBuildingId;
	p.interactingWithBuildingFace = playerMap[p.id].interactingWithBuildingFace;
	p.currInnerState = playerMap[p.id].currInnerState;
	p.currPhysState = playerMap[p.id].currPhysState;
	p.currCamState = playerMap[p.id].currCamState;
	p.aabb = playerMap[p.id].aabb;
	p.position = playerMap[p.id].position;
	p.stopwatch = playerMap[p.id].stopwatch;

	p.cameraObject.cameraCenter = playerMap[p.id].cameraObject.cameraCenter;
	p.cameraObject.cameraLookAt = playerMap[p.id].cameraObject.cameraLookAt;
	p.cameraObject.cameraUp = playerMap[p.id].cameraObject.cameraUp;
	p.cameraObject.camX = playerMap[p.id].cameraObject.camX;
	p.cameraObject.camZ = playerMap[p.id].cameraObject.camZ;
	p.cameraObject.xRotated = playerMap[p.id].cameraObject.xRotated;
	p.cameraObject.yRotated = playerMap[p.id].cameraObject.yRotated;
	
	// ??
	if (checkCollisionsWithAllNonTraps(&p) == -1)
		return;

	playerMap[p.id] = p;
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
 *		8: number of traps
 *      12: playerObjects (not being serialized)
 *      ...: trapObjects (not being serialized)
 */
int DynamicWorld::serialize(char *ptr)
{
	vector<struct trapObject> trapsToSend;
	vector<int> trapsToRemove;

	// Iterating traps
	for(map<int,struct trapObject>::iterator it = trapMap.begin(); it != trapMap.end(); ++it) 
	{
		if (it->second.eventCode != 0)
		{
			//printf("Sending event %d about trap %d\n", it->second.eventCode, it->second.id);
			trapsToSend.push_back(it->second);

			if (it->second.eventCode == EVENT_REMOVE_TRAP)
				trapsToRemove.push_back(it->first);

			it->second.eventCode = 0;
		}
	}

	for (int i = 0; i < trapsToRemove.size(); i++)
		trapMap.erase(trapsToRemove[i]);

	int payloadSize = sizeof(struct playerObject)*playerMap.size() + sizeof(trapObject)*trapsToSend.size();
	int totalSize = HEADER_SIZE + payloadSize;

	char *buf = ptr;
	char *movingPtr = buf + HEADER_SIZE;

	// HEADER
	((int *)buf)[0] = 4;
	((int *)buf)[1] = playerMap.size();
	//cout << "size :" << playerMap.size() << endl;
	((int *)buf)[2] = trapsToSend.size();

	// PAYLOAD
	for(map<int,struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		memcpy(movingPtr, &it->second, sizeof(struct playerObject));
		//it->second.toAdd = glm::vec3(0, 0, 0);
		movingPtr += sizeof(struct playerObject);
	}
	for (int i = 0; i < trapsToSend.size(); i++)
	{
		memcpy(movingPtr, &trapsToSend[i], sizeof(struct trapObject));
		movingPtr += sizeof(struct trapObject);
	}
	

	if (trapsToSend.size() > 0)
		printf("trapstosend: %d\n", ((int *)buf)[2]);

	if (totalSize > BUFSIZE)
		printf("[COMMON]: ERROR!! SIZE exceeds BUFSIZE. SIZE: %d\n", totalSize); 

	memset(playerLock, 0, sizeof(playerLock)); // unlock all players

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
			vec[i].position.x, vec[i].position.y, vec[i].position.z);
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
/*
bool DynamicWorld::checkCollision(struct aabb a, struct aabb b)
{
	
	return (a.maxX >= b.minX && a.minX <= b.maxX
		&& a.maxY >= b.minY && a.minY <= b.maxY
		&& a.maxZ >= b.minZ && a.minZ <= b.maxZ);

	return false;
}
*/


void DynamicWorld::addStaticObject(struct staticObject obj)
{
	staticObjects.push_back(obj);
}

void DynamicWorld::addStaticWallObject(struct staticObject obj)
{
	staticWallObjects.push_back(obj);
}

void DynamicWorld::addStaticRampObject(struct staticRampObject obj)
{
	staticRampObjects.push_back(obj);
}

void DynamicWorld::addStaticResourceObject(struct staticResourceObject obj)
{
	staticResourceObjects.push_back(obj);
}

int DynamicWorld::getNumStaticObjects() 
{
	return staticObjects.size();
}

int DynamicWorld::getNumStaticWallObjects()
{
	return staticWallObjects.size();
}

int DynamicWorld::getNumStaticRampObjects()
{
	return staticRampObjects.size();
}

int DynamicWorld::getNumStaticResourceObjects()
{
	return staticResourceObjects.size();
}

AABB DynamicWorld::getStaticObjectBB(int buildingId) {
	return staticObjects[buildingId].aabb;
}

AABB DynamicWorld::getStaticRampObjectBB(int rampId) {
	return staticRampObjects[rampId].aabb;
}

AABB DynamicWorld::getStaticResourceBB(int resourceId)
{
	for (int i = 0; i < staticResourceObjects.size(); ++i) {
		if (staticResourceObjects[i].id == resourceId) {
			return staticResourceObjects[i].aabb;
		}
	}

	return AABB();
}

void DynamicWorld::addTrap(struct trapObject t)
{
	int cost = 0;
	switch (t.type)
	{
	case TYPE_FREEZE_TRAP:
		ConfigSettings::getConfig()->getValue("CostFreezeTrap", cost);
		break;

	case TYPE_TRAMPOLINE_TRAP:
		ConfigSettings::getConfig()->getValue("CostTrampolineTrap", cost);
		break;

	case TYPE_SLOW_TRAP:
		ConfigSettings::getConfig()->getValue("CostSlowTrap", cost);
		break;

	case TYPE_PUSH_TRAP:
		ConfigSettings::getConfig()->getValue("CostPushTrap", cost);
		break;

	case TYPE_LIGHTNING_TRAP:
		ConfigSettings::getConfig()->getValue("CostLightningTrap", cost);
		break;

	case TYPE_PORTAL_TRAP:
	{
		ConfigSettings::getConfig()->getValue("CostPortalTrap", cost);
		break;
	}
	case TYPE_FLASH_TRAP:
	{
		ConfigSettings::getConfig()->getValue("CostFlashTrap", cost);
		break;
	}
	default:
		cost = 10;
		break;
	}

	//cost = 5;

	struct playerObject *trapOwner = &playerMap[t.ownerId];
	if (trapOwner->resources - cost < 0)
		return; // You do not have enough money!

	trapOwner->resources -= cost;

	t.eventCode = EVENT_ADD_TRAP;
	t.id = currentId;
	int timeTillActive = 0;
	ConfigSettings::getConfig()->getValue("TrapInactivePeriod", timeTillActive);
	t.timeTillActive = timeTillActive;
	trapMap[currentId] = t;
	
	playerLock[t.ownerId] = true;

	// Special case for portal trap
	if (t.type == TYPE_PORTAL_TRAP)
	{

		if (portalMap[t.ownerId] != nullptr)
		{
			portalMap[t.ownerId]->eventCode = EVENT_REMOVE_TRAP;
			portalMap.erase(t.ownerId);
		}
		portalMap[t.ownerId] = &trapMap[t.id];

		// Announce this new portal
		for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
		{
			if (it->second.id != t.ownerId && it->second.id % 2 == t.ownerId % 2)
				addInfoMessage(it->second.id, "Your teammate (player " + to_string(t.ownerId) + ") deployed a portal");
		}
	}

	currentId++;
}

int DynamicWorld::checkCollisionsWithAllNonTraps(struct playerObject *e)
{
	vector<struct playerObject> players = getAllPlayers();
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].id != e->id && e->aabb.collidesWith(players[i].aabb))
		{
			//printf("Collision: player %d with player %d\n", e->id, players[i].id);
			return true;
		}
	}
	for (int i = 0; i < staticObjects.size(); i++)
	{
		if (e->aabb.collidesWith(staticObjects[i].aabb))
		{
			//printf("Collision: player %d with static object %d\n", e->id, i);
			return i;
		}
	}

	for (int i = 0; i < staticWallObjects.size(); i++) 
	{
		if (e->aabb.collidesWith(staticWallObjects[i].aabb))
		{
			//printf("Collision: player %d with static wall object %d\n", e->id, i);
			return i;
		}
	}

	for (int i = 0; i < staticResourceObjects.size(); i++)
	{
		if (e->aabb.collidesWith(staticResourceObjects[i].aabb))
		{
			printf("Collision: player %d with resource object %d\n", e->id, i);
			return i;
		}
	}


	return -1;
}

int DynamicWorld::checkSideCollisionsWithAllBuildings(glm::vec3 from, glm::vec3 goTo, struct playerObject *e)
{
	for (int i = 0; i < staticObjects.size(); i++)
	{
		// Something wrong with building#40
		if (staticObjects[i].aabb.collidesWithSide(from, goTo, e->aabb, i))
		{
			//printf("Collision: player %d with static object %d\n", e->id, i);
			return i;
		}
	}

	return -1;
}

void DynamicWorld::updateTimings(int timeDiff)
{
	for(map<int,struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		if (p.stunDuration > 0)
			p.stunDuration -= timeDiff;
		
		if (p.slowDuration > 0)
			p.slowDuration -= timeDiff;

		if (p.timeUntilRespawn > 0)
		{
			p.timeUntilRespawn -= timeDiff;
			if (p.timeUntilRespawn <= 0)
				respawnPlayer(&p);
		}

		if (p.knifeDelay > 0)
			p.knifeDelay -= timeDiff;

		if (p.timeUntilRegen > 0)
			p.timeUntilRegen -= timeDiff;
		else
		{
			if (p.health < 100)
			{
				ConfigSettings::getConfig()->getValue("HealthRegenInterval", p.timeUntilRegen);
				p.health++;
			}
		}
		
		if (p.flashDuration > 0)
			p.flashDuration -= timeDiff;

		if (p.hitCrosshair > 0)
			p.hitCrosshair -= timeDiff;
	}

	for (map<int, struct trapObject>::iterator it = trapMap.begin(); it != trapMap.end(); ++it)
	{
		struct trapObject *t = &it->second;

		if (t->timeTillActive > 0)
			t->timeTillActive -= timeDiff;
	}
}

void DynamicWorld::playerDamage(struct playerObject *attacker, struct playerObject *target, int damage)
{
	playerLock[target->id] = true;
	playerLock[attacker->id] = true;

	// already dead
	if (target->deathState)
		return;

	target->health -= damage;
	ConfigSettings::getConfig()->getValue("HealthRegenWaitAfterDamage", target->timeUntilRegen);
	ConfigSettings::getConfig()->getValue("HitCrosshairDuration", attacker->hitCrosshair);

	if (target->health <= 0)
	{
		int respawnTime = 0;
		ConfigSettings::getConfig()->getValue("RespawnTime", respawnTime);
		target->timeUntilRespawn = respawnTime;

		target->numDeaths++;
		target->position = glm::vec3(0, 0, 0);
		target->aabb.maxX = 0;
		target->aabb.maxY = 0;
		target->aabb.maxZ = 0;
		target->aabb.minX = 0;
		target->aabb.minY = 0;
		target->aabb.minZ = 0;
		target->deathState = true;

		addInfoMessage(attacker->id, "You have killed player " + to_string(target->id));
		addInfoMessage(target->id, "You have been killed by player " + to_string(attacker->id));

		if (attacker->id % 2 != target->id % 2)
		{
			attacker->numKills++;

			int killBonusResource = 0;
			ConfigSettings::getConfig()->getValue("KillBonusResource", killBonusResource);
			attacker->resources += killBonusResource;
		}
	}
}

void DynamicWorld::respawnPlayer(struct playerObject *p) {

	cout << "RESPAWNING PLAYER " << p->id << endl;
	
	if (p->id % 2 == 0)
		p->position = team1RespawnPoints[rand() % team1RespawnPoints.size()];
	else
		p->position = team2RespawnPoints[rand() % team2RespawnPoints.size()];
	
	computeAABB(p);

	p->health = 100;
	p->timeUntilRespawn = 0;
	p->deathState = false;
	p->currPhysState = PhysicsStates::None;
	p->currInnerState = innerStates::Off;
}

void DynamicWorld::computeAABB(struct playerObject *p)
{
	p->aabb.update(p->position, &aabbOffsets[TYPE_PLAYER]);

	/*
	static int counter = 0;
	if (counter == 100)
	{
		counter = 0;
		printf("PLAYER: ");
		p->aabb.print();
	}
	counter++;
	*/
}

void DynamicWorld::computeAABB(struct trapObject *t)
{
	t->aabb.update(t->pos, &aabbOffsets[t->type]);
}

void DynamicWorld::applyCollisions() {
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		if (p.currPhysState == PhysicsStates::None /*|| p.currPhysState == PhysicsStates::WallRunning*/) {
			glm::vec3 proposedNewPos = p.position + p.velocity + p.velocityDiff;

			glm::vec3 oldPos = p.position;
			glm::vec3 newPos;

			//cout << "pos: " << glm::to_string(p.position) << endl;
			//cout << "newPos: " << glm::to_string(proposedNewPos) << endl;
			p.position = proposedNewPos;	
			p.position.y += YOFFSET;
			computeAABB(&p);
			p.position = oldPos;
			p.position.y += YOFFSET;
			int collisionId = checkCollisionsWithAllNonTraps(&p);
			p.position = oldPos;
			computeAABB(&p);
			//cout << "collision id: " << collisionId << endl;
			if (collisionId != -1) {
				//cout << "collision id: " << collisionId << endl;
				p.position = proposedNewPos;
				//p.position.y += YOFFSET;
				computeAABB(&p);
				//p.position = oldPos;
				//p.position.y += YOFFSET;
				int buildingId = checkSideCollisionsWithAllBuildings(oldPos, p.position, &p);
				p.position = oldPos;
				computeAABB(&p);
				if (buildingId != -1) {
					//cout << "collided with building" << endl;
					//cout << "xz: " << proposedNewPos.x << ", " << proposedNewPos.z << endl;
					//cout << "heightmap at that location: " << World::m_heightMap[(int)proposedNewPos.x + World::m_heightMapXShift][(int)proposedNewPos.z + World::m_heightMapXShift];
					// TODO: check guy is facing wall too, at rest
					if (!p.feetPlanted /*&& !(m_physics->atRest()) */ && p.triedForward) {
						if (Physics::handleNearTop(proposedNewPos, staticObjects[buildingId])) {
							StateLogic::startHoldingEdge(&p, buildingId);
							return;
						}
						float angle = Physics::handleAngleIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
						if (abs(90.0f - angle) < 15.0f /*&& m_physics->m_velocity.y >= m_miniJumpYVelocityThreshold*/) {
							StateLogic::startClimbing(&p, buildingId);
							return;
						}

						else {
							// 0,1 = x, -1 = y, 4,5 = z
							int newDirection = Physics::handleReflectionIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
							if (newDirection != -1) {
								StateLogic::startWallRunning(&p, newDirection, p.velocityDiff, angle, buildingId);
								return;
							}
							else {
								p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
								p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
							}
						}
					}
					else {
						p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
						p.velocity = glm::vec3(0.0f, p.velocity.y, 0.0f);
					}
				}

				else {
					p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
					p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
				}
				/*p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
				p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);*/
			}
		}
	}
}

void DynamicWorld::processMoveEvent(int playerId, Direction dir)
{
	struct playerObject *p = &playerMap[playerId];
	//cout << "processing move event, curr_state is: " << p->currPhysState << endl;
	switch (dir)
	{
		case FORWARD:
			p->triedForward = true;
			break;
		case BACKWARD:
			p->triedBackward = true;
			break;
		case LEFT:
			p->triedLeft = true;
			break;
		case RIGHT:
			p->triedRight = true;
			break;
		default:
			break;
	}

	
}



void DynamicWorld::applyMoveEvents() {
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		switch (p.currPhysState) {
			case None:
				noneMoveEvent(p.id);
				break;
			case Climbing:
				climbingMoveEvent(p.id);
				break;
			case PullingUp:
				pullingUpMoveEvent(p.id);
				break;
			case HoldingEdge:
				holdingEdgeMoveEvent(p.id);
				break;
			case WallRunning:
				wallRunningMoveEvent(p.id);
				break;
			}
	}
}

void DynamicWorld::noneMoveEvent(int playerId)
{
	//cout << "handling none move event" << endl;
	struct playerObject *p = &playerMap[playerId];

	glm::vec3 proposedNewPos;
	
	glm::vec3 tmp_camZ = glm::vec3(p->cameraObject.camZ.x, 0.0f, p->cameraObject.camZ.z);
	//cout << "tmp_camZ : " << glm::to_string(tmp_camZ) << endl;

	float speedMultiplier = 1.0;
	if (p->slowDuration > 0)
	{
		float slowFactor = 0;
		ConfigSettings::getConfig()->getValue("SlowFactor", slowFactor);
		speedMultiplier = slowFactor;
	}

	float xSlowWalkFactor = 0;
	ConfigSettings::getConfig()->getValue("xSlowWalkFactor", xSlowWalkFactor);
	
	float zSlowWalkFactor = 0;
	ConfigSettings::getConfig()->getValue("zSlowWalkFactor", zSlowWalkFactor);
	
	float xWalkFactor = 0;
	ConfigSettings::getConfig()->getValue("xWalkFactor", xWalkFactor);
	
	float zWalkFactor = 0;
	ConfigSettings::getConfig()->getValue("zWalkFactor", zWalkFactor);

	if (!p->feetPlanted) {
		xWalkFactor = xSlowWalkFactor * speedMultiplier;
		zWalkFactor = zSlowWalkFactor * speedMultiplier;
	}
	else {
		xWalkFactor = xWalkFactor * speedMultiplier;
		zWalkFactor = zWalkFactor * speedMultiplier;
	}
	glm::vec3 toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
	if (p->triedForward) {
		toAdd += zWalkFactor*tmp_camZ;
	}
	else if (p->triedBackward) {
		toAdd += -1.0f*zWalkFactor*tmp_camZ;
	}
	if (p->triedLeft) {
		toAdd += -1.0f*xWalkFactor*p->cameraObject.camX;
	}
	else if (p->triedRight) {
		toAdd += xWalkFactor*p->cameraObject.camX;
	}

	p->velocityDiff = toAdd;

}

void DynamicWorld::climbingMoveEvent(int playerId) {
	//cout << "handling climbing move event" << endl;
	struct playerObject *p = &playerMap[playerId];

	// TODO: enable movement?

	if (p->triedBackward) {
		cout << "decided to fall back down from climbing" << endl;
		StateLogic::statesInfo->End.lookY = 0.0f - p->cameraObject.yRotated;
		StateLogic::statesInfo->End.lookYIncrement = StateLogic::statesInfo->End.lookY / (StateLogic::statesInfo->End.fraction* StateLogic::statesInfo->numFrames);
		p->currInnerState = innerStates::Ending;
		//TODO: maybe more, set Holders
	}
	
}

void DynamicWorld::holdingEdgeMoveEvent(int playerId) {
	//cout << "handling holding edge move event" << endl;
	struct playerObject *p = &playerMap[playerId];

	glm::vec3 proposedNewPos;

	glm::vec3 tmp_camZ = glm::vec3(p->cameraObject.camZ.x, 0.0f, p->cameraObject.camZ.z);

	float speedMultiplier = 1.0;
	if (p->slowDuration > 0)
	{
		float slowFactor = 0;
		ConfigSettings::getConfig()->getValue("SlowFactor", slowFactor);
		speedMultiplier = slowFactor;
	}

	float xSlowWalkFactor = 0;
	ConfigSettings::getConfig()->getValue("xSlowWalkFactor", xSlowWalkFactor);

	float zSlowWalkFactor = 0;
	ConfigSettings::getConfig()->getValue("zSlowWalkFactor", zSlowWalkFactor);

	float xWalkFactor = xSlowWalkFactor * speedMultiplier;
	float zWalkFactor = zSlowWalkFactor * speedMultiplier;
	
	glm::vec3 toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
	// TODO: depend on building face rather than where you looking at
	if (p->triedForward) {
		cout << "started pulling up" << endl;
		StateLogic::startPullingUp(p, p->interactingWithBuildingId);
		return;
	}
	else if (p->triedBackward) {
		cout << "decided to fall back down" << endl;
		p->currInnerState = innerStates::Ending;
		//TODO: maybe more, set Holders
		return;
	}
	
	if (!p->triedLeft && !p->triedRight) {
		return;
	}

	if (p->triedLeft) {
		toAdd += -1.0f*xWalkFactor*p->cameraObject.camX;
	}
	else if (p->triedRight) {
		toAdd += xWalkFactor*p->cameraObject.camX;
	}
		
	proposedNewPos = p->position + p->velocity + toAdd;

	glm::vec3 oldPos = p->position;
	glm::vec3 newPos;

	//cout << "pos: " << glm::to_string(p->position) << endl;
	//cout << "newPos: " << glm::to_string(proposedNewPos) << endl;
	p->position = proposedNewPos;
	computeAABB(p);

	// uncomment below if you want to not fall off when holding edge. idk if works
	/*
	bool fellOffSide = staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb);
	p->position = oldPos;
	computeAABB(p);

	// TODO: let you fall off?
	if (fellOffSide) {
		cout << "reached side while holding edge" << endl;
		toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
		p->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
		p->velocity = glm::vec3(0.0f, p->velocity.y, 0.0f);
	}
	p->velocityDiff += toAdd;
	*/
	p->velocityDiff = toAdd;

}

void DynamicWorld::pullingUpMoveEvent(int playerId) {

	//cout << "why handling pullingupevent wtf??" << endl;
	return;
}

void DynamicWorld::wallRunningMoveEvent(int playerId) {

	//cout << "why handling wallrunningmoveevent" << endl;
	return;
}


void DynamicWorld::resetWorldInfo() {
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		// TODO: add more if needed
		p.triedForward = false;
		p.triedBackward = false;
		p.triedLeft = false;
		p.triedRight = false;
		p.currCamState = CameraStates::Client;
		p.oldPhysState = p.currPhysState;
		switch (p.currPhysState) {
			case PhysicsStates::Climbing:
				//p.cameraObject.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				//if (p.currInnerState == innerStates::Starting || p.currInnerState == innerStates::Mid || p.currInnerState == innerStates::Ending) {
					p.currCamState = CameraStates::Server;
				//}
				break;
			case PhysicsStates::HoldingEdge:
				//p.cameraObject.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				//if (p.currInnerState == innerStates::Starting || p.currInnerState == innerStates::Ending) {
					p.currCamState = CameraStates::Server;
				//}
				break;
			case PhysicsStates::PullingUp:
				//p.cameraObject.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				//if (p.currInnerState == innerStates::Starting || p.currInnerState == innerStates::Ending) {
					p.currCamState = CameraStates::Server;
				//}
				break;
			case PhysicsStates::WallRunning:
				p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
				//if (p.currInnerState == innerStates::Starting || p.currInnerState == innerStates::Ending) {
					p.currCamState = CameraStates::Server;
				//}
				break;
			default:
				//p.cameraObject.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
		}
		
	}
}

void DynamicWorld::applyTrapGravity() {
	for (map<int, struct trapObject>::iterator it = trapMap.begin(); it != trapMap.end(); ++it)
	{
		struct trapObject &t = it->second;
		glm::vec3 oldPos = t.pos;

		float gravityConstant = 0;
		ConfigSettings::getConfig()->getValue("gravityConstant", gravityConstant);

		t.pos += glm::vec3(0.0f, gravityConstant, 0.0f);
		computeAABB(&t);
		//cout << ""
		/*cout << "taabb: ";
		t.aabb.print();*/

		for (int i = 0; i < staticObjects.size(); i++) {
			if (staticObjects[i].aabb.cameFromTop(oldPos, t.pos, t.aabb, i)) {

				t.pos = oldPos;

				//cout << "trap aabb: " << endl;
				//t.aabb.print();
				//cout << "building thing " << endl;
				//staticObjects[1].aabb.print();
				//float yLength = t.aabb.maxY - t.aabb.minY;
				//t.pos.y = staticObjects[i].aabb.maxY+(yLength/2);
				//cout << "updated to: " << glm::to_string(t.pos) << endl;
				
				break;
			}
		}

		/*if (t.pos.y < 0)
			t.pos = oldPos;*/

		if (t.pos != oldPos)
		{
			//computeAABB(&t);
			t.eventCode = EVENT_UPDATE_TRAP;
		}
	}
}


// TODO: check for feet?
void DynamicWorld::applyGravity()
{
	//cout << "checking for gravity\n";
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;

		/*if (p.velocity != glm::vec3(0.0f, 0.0f, 0.0f) || p.velocityDiff != glm::vec3(0.0f, 0.0f, 0.0f)) {
			cout << "pvelo: " << glm::to_string(p.velocity) << endl;
			cout << "pvelodiff: " << glm::to_string(p.velocityDiff) << endl;
			}*/

		glm::vec3 oldPos = p.position;

		int xIndex1 = (int)floor(p.position.x + p.velocity.x + p.velocityDiff.x /*- 5.0f*/);
		int zIndex1 = (int)floor(p.position.z + p.velocity.z + p.velocityDiff.z /*- 5.0f*/);

		int xIndex2 = xIndex1 + 1/*0*/;
		int zIndex2 = zIndex1 + 1/*0*/;

	/*	int worldHeightMapNegXBound = -535;
		int worldHeightMapPosXBound = 535;
		int worldHeightMapNegZBound = -535;
		int worldHeightMapPosZBound = 535;*/

		/*if (xIndex1 < worldHeightMapNegXBound || xIndex1 > worldHeightMapPosXBound || zIndex1 < worldHeightMapNegZBound || zIndex1 > worldHeightMapPosZBound) {
			cout << "OUT OF BOUNDS!" << endl;
			p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			p.currInnerState = innerStates::Off;
			p.currPhysState = PhysicsStates::None;
			return;
		}*/
		// safety net for test
		if (p.currPhysState == PhysicsStates::HoldingEdge) {
			int jjj = 0;
		}


		if (p.position.y <= -500.0f) {
			p.position = glm::vec3(200, 500, -300);
			//p.feetPlanted = true;
			p.velocity.y = 0.0f;
			p.velocityDiff.y = 0.0f;
			
			p.health = 0;
			p.numDeaths++;
			p.deathState = true;
			
			int respawnTime = 0;
			ConfigSettings::getConfig()->getValue("RespawnTime", respawnTime);
			p.timeUntilRespawn = respawnTime;
			//p.canClimb = true;
			return;
		}

		/*if (p.currPhysState != PhysicsStates::Climbing && p.currPhysState != PhysicsStates::HoldingEdge &&
		p.currPhysState != PhysicsStates::PullingUp && p.currPhysState != PhysicsStates::WallRunning) {*/
			
			/*cout << "player aabb:";
			p.aabb.print();*/

		float gravityConstant = 0;
		ConfigSettings::getConfig()->getValue("gravityConstant", gravityConstant);
		if (p.currPhysState == PhysicsStates::None) {
			p.velocity += glm::vec3(0.0f, gravityConstant, 0.0f);
		}

		//cout << "falling at: " << glm::to_string(p.position) << endl;
		//cout << "falling: heightmap at: " << World::m_heightMap[xIndex + World::m_heightMapXShift][zIndex + World::m_heightMapZShift] << ", " << "player at: " << p.position.y + p.velocity.y << endl;
		//cout << "p.velo+p.velodiff: " << glm::to_string(p.velocity + p.velocityDiff) << endl;
		p.position += p.velocity + p.velocityDiff;
		computeAABB(&p);
		p.position = oldPos;
		for (int i = 0; i < staticObjects.size(); i++) {

			// hit a ceiling
			if (staticObjects[i].aabb.cameFromBottom(p.position, p.position + p.velocity + p.velocityDiff, p.aabb, i)) {
				p.position.y = staticObjects[i].aabb.minY - YOFFSET; // technically not needed
				p.velocity.y = 0.0f;
				p.velocityDiff.y = 0.0f;
				cout << "Hit ceiling" << endl;
				if (p.currPhysState != PhysicsStates::None) {
					StateLogic::statesInfo[p.id].End.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
					p.currInnerState = innerStates::Ending;
				}
				break;

			}
			//if (p.currPhysState == PhysicsStates::None) {

				// landed
			if (staticObjects[i].aabb.cameFromTop(p.position, p.position + p.velocity + p.velocityDiff, p.aabb, i)) {
				p.position.y = staticObjects[i].aabb.maxY + YOFFSET; // on ground
				//cout << "landed: " << i << endl;
				//cout << "pposition: " << glm::to_string(p.position) << endl;
				p.feetPlanted = true;
				p.velocity.y = 0.0f;
				p.velocityDiff.y = 0.0f;
				p.canClimb = true;
				if (p.currPhysState != PhysicsStates::None) {
					StateLogic::statesInfo[p.id].End.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
					p.currInnerState = innerStates::Ending;
				}
				break;
			}
			//}
		}
		//}

	}

}

void DynamicWorld::applyAdjustments() {
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		
		int xIndex1 = (int)floor(p.position.x + p.velocity.x + p.velocityDiff.x /*- XOFFSET*/);
		int zIndex1 = (int)floor(p.position.z + p.velocity.z + p.velocityDiff.z /*- ZOFFSET*/);

		int xIndex2 = xIndex1 /*+ 2*XOFFSET*/;
		int zIndex2 = zIndex1 /*+ 2*ZOFFSET*/;

		/*int worldHeightMapNegXBound = -535;
		int worldHeightMapPosXBound = 535;
		int worldHeightMapNegZBound = -535;
		int worldHeightMapPosZBound = 535;

		if (xIndex1 < worldHeightMapNegXBound || xIndex1 > worldHeightMapPosXBound || zIndex1 < worldHeightMapNegZBound || zIndex1 > worldHeightMapPosZBound) {
			cout << "OUT OF BOUNDS!" << endl;
			p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			p.currInnerState = innerStates::Off;
			p.currPhysState = PhysicsStates::None;
			return;
		}*/

		/*cout << "player pos: ";
		p.aabb.print();*/

		// hardcoding cuz idk why this aint working
		//if (p.currPhysState == PhysicsStates::HoldingEdge) {
		//	p.velocityDiff.y = 0.0f;
		//	p.velocity.y = 0.0f;
		//}

		float playerHeight = 0;
		ConfigSettings::getConfig()->getValue("PlayerHeight", playerHeight);
		
		float velocityDecayFactor = 0.95f;
		//ConfigSettings::getConfig()->getValue("velocityDecayFactor ", velocityDecayFactor );

		if (p.currPhysState == PhysicsStates::HoldingEdge) {
			cout << "pos: " << glm::to_string(p.cameraObject.cameraLookAt) << endl;
		}

		p.velocity += p.velocityDiff;
		p.position += p.velocity;
		p.velocity -= p.velocityDiff;
		p.velocity.x *= velocityDecayFactor;
		p.velocity.z *= velocityDecayFactor;
		p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);

		computeAABB(&p);

		/*cout << "player aabb:";
		p.aabb.print();*/
		
		//ANDRE
		//cout << "p.cameraObject.xrotated: " << p.cameraObject.xRotated << endl;
		p.cameraObject.cameraCenter = glm::vec3(p.position.x, p.aabb.minY + playerHeight, p.position.z);
		p.cameraObject.cameraLookAt = p.cameraObject.cameraCenter + p.cameraObject.camZ;

		if (p.currPhysState != PhysicsStates::WallRunning) {
			p.cameraObject.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		
	}
}

void DynamicWorld::applyPhysics()
{
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject *p = &it->second;
		switch (p->currPhysState) {
			case PhysicsStates::Climbing:
				StateLogic::applyClimbing(p);
				break;
			case PhysicsStates::HoldingEdge:
				StateLogic::applyHoldingEdge(p);
				/*cout << "velo: " << p->velocity;
				cout << "velodiff: " << glm::to_string(p->velocityDiff) << endl;*/
				break;
			case PhysicsStates::PullingUp:
				StateLogic::applyPullingUp(p);
				break;
			case PhysicsStates::WallRunning:
				StateLogic::applyWallRunning(p);
				break;
		}
		checkForStateChanges(p);
	}
}

void DynamicWorld::checkForStateChanges(struct playerObject *p) {
	float climbingMaxDuration = 3000.0f;
	ConfigSettings::getConfig()->getValue("climbingMaxDuration", climbingMaxDuration);
	float holdingEdgeMaxDuration = 3000.0f;
	ConfigSettings::getConfig()->getValue("holdingEdgeMaxDuration", holdingEdgeMaxDuration);
	switch (p->currPhysState) {
	case PhysicsStates::Climbing:
		if (staticObjects[p->interactingWithBuildingId].aabb.nearTop(p->position)) {
			StateLogic::startHoldingEdge(p, p->interactingWithBuildingId);
			return;
		}		
		if (p->stopwatch.getElapsedMilliseconds() > climbingMaxDuration) {
			cout << "took too long to climb " << endl;
			// technically below two lines dont do anything
			/*p->stopwatch.stop();
			p->stopwatch.reset();*/
			p->currInnerState = innerStates::Ending;
			return;
		}
		break;
	case PhysicsStates::HoldingEdge:
		
		if (staticObjects[p->interactingWithBuildingId].aabb.clearedTop(p->aabb)) {
			cout << "cleared top" << endl;
			StateLogic::startPullingUp(p, p->interactingWithBuildingId);
			return;
		}
		// not sure if needed. also in holdingedgemoveevent but commented
		else if (p->currInnerState != innerStates::Ending && staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb)) {
			glm::vec3 oldPos = p->position;
			cout << glm::to_string(p->cameraObject.camZ) << endl;
			p->position += 10.0f*glm::vec3(p->cameraObject.camZ.x, 0.0f, p->cameraObject.camZ.z);
			computeAABB(p);
			int buildingId = checkSideCollisionsWithAllBuildings(oldPos, p->position, p);
			p->position = oldPos;
			computeAABB(p);
			if (buildingId != -1) {
				cout << "SHOULD SWITCH BUILDINGS" << endl;
			}
			cout << "fell off side while holding edge" << endl;
			p->currInnerState = innerStates::Ending;
			return;
		}
		else if (p->currInnerState != innerStates::Ending && p->stopwatch.getElapsedMilliseconds() > holdingEdgeMaxDuration) {
			cout << "took too long while holding edge " << endl;
			// technically below two lines dont do anything
			/*p->stopwatch.stop();
			p->stopwatch.reset();*/
			p->currInnerState = innerStates::Ending;
			return;
		}
	case PhysicsStates::WallRunning:
		if (p->currInnerState != innerStates::Ending && staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb)) {
			glm::vec3 oldPos = p->position;
			glm::vec3 WRcamUp = StateLogic::statesInfo[p->id].Start.camUp;
			p->position += -1.0f*(1.0f / WRcamUp.y)*WRcamUp;
			computeAABB(p);
			int buildingId = checkSideCollisionsWithAllBuildings(oldPos, p->position, p);
			p->position = oldPos;
			computeAABB(p);
			if (buildingId != -1) {
				cout << "SHOULD SWITCH BUILDINGS" << endl;
			}
			cout << "fell off edge while wallrunning" << endl;
			StateLogic::statesInfo[p->id].End.camUpIncrement = (StateLogic::statesInfo[p->id].initialUp - p->cameraObject.cameraUp) / (StateLogic::statesInfo[p->id].End.fraction*StateLogic::statesInfo[p->id].numFrames);
			p->currInnerState = innerStates::Ending;
			return;
		}
		break;
	}

}

void DynamicWorld::processJumpEvent(int playerId)
{
	struct playerObject *p = &playerMap[playerId];
	//noneJumpEvent(pkt);
	
	switch (p->currPhysState) {
		case None:
			noneJumpEvent(playerId);
			break;
		case Climbing:
			climbingJumpEvent(playerId);
			break;
		case PullingUp:
			pullingUpJumpEvent(playerId);
			break;
		case HoldingEdge:
			holdingEdgeJumpEvent(playerId);
			break;
		case WallRunning:
			wallRunningJumpEvent(playerId);
			break;
	}
	
}

void DynamicWorld::processLookEvent(int playerId, struct cameraObject *cam)
{
	//cout << "process" << endl;
	
	struct playerObject *p = &playerMap[playerId];

	memcpy(&p->cameraObject, cam, sizeof(struct cameraObject));
	StateLogic::handleXRotation(p, cam->xAngle);
	StateLogic::handleYRotation(p, cam->yAngle);
	/*cout << "xrotated: " << p->cameraObject.xRotated << endl;
	cout << "yrotated: " << p->cameraObject.yRotated << endl;*/
}

void DynamicWorld::noneJumpEvent(int playerId) {
	
	float jumpFactor = 0;
	ConfigSettings::getConfig()->getValue("jumpFactor", jumpFactor);

	struct playerObject *p = &playerMap[playerId];
	if (p->feetPlanted) {
		cout << "None jump event sucessful" << endl;
		p->velocity.y += jumpFactor;
		p->feetPlanted = false;
	}
	else {
		cout << "None jump event NOT sucessful" << endl;
	}

	return;
}

void DynamicWorld::wallRunningJumpEvent(int playerId) {

	cout << "wall running jump event" << endl;
	struct playerObject *p = &playerMap[playerId];

	if (p->currInnerState != innerStates::Ending) {
		glm::vec3 WRBounceFactor = glm::vec3(100.0f, 1.0f, 100.0f);
		//ConfigSettings::getConfig()->getValue("WRBounceFactor", WRBounceFactor);
		StateLogic::statesInfo[p->id].Holder.velocityDiff.x *= WRBounceFactor.x;
		StateLogic::statesInfo[p->id].Holder.velocityDiff.z *= WRBounceFactor.z;
		StateLogic::statesInfo[p->id].End.camUpIncrement = (StateLogic::statesInfo[p->id].initialUp - p->cameraObject.cameraUp) / (StateLogic::statesInfo[p->id].End.fraction*StateLogic::statesInfo[p->id].numFrames);
		p->currInnerState = innerStates::Ending;
	}

	return;
}

void DynamicWorld::holdingEdgeJumpEvent(int playerId) {
	cout << "decided to jump off while holding edge" << endl;
	struct playerObject *p = &playerMap[playerId];
	p->currInnerState = innerStates::Ending;
	//TODO: maybe more
	return;
}

void DynamicWorld::pullingUpJumpEvent(int playerId) {
	cout << "tried to jump while pulling up.. this wont do much" << endl;
	return;
}

void DynamicWorld::climbingJumpEvent(int playerId) {
	// TODO: disable climbing?
	//struct playerObject *p = &playerMap[playerId];

	//cout << "decided to jump back down from climbing" << endl;
	//p->currInnerState = innerStates::Ending;
	//TODO: maybe more, set Holders

}

void DynamicWorld::checkPlayersCollideWithTrap()
{
	for (map<int, struct playerObject>::iterator pit = playerMap.begin(); pit != playerMap.end(); pit++)
	{
		struct playerObject *p = &pit->second;

		for (map<int, struct trapObject>::iterator it = trapMap.begin(); it != trapMap.end(); it++)
		{
			if (it->second.timeTillActive <= 0 && it->second.aabb.collidesWith(p->aabb) && it->second.eventCode == 0)
			{
				printf("Collision: player %d with trap id %d\n", p->id, it->second.id);
				playerLock[p->id] = true;
				bool okToRemove = true;

				switch (it->second.type)
				{
					case TYPE_FREEZE_TRAP:
					{
						int stunDuration = 0;
						ConfigSettings::getConfig()->getValue("StunTrapDuration", stunDuration);
						p->stunDuration = stunDuration;
						break;
					}
					case TYPE_TRAMPOLINE_TRAP:
					{
						int trampolinePower = 0;
						ConfigSettings::getConfig()->getValue("TrampolinePower", trampolinePower);
						p->velocity = glm::vec3(0, trampolinePower, 0);

						break;
					}
					case TYPE_SLOW_TRAP:
					{
						int slowDuration = 0;
						ConfigSettings::getConfig()->getValue("SlowTrapDuration", slowDuration);
						p->slowDuration = slowDuration;
						break;
					}
					case TYPE_PUSH_TRAP:
					{
						float pushPower = 0;
						ConfigSettings::getConfig()->getValue("PushTrapPower", pushPower);

						glm::vec3 force = glm::rotateY(glm::vec3(0, 0, -1), it->second.rotationAngle);
						force *= pushPower;

						p->velocity = glm::vec3(force.x, 0, force.z);

						break;
					}
					case TYPE_LIGHTNING_TRAP:
					{
						float power = 0;
						ConfigSettings::getConfig()->getValue("LightningTrapPower", power);

						playerDamage(&playerMap[it->second.ownerId], p, power);

						// Flashes everyone
						int flashEffect = 0;
						ConfigSettings::getConfig()->getValue("LightningTrapFlashEffect", flashEffect);
						for (map<int, struct playerObject>::iterator iterator = playerMap.begin(); iterator != playerMap.end(); iterator++)
						{
							if (iterator->second.flashDuration < flashEffect)
								iterator->second.flashDuration = flashEffect;
						}

						break;
					}
					case TYPE_PORTAL_TRAP:
					{
						okToRemove = false;

						// Has to hit his own portal
						if (it->second.ownerId == p->id)
						{
							// Find a teammate's portal
							for (map<int, struct trapObject *>::iterator portalItr = portalMap.begin(); portalItr != portalMap.end(); portalItr++)
							{
								// Found teammate's portal, teleport successful!
								if (portalItr->second->ownerId != p->id && portalItr->second->ownerId % 2 == p->id % 2)
								{
									addInfoMessage(portalItr->second->ownerId, "Player " + to_string(p->id) + " used your portal");

									okToRemove = true;
									p->position = portalItr->second->pos;
									portalItr->second->eventCode = EVENT_REMOVE_TRAP;
									portalMap.erase(portalItr);
									portalMap.erase(it->second.ownerId);
									break;
								}
							}
						}

						break;
					}
					case TYPE_FLASH_TRAP:
					{
						float flashDuration = 0;
						ConfigSettings::getConfig()->getValue("FlashTrapDuration", flashDuration);

						p->flashDuration = flashDuration;

						break;
					}
					default:
						break;
				}

				if (okToRemove)
				{
					it->second.eventCode = EVENT_REMOVE_TRAP;
					ConfigSettings::getConfig()->getValue("HitCrosshairDuration", playerMap[it->second.ownerId].hitCrosshair);
					return;
				}

			} // end of if hit
		}
	}
}

void DynamicWorld::addAABBInfo(int type, AABB aabb)
{
	//printf("[COMMON]: Got AABB info: type %d: ", type); 
	//aabb.print();

	if (type == TYPE_PLAYER) {
		AABB tmp = AABB(-1*XOFFSET, -1*YOFFSET, -1*ZOFFSET, XOFFSET, YOFFSET, ZOFFSET);
		aabbOffsets[type] = tmp;
	}
	else {
		if (aabb.maxY < 5) 
			aabb.maxY = 5;
		aabbOffsets[type] = aabb;
	}
}

void DynamicWorld::handleKnifeEvent(int knifer)
{
	playerObject *player = &playerMap[knifer];

	
	for (map<int, struct playerObject>::iterator pit = playerMap.begin(); pit != playerMap.end(); pit++)
	{
		struct playerObject *target = &pit->second;

		// Make sure they're on different teams & knife is ready
		if (/*player->knifeDelay <= 0 &&*/ player->id % 2 != target->id % 2)
		{
			ConfigSettings::getConfig()->getValue("KnifeDelay", player->knifeDelay);

			float knifeRange = 0.0f;
			ConfigSettings::getConfig()->getValue("KnifeRange", knifeRange);

			glm::vec3 lookAt = player->cameraObject.cameraLookAt;
			glm::vec3 center = player->cameraObject.cameraCenter;
			glm::vec3 difVec = lookAt - center;
			glm::vec3 hitPt = center + (knifeRange * difVec);

			if (hitPt.x >= target->aabb.minX && hitPt.x <= target->aabb.maxX
				&& hitPt.y >= target->aabb.minY && hitPt.y <= target->aabb.maxY
				&& hitPt.z >= target->aabb.minZ && hitPt.z <= target->aabb.maxZ)
			{
				playerDamage(player, target, KNIFE_HIT_DMG);
			}
		}
	}
}

void DynamicWorld::addInfoMessage(int destination, string msg)
{
	struct infoMsg info = {};
	info.destination = destination;
	info.msg = msg;
	infoMsgQueue.push_back(info);

}