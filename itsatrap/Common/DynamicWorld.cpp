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
	initStateInfo();
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
	initStateInfo();
}

void DynamicWorld::initStateInfo() {
	/*
	float climbLookUp = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookUp", climbLookUp);
	float climbLookUpDownNumFrames = 0;
	ConfigSettings::getConfig()->getValue("climbLookUpDownNumFrames", climbLookUpDownNumFrames);
	p.physicsObject.climbLookUpIncrement = climbLookUp / climbLookUpDownNumFrames;

	float climbLookDown = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookDown", climbLookDown);
	float climbLookDownReadjustNumFrames = 0;
	ConfigSettings::getConfig()->getValue("climbLookDownReadjustNumFrames", climbLookDownReadjustNumFrames);
	float climbLookDownNumFramesFraction = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookDownNumFramesFraction", climbLookDownNumFramesFraction);
	p.physicsObject.climbLookDownIncrement = climbLookDown*2.0f / (climbLookDownReadjustNumFrames*climbLookDownNumFramesFraction);

	float climbLookReadjustNumFramesFraction = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookReadjustNumFramesFraction", climbLookReadjustNumFramesFraction);
	p.physicsObject.climbLookReadjustIncrement = -1.0f*climbLookDown*2.0f / (climbLookDownReadjustNumFrames*climbLookReadjustNumFramesFraction);

	float climbLookRight = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookRight", climbLookRight);
	float climbLookXsNumFrames = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookXsNumFrames", climbLookXsNumFrames);
	float climbLookXNumFramesFraction = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookXNumFramesFraction", climbLookXNumFramesFraction);
	p.physicsObject.climbLookXIncrement = climbLookRight / (climbLookXsNumFrames*climbLookXNumFramesFraction);

	float climbLookBackXNumFramesFraction = 0.0f;
	ConfigSettings::getConfig()->getValue("climbLookBackXNumFramesFraction", climbLookBackXNumFramesFraction);
	p.physicsObject.climbLookBackXIncrement = -1.0f*climbLookRight / (climbLookXsNumFrames*climbLookBackXNumFramesFraction);
	*/
	
}

void DynamicWorld::addNewPlayer(struct playerObject p)
{
	while (checkCollisionsWithAllNonTraps(&p) == -1)
	{
		p.aabb.minX += 10;
		p.aabb.maxX += 10;
		p.position.x += 10;
	}

	p.health = 100;
	p.numKills = 0;
	p.numDeaths = 0;
	p.stunDuration = 0;
	p.slowDuration = 0;

	playerMap[p.id] = p; 

	
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

	// OVERRIDE STUFF FROM CLIENT
	p.health = playerMap[p.id].health;
	p.stunDuration = playerMap[p.id].stunDuration;
	p.slowDuration = playerMap[p.id].slowDuration;
	p.numKills = playerMap[p.id].numKills;
	p.numDeaths = playerMap[p.id].numDeaths;
	p.resources = playerMap[p.id].resources;
	p.velocityDiff = playerMap[p.id].velocityDiff;
	p.velocity = playerMap[p.id].velocity;


	if (checkCollisionsWithAllNonTraps(&p) == -1)
		return;
	
	for (map<int, struct trapObject>::iterator it = trapMap.begin(); it != trapMap.end(); it++)
	{
		if (p.id != it->second.ownerId && it->second.aabb.collidesWith(p.aabb) && it->second.eventCode == 0)
		{
			printf("Collision: player %d with trap id %d\n", p.id, it->second.id);
			playerLock[p.id] = true;
			switch (it->second.type)
			{
				case TYPE_FREEZE_TRAP:
				{
					int stunDuration = 0;
					ConfigSettings::getConfig()->getValue("StunTrapDuration", stunDuration);
					p.stunDuration = stunDuration;
					break;
				}
				case TYPE_TRAMPOLINE_TRAP:
				{
					int trampolinePower = 0;
					ConfigSettings::getConfig()->getValue("TrampolinePower", trampolinePower);
					p.velocityDiff = glm::vec3(0, trampolinePower, 0);

					break;
				}
				case TYPE_SLOW_TRAP:
				{
					int slowDuration = 0;
					ConfigSettings::getConfig()->getValue("SlowTrapDuration", slowDuration);
					p.slowDuration = slowDuration;
					break;
				}
				case TYPE_PUSH_TRAP:
				{
					float pushPower = 0;
					ConfigSettings::getConfig()->getValue("PushTrapPower", pushPower);

					glm::vec3 force = glm::rotateY(glm::vec3(0, 0, -1), it->second.rotationAngle);
					force*=pushPower;

					p.velocityDiff = glm::vec3(force.x, 0, force.z);

					break;
				}
				case TYPE_LIGHTNING_TRAP:
				{
					float power = 0;
					ConfigSettings::getConfig()->getValue("LightningTrapPower", power);

					playerDamage(&playerMap[it->second.ownerId], &p, power);

					break;
				}
				default:
					break;
			}

			it->second.eventCode = EVENT_REMOVE_TRAP;

			break;
		}
	}

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
int DynamicWorld::serialize(char **ptr)
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

	char *buf = (char *) malloc(totalSize);
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
	

	*ptr = buf;

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

int DynamicWorld::getNumStaticObjects() 
{
	return staticObjects.size();
}

void DynamicWorld::addTrap(struct trapObject t)
{
	t.eventCode = EVENT_ADD_TRAP;
	t.id = currentId;
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
			printf("Collision: player %d with player %d\n", e->id, players[i].id);
			return true;
		}
	}
	for (int i = 0; i < staticObjects.size(); i++)
	{
		// Something wrong with building#40
		if (!(e->onTopOfBuildingId == i) && e->aabb.collidesWith(staticObjects[i].aabb))
		{
			printf("Collision: player %d with static object %d\n", e->id, i);
			return i;
		}
	}

	return -1;
}

int DynamicWorld::checkSideCollisionsWithAllNonTraps(struct playerObject *e)
{
	vector<struct playerObject> players = getAllPlayers();
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].id != e->id && e->aabb.collidesWithSide(players[i].aabb))
		{
			printf("Collision: player %d with player %d\n", e->id, players[i].id);
			return true;
		}
	}
	for (int i = 0; i < staticObjects.size(); i++)
	{
		// Something wrong with building#40
		if (!(e->onTopOfBuildingId == i) && e->aabb.collidesWithSide(staticObjects[i].aabb))
		{
			printf("Collision: player %d with static object %d\n", e->id, i);
			return i;
		}
	}

	return -1;
}

void DynamicWorld::updatePlayerBuffs(int timeDiff)
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
	p->position = glm::vec3(75, 0, 0);
	computeTemporaryAABB(p);

	while(checkCollisionsWithAllNonTraps(p) == -1)
	{
		p->position.x += 10;
		computeTemporaryAABB(p);
	}

	p->health = 100;
	p->timeUntilRespawn = 0;
	p->deathState = false;
}

void DynamicWorld::computeTemporaryAABB(struct playerObject *p)
{
	p->aabb.minX = p->position.x - 5;
	p->aabb.maxX = p->position.x + 5;
	p->aabb.minY = p->position.y - 5;
	p->aabb.maxY = p->position.y + 5;
	p->aabb.minZ = p->position.z - 5;
	p->aabb.maxZ = p->position.z + 5;
}

void DynamicWorld::processMoveEvent(struct moveEventPacket *pkt)
{


	struct playerObject *p = &playerMap[pkt->playerId];

	switch (p->currState) {
		case None:
			noneMoveEvent(pkt);
			break;
		case Climbing:
			//climbingMoveEvent(pkt);
			break;
		case PullingUp:
			//pullingUpMoveEvent(pkt);
			break;
		case HoldingEdge:
			//holdingEdgeMoveEvent(pkt);
			break;
		case WallRunning:
			//wallRunningMoveEvent(pkt);
			break;
	}

}

void changeStates() {
}

void DynamicWorld::noneMoveEvent(struct moveEventPacket *pkt)
{
	struct playerObject *p = &playerMap[pkt->playerId];

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
	switch (pkt->direction)
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
	p->velocityDiff += toAdd;

	glm::vec3 oldPos = p->position;
	glm::vec3 newPos;
	
	p->position = proposedNewPos;
	computeTemporaryAABB(p);
	int buildingId = checkSideCollisionsWithAllNonTraps(p);
	p->position = oldPos;

	if (buildingId != -1) {
		int oldOnTopOfBuildingId = p->onTopOfBuildingId;
		p->onTopOfBuildingId = -1;
		// TODO: check guy is facing wall too, at rest
		if (!p->feetPlanted /*&& !(m_physics->atRest()) */ && pkt->direction == FORWARD) {
			float angle = Physics::handleAngleIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
			if (abs(90.0f-angle) < 22.5f /*&& m_physics->m_velocity.y >= m_miniJumpYVelocityThreshold*/) {
				newPos = oldPos;
				//cout << "starting the climb with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << buildingId << endl;
				//m_cam->m_cameraLookAtWallJump = m_cam->m_cameraCenter - m_cam->m_camZ;
				startClimbing(p, buildingId);
				return;
			}
			else {
				//if (oldOnTopOfBuildingId != buildingId) {
				//if (m_onTopOfBuildingId != -1) {
					//cout << "starting the wallrunning with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << buildingId << endl;
				// 0,1 = x, -1 = y, 4,5 = z
				int newDirection = Physics::handleReflectionIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
				startWallRunning(p, newDirection, toAdd, angle);
				return;
					
				//}
				// TODO: need else?
				/*else {
				}*/
			}
		}
	
		else {
			toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
			p->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			p->velocity = glm::vec3(0.0f, p->velocity.y, 0.0f);
		}
	}
	else {
		newPos = proposedNewPos;
	}
	p->velocityDiff+=toAdd;

}

void DynamicWorld::startClimbing(struct playerObject *e, int buildingId) {
	e->currState = PhysicsStates::Climbing;
	e->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	e->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	e->interactingWithBuildingId = buildingId;
}

void DynamicWorld::startWallRunning(struct playerObject *e, int newDirection, glm::vec3 toAdd, float angle) {
	e->currState = PhysicsStates::WallRunning;

}


void DynamicWorld::applyGravity()
{
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		p.velocity -= glm::vec3(0, 0.01, 0);
	}
}

void DynamicWorld::applyPhysics()
{
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject *p = &it->second;
		 
	}
}

void DynamicWorld::processJumpEvent(struct jumpEventPacket *pkt)
{
	struct playerObject *p = &playerMap[pkt->playerId];
	// do stuff with p
}