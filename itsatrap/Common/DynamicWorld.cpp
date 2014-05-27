#include "DynamicWorld.h"

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

	team1RespawnPoints.push_back(glm::vec3(0, 50, 19 * UNIT_SIZE));
	team2RespawnPoints.push_back(glm::vec3(0, 50, -19 * UNIT_SIZE));
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

	// why not update resources?
	p.health = 100;
	p.numKills = 0;
	p.numDeaths = 0;
	p.stunDuration = 0;
	p.slowDuration = 0;
	p.feetPlanted = true;
	p.interactingWithBuildingId = -1;
	p.interactingWIthBuildingFace = -1;
	p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	p.feetPlanted = true;
	p.triedToRun = false;
	p.currPhysState = PhysicsStates::None;
	p.currInnerState = innerStates::Off;
	p.currCamState = CameraStates::Client;
	playerMap[p.id] = p;
	p.stopwatch = Stopwatch();
	p.canClimb = true;
	p.knifeDelay = 0;
	p.timeUntilRegen = 0;
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
	p.triedToRun = playerMap[p.id].triedToRun;
	p.interactingWithBuildingId = playerMap[p.id].interactingWithBuildingId;
	p.interactingWIthBuildingFace = playerMap[p.id].interactingWIthBuildingFace;
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
			printf("Sending event %d about trap %d\n", it->second.eventCode, it->second.id);
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

int DynamicWorld::getNumStaticRampObjects()
{
	return staticRampObjects.size();
}

int DynamicWorld::getNumStaticResourceObjects()
{
	return staticResourceObjects.size();
}

void DynamicWorld::addTrap(struct trapObject t)
{
	t.eventCode = EVENT_ADD_TRAP;
	t.id = currentId;
	int timeTillActive = 0;
	ConfigSettings::getConfig()->getValue("TrapInactivePeriod", timeTillActive);
	t.timeTillActive = timeTillActive;
	trapMap[currentId] = t;
	
	playerLock[t.ownerId] = true;
	switch (t.type)
	{
		case TYPE_FREEZE_TRAP:
			playerMap[t.ownerId].resources -= 20;
			break;

		case TYPE_TRAMPOLINE_TRAP:
			playerMap[t.ownerId].resources -= 30;
			break;

		case TYPE_SLOW_TRAP:
			playerMap[t.ownerId].resources -= 15;
			break;

		case TYPE_PUSH_TRAP:
			playerMap[t.ownerId].resources -= 15;
			break;

		case TYPE_LIGHTNING_TRAP:
			playerMap[t.ownerId].resources -= 75;
			break;

		case TYPE_PORTAL_TRAP:
		{
			playerMap[t.ownerId].resources -= 10;

			if (portalMap[t.ownerId] != nullptr)
			{
				portalMap[t.ownerId]->eventCode = EVENT_REMOVE_TRAP;
				portalMap.erase(t.ownerId);
			}
			portalMap[t.ownerId] = &trapMap[t.id];
			break;
		}

		default:
			playerMap[t.ownerId].resources -= 10;
			break;
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

	return -1;
}

int DynamicWorld::checkSideCollisionsWithAllBuildings(struct playerObject *e)
{
	for (int i = 0; i < staticObjects.size(); i++)
	{
		// Something wrong with building#40
		if (e->aabb.collidesWithSide(staticObjects[i].aabb))
		{
			printf("Collision: player %d with static object %d\n", e->id, i);
			return i;
		}
	}

	//if (e->aabb.collidesWithSide(staticObjects[18].aabb)) {
	//	return 18;
	//}

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
			if (p.timeUntilRespawn < 0)
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

		attacker->numKills++;

		int killBonusResource = 0;
		ConfigSettings::getConfig()->getValue("KillBonusResource", killBonusResource);
		attacker->resources += killBonusResource;
	}
}

void DynamicWorld::respawnPlayer(struct playerObject *p) {
	
	if (p->id % 2 == 0)
		p->position = team1RespawnPoints[rand() % team1RespawnPoints.size()];
	else
		p->position = team2RespawnPoints[rand() % team2RespawnPoints.size()];
	
	computeAABB(p);

	p->health = 100;
	p->timeUntilRespawn = 0;
	p->deathState = false;
}

void DynamicWorld::computeAABB(struct playerObject *p)
{
	p->aabb.minX = p->position.x - 5;
	p->aabb.maxX = p->position.x + 5;
	p->aabb.minY = p->position.y - 5;
	p->aabb.maxY = p->position.y + 5;
	p->aabb.minZ = p->position.z - 5;
	p->aabb.maxZ = p->position.z + 5;
}

void DynamicWorld::processMoveEvent(int playerId, Direction dir)
{
	struct playerObject *p = &playerMap[playerId];
	//noneMoveEvent(playerId, dir);
	switch (p->currPhysState) {
		case None:
			noneMoveEvent(playerId, dir);
			break;
		case Climbing:
			climbingMoveEvent(playerId, dir);
			break;
		case PullingUp:
			pullingUpMoveEvent(playerId, dir);
			break;
		case HoldingEdge:
			holdingEdgeMoveEvent(playerId, dir);
			break;
		case WallRunning:
			wallRunningMoveEvent(playerId, dir);
			break;
	}
	

}

void DynamicWorld::noneMoveEvent(int playerId, Direction dir)
{
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
	glm::vec3 toAdd;
	switch (dir)
	{
		case FORWARD:
			proposedNewPos = p->position + zWalkFactor*tmp_camZ;
			toAdd = zWalkFactor*tmp_camZ;
			break;

		case BACKWARD:
			proposedNewPos = p->position + -1.0f*zWalkFactor*tmp_camZ;
			toAdd = -1.0f*zWalkFactor*tmp_camZ;
			break;

		case LEFT:
			proposedNewPos = p->position + -1.0f*xWalkFactor*p->cameraObject.camX;
			toAdd = -1.0f*xWalkFactor*p->cameraObject.camX;
			break;		

		case RIGHT:
			proposedNewPos = p->position + xWalkFactor*p->cameraObject.camX;
			toAdd = xWalkFactor*p->cameraObject.camX;
			break;
		default:
			break;
	}
	
	glm::vec3 oldPos = p->position;
	glm::vec3 newPos;
	
	//cout << "pos: " << glm::to_string(p->position) << endl;
	//cout << "newPos: " << glm::to_string(proposedNewPos) << endl;
	p->position = proposedNewPos;
	p->position.y += 5.0f;
	computeAABB(p);
	int collisionId = checkCollisionsWithAllNonTraps(p);
	p->position = oldPos;
	computeAABB(p);
	//cout << "collision id: " << collisionId << endl;
	if (collisionId != -1) {
		p->position = proposedNewPos;
		p->position.y += 5.0f;
		computeAABB(p);
		int buildingId = checkSideCollisionsWithAllBuildings(p);
		p->position = oldPos;
		computeAABB(p);
		cout << "buildingId: " << buildingId << endl;
		if (buildingId != -1) {
			cout << "collided with building" << endl;
			// TODO: check guy is facing wall too, at rest
			if (!p->feetPlanted /*&& !(m_physics->atRest()) */ && dir == FORWARD) {
				cout << "doing extra logic!" << endl;
				if (Physics::handleNearTop(proposedNewPos, staticObjects[buildingId])) {
					//TODO: startHoldingEdge(p);
					//return;
				}

				float angle = Physics::handleAngleIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
				//if (abs(90.0f-angle) < 22.5f /*&& m_physics->m_velocity.y >= m_miniJumpYVelocityThreshold*/) {
				newPos = oldPos;
				//cout << "starting the climb with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << buildingId << endl;
				//m_cam->m_cameraLookAtWallJump = m_cam->m_cameraCenter - m_cam->m_camZ;
				StateLogic::startClimbing(p, buildingId);
				p->stopwatch.start();
				return;
				//}
				/*
				else {
				//if (oldOnTopOfBuildingId != buildingId) {
				//if (m_onTopOfBuildingId != -1) {
				//cout << "starting the wallrunning with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << buildingId << endl;
				// 0,1 = x, -1 = y, 4,5 = z
				int newDirection = Physics::handleReflectionIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
				startWallRunning(p, newDirection, toAdd, angle, buildingId);
				return;

				//}

				}*/
			}
			else {
				toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
				p->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
				p->velocity = glm::vec3(0.0f, p->velocity.y, 0.0f);
			}
		}

	
		else {
			toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
			p->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			p->velocity = glm::vec3(0.0f, p->velocity.y, 0.0f);
		}
	}

	p->velocityDiff += toAdd;

}

void DynamicWorld::climbingMoveEvent(int playerId, Direction dir) {
	cout << "handling climbing move event" << endl;
	struct playerObject *p = &playerMap[playerId];

	// TODO: enable movement?
	switch (dir)
	{

		case BACKWARD:
			cout << "decided to fall back down from climbing" << endl;
			p->currInnerState = innerStates::Ending;
			//TODO: maybe more, set Holders
			return;
			break;

		default:
			break;
	}
	
}

void DynamicWorld::holdingEdgeMoveEvent(int playerId, Direction dir) {
	cout << "handling holding edge move event" << endl;
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
	
	glm::vec3 toAdd;
	// TODO: depend on building face rather than where you looking at
	switch (dir)
	{
		case FORWARD:
			cout << "started pulling up" << endl;
			StateLogic::startPullingUp(p, p->interactingWithBuildingId);
			return;
			break;

		case BACKWARD:
			cout << "decided to fall back down" << endl;
			p->currInnerState = innerStates::Off;
			//TODO: maybe more, set Holders
			return;
			break;

		case LEFT:
			proposedNewPos = p->position + -1.0f*xWalkFactor*p->cameraObject.camX;
			toAdd = -1.0f*xWalkFactor*p->cameraObject.camX;
			break;

		case RIGHT:
			proposedNewPos = p->position + xWalkFactor*p->cameraObject.camX;
			toAdd = xWalkFactor*p->cameraObject.camX;
			break;

		default:
			break;
	}
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

}

void DynamicWorld::pullingUpMoveEvent(int playerId, Direction dir) {

	cout << "why handling pullingupevent wtf??" << endl;
	return;
}

void DynamicWorld::wallRunningMoveEvent(int playerId, Direction dir) {

	cout << "handling wallrunningmoveevent" << endl;
	struct playerObject *p = &playerMap[playerId];
	switch (dir)
	{
		case FORWARD:
			p->triedToRun = true;
			return;
			break;

		default:
			break;
	}
	return;
}


void DynamicWorld::resetWorldInfo() {
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		// TODO: add more if needed
		p.triedToRun = false;
	}
}

// TODO: check for feet?
void DynamicWorld::applyGravity()
{
	//cout << "checking for gravity\n";
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		if (p.currPhysState != PhysicsStates::Climbing && p.currPhysState != PhysicsStates::HoldingEdge &&
			p.currPhysState != PhysicsStates::PullingUp && p.currPhysState != PhysicsStates::WallRunning) {
			
			float gravityConstant = 0;
			ConfigSettings::getConfig()->getValue("gravityConstant", gravityConstant);

			p.velocity += glm::vec3(0.0f, gravityConstant, 0.0f);

			int xIndex = (int)floor(p.position.x + p.velocity.x + p.velocityDiff.x + 0.5f);
			int zIndex = (int)floor(p.position.z + p.velocity.z + p.velocityDiff.z + 0.5f);

			//cout << "falling: heightmap at: " << World::m_heightMap[xIndex + World::m_heightMapXShift][zIndex + World::m_heightMapZShift] << ", " << "player at: " << p.position.y + p.velocity.y << endl;

			if (World::m_heightMap[xIndex + World::m_heightMapXShift][zIndex + World::m_heightMapZShift] > p.position.y + p.velocity.y + p.velocityDiff.y - 5.0f) {
				//cout << "landed: heightmap at: " << World::m_heightMap[xIndex + World::m_heightMapXShift][zIndex + World::m_heightMapZShift] << ", " << "player at: " << p.position.y + p.velocity.y << endl;
				//cout << "landed\n";
				//m_position.y = World::m_heightMap[xIndex + World::m_heightMapXShift][zIndex + World::m_heightMapZShift]; // on ground
				p.feetPlanted = true;
				p.velocity.y = 0.0f;
				p.velocityDiff.y = 0.0f;
				p.canClimb = true;
			}
		}

	}

}

void DynamicWorld::applyAdjustments() {
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		p.velocity += p.velocityDiff;
		p.position += p.velocity;
		p.velocity -= p.velocityDiff;
		p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);

		float playerHeight = 4.0f;
		//ConfigSettings::getConfig()->getValue("playerHeight", playerHeight);

		p.cameraObject.cameraCenter = glm::vec3(p.position.x, p.position.y + playerHeight, p.position.z);
		p.cameraObject.cameraLookAt = p.cameraObject.cameraCenter + p.cameraObject.camZ;
		computeAABB(&p);
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
	//ConfigSettings::getConfig()->getValue("climbingMaxDuration", climbingMaxDuration);
	float holdingEdgeMaxDuration = 3000.0f;
	//ConfigSettings::getConfig()->getValue("holdingEdgeMaxDuration", climbMaxDuration);
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
		else if (staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb)) {
			cout << "fell off side while holding edge" << endl;
			p->currInnerState = innerStates::Ending;
			return;
		}
		else if (p->stopwatch.getElapsedMilliseconds() > holdingEdgeMaxDuration) {
			cout << "took too long while holding edge " << endl;
			// technically below two lines dont do anything
			/*p->stopwatch.stop();
			p->stopwatch.reset();*/
			p->currInnerState = innerStates::Ending;
			return;
		}
	case PhysicsStates::WallRunning:
		if (staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb)) {
			cout << "fell off edge while wallrunning" << endl;
			p->currInnerState = innerStates::Ending;
			return;
		}
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
	struct playerObject *p = &playerMap[playerId];
	memcpy(&p->cameraObject, cam, sizeof(struct cameraObject));
}

void DynamicWorld::noneJumpEvent(int playerId) {
	cout << "None jump event" << endl;
	float yJumpFactor = 0;
	ConfigSettings::getConfig()->getValue("yJumpFactor", yJumpFactor);

	struct playerObject *p = &playerMap[playerId];
	if (p->feetPlanted) {
		p->velocity.y += yJumpFactor;
		p->feetPlanted = false;
	}

}

void DynamicWorld::wallRunningJumpEvent(int playerId) {
	cout << "wall running jump event" << endl;
	struct playerObject *p = &playerMap[playerId];

	//float bounceFactor = 5.0f;
	////ConfigSettings::getConfig()->getValue("bounceFactor", bounceFactor);
	//StateLogic::statesInfo[p->id].End.velocityDiff *= bounceFactor;
	p->currInnerState = innerStates::Ending;
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
	struct playerObject *p = &playerMap[playerId];

	cout << "decided to jump back down from climbing" << endl;
	p->currInnerState = innerStates::Ending;
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
						p->velocityDiff = glm::vec3(0, trampolinePower, 0);

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

						p->velocityDiff = glm::vec3(force.x, 0, force.z);

						break;
					}
					case TYPE_LIGHTNING_TRAP:
					{
						float power = 0;
						ConfigSettings::getConfig()->getValue("LightningTrapPower", power);

						playerDamage(&playerMap[it->second.ownerId], p, power);

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
					default:
						break;
				}

				if (okToRemove)
				{
					it->second.eventCode = EVENT_REMOVE_TRAP;
					return;
				}

			} // end of if hit
		}
	}
}
