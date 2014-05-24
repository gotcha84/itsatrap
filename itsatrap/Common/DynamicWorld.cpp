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
	while (checkCollisionsWithAllNonTraps(&p) != -1)
	{
		p.position.x += 10;
		computeAABB(&p);
	}
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
	p.feetPlanted = playerMap[p.id].feetPlanted;
	p.interactingWithBuildingId = playerMap[p.id].interactingWithBuildingId;
	p.interactingWIthBuildingFace = playerMap[p.id].interactingWIthBuildingFace;
	
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

void DynamicWorld::addStaticRampObject(struct staticObject obj)
{
	staticRampObjects.push_back(obj);
}

int DynamicWorld::getNumStaticObjects() 
{
	return staticObjects.size();
}

int DynamicWorld::getNumStaticRampObjects()
{
	return staticRampObjects.size();
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
		if (e->aabb.collidesWith(staticObjects[i].aabb))
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
		if (e->aabb.collidesWithSide(staticObjects[i].aabb))
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
	computeAABB(p);

	while(checkCollisionsWithAllNonTraps(p) != -1)
	{
		p->position.x += 10;
		computeAABB(p);
	}

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
	noneMoveEvent(playerId, dir);
	/*
	switch (p->currState) {
		case None:
			Physics::noneMoveEvent(pkt);
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
	*/

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
	computeAABB(p);
	int buildingId = checkCollisionsWithAllNonTraps(p);
	p->position = oldPos;
	computeAABB(p);

	if (buildingId != -1) {
	//	int oldOnTopOfBuildingId = p->onTopOfBuildingId;
	//	p->onTopOfBuildingId = -1;
	//	// TODO: check guy is facing wall too, at rest

	//	if (!p->feetPlanted /*&& !(m_physics->atRest()) */ && pkt->direction == FORWARD) {
	//		cout << "doing extra logic!" << endl;
	//		if (Physics::handleNearTop(proposedNewPos, staticObjects[buildingId])) {
	//			//TODO: startHoldingEdge(p);
	//			return;
	//		}

	//		
	//		float angle = Physics::handleAngleIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
	//		//if (abs(90.0f-angle) < 22.5f /*&& m_physics->m_velocity.y >= m_miniJumpYVelocityThreshold*/) {
	//			newPos = oldPos;
	//			//cout << "starting the climb with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << buildingId << endl;
	//			//m_cam->m_cameraLookAtWallJump = m_cam->m_cameraCenter - m_cam->m_camZ;
	//			startClimbing(p, buildingId);
	//			return;
	//		//}
	//		/*
	//		else {
	//			//if (oldOnTopOfBuildingId != buildingId) {
	//			//if (m_onTopOfBuildingId != -1) {
	//				//cout << "starting the wallrunning with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << buildingId << endl;
	//			// 0,1 = x, -1 = y, 4,5 = z
	//			int newDirection = Physics::handleReflectionIntersection(oldPos, proposedNewPos, staticObjects[buildingId]);
	//			startWallRunning(p, newDirection, toAdd, angle);
	//			return;
	//				
	//			//}
	//
	//		}*/
	//	}

	
		//else {
			toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
			p->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			p->velocity = glm::vec3(0.0f, p->velocity.y, 0.0f);
		//}
	}

	p->velocityDiff += toAdd;

}

void DynamicWorld::climbingMoveEvent(int playerId, Direction dir) {

	struct playerObject *p = &playerMap[playerId];

	// TODO: enable movement?
	switch (dir)
	{

		case BACKWARD:
			cout << "decided to fall back down from climbing" << endl;
			p->currminiState = innerStates::Ending;
			//TODO: maybe more, set Holders
			return;
			break;

		default:
			break;
	}
	
}

void DynamicWorld::holdingEdgeMoveEvent(int playerId, Direction dir) {
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
			startPullingUp(p);
			return;
			break;

		case BACKWARD:
			cout << "decided to fall back down" << endl;
			p->currminiState = -1;
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
	bool fellOffSide = staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb);
	p->position = oldPos;
	computeAABB(p);

	// TODO: let you fall off?
	if (fellOffSide) {
		toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
		p->velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
		p->velocity = glm::vec3(0.0f, p->velocity.y, 0.0f);
	}
	p->velocityDiff += toAdd;

}

void DynamicWorld::wallRunningMoveEvent(int playerId, Direction dir) {

	cout << "why handling wallrunningmoveevent wtf??" << endl;
	return;
}

void DynamicWorld::startClimbing(struct playerObject *e, int buildingId) {
	cout << "started climbing" << endl;
	// CLIMBING
	float CLinitialX = e->cameraObject.xRotated;
	float CLinitialY = e->cameraObject.yRotated;

	float CLnumFrames = 100.0f;
	//ConfigSettings::getConfig()->getValue("CLnumFrames", CLnumFrames);

	// start
	float CLStartlookX = 0.0f-CLinitialX;
	float CLStartlookY = 70.0f-CLinitialY;
	
	float CLStartfraction = 1.0f;

	float CLStartlookXIncrement = 0.0f;
	float CLStartlookYIncrement = CLStartlookY*CLStartfraction/CLnumFrames;
	glm::vec3 CLStartlookUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);

	float climbFactor = 3.0f;
	//ConfigSettings::getConfig()->getValue("climbFactor", climbFactor);

	glm::vec3 CLStartvelocityDiff = glm::vec3(0.0f, climbFactor, 0.0f);

	int CLStartCounter = 0;

	// Mid
	glm::vec3 CLMidvelocityDiff = glm::vec3(0.0f, climbFactor, 0.0f);

	// End
	float CLEndfraction = 0.1f;
	float CLEndlookYIncrement = -70.0f*CLEndfraction / CLnumFrames;

	int CLEndCounter = 0;

	statesInfo[e->id].initialX = CLinitialX;
	statesInfo[e->id].initialY = CLinitialY;
	statesInfo[e->id].numFrames = CLnumFrames;
	statesInfo[e->id].innerState = innerStates::Starting;
	
	statesInfo[e->id].Start.lookX = CLStartlookX;
	statesInfo[e->id].Start.lookY = CLStartlookY;
	statesInfo[e->id].Start.fraction = CLStartfraction;
	statesInfo[e->id].Start.lookXIncrement = CLStartlookXIncrement;
	statesInfo[e->id].Start.lookYIncrement = CLStartlookYIncrement;
	statesInfo[e->id].Start.velocityDiff = CLStartvelocityDiff;
	statesInfo[e->id].Start.counter = CLStartCounter;

	statesInfo[e->id].Mid.velocityDiff = CLMidvelocityDiff;

	statesInfo[e->id].End.fraction = CLEndfraction;
	statesInfo[e->id].End.lookYIncrement = CLEndlookYIncrement;
	statesInfo[e->id].End.counter = CLEndCounter;

	e->interactingWithBuildingId = buildingId;
	e->currState = PhysicsStates::Climbing;

}

void DynamicWorld::startHoldingEdge(struct playerObject *e, int buildingId) {
	cout << "started holding edge" << endl;
	int direction = staticObjects[e->interactingWithBuildingId].aabb.reflectionIntersection(e->position, e->position + e->velocity + e->velocityDiff);

	// TODO: imp if necessary
	/*
	switch (direction) {
		case 0:
			//set HEstartlookX etc
			
			break;
		case 1:
			break;
		case 4:
			break;
		case 5:
			break;
	}
	*/

	float HEinitialX = e->cameraObject.xRotated;
	float HEinitialY = e->cameraObject.yRotated;
	glm::vec3 HEHoldervelocityDiff = e->velocityDiff;

	float HEnumFrames = 100.0f;
	//ConfigSettings::getConfig()->getValue("HEnumFrames", HEnumFrames);

	// start
	float HEStartlookY = 70.0f - HEinitialY;

	float HEStartfraction = 1.0f;

	float HEStartlookYIncrement = HEStartlookY*HEStartfraction / HEnumFrames;
	glm::vec3 HEStartlookUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);

	int HEStartCounter = 0;

	// End
	float HEEndlookX = 180.0f;// calculate
	float HEEndlookY = -70.0f;

	float HEEndfraction = 0.1f;

	float HEEndlookXIncrement = HEEndlookX*HEEndfraction / HEnumFrames;
	float HEEndlookYIncrement = HEEndlookY*HEEndfraction / HEnumFrames;

	float bounceFactor = 5.0f;
	//ConfigSettings::getConfig()->getValue("bounceFactor", bounceFactor);


	glm::vec3 HEEndVeloDiff = glm::vec3(-1.0f*HEHoldervelocityDiff.x, bounceFactor, -1.0f*HEHoldervelocityDiff.z);

	int HEEndCounter = 0;

	statesInfo[e->id].initialX = HEinitialX;
	statesInfo[e->id].initialY = HEinitialY;
	statesInfo[e->id].numFrames = HEnumFrames;
	statesInfo[e->id].innerState = innerStates::Starting;

	statesInfo[e->id].Holder.velocityDiff = HEHoldervelocityDiff;

	statesInfo[e->id].Start.lookY = HEStartlookY;
	statesInfo[e->id].Start.fraction = HEStartfraction;
	statesInfo[e->id].Start.lookYIncrement = HEStartlookYIncrement;
	statesInfo[e->id].Start.counter = HEStartCounter;

	statesInfo[e->id].End.lookX = HEEndlookX;
	statesInfo[e->id].End.lookY = HEEndlookY;
	statesInfo[e->id].End.fraction = HEEndfraction;
	statesInfo[e->id].End.lookXIncrement = HEEndlookXIncrement;
	statesInfo[e->id].End.lookYIncrement = HEEndlookYIncrement;
	statesInfo[e->id].End.counter = HEEndCounter;

	e->interactingWithBuildingId = buildingId;
	e->currState = PhysicsStates::HoldingEdge;

}

void DynamicWorld::startPullingUp(struct playerObject *e) {
	cout << "start pulling up\n";
	float PUinitialX = e->cameraObject.xRotated;
	float PUinitialY = e->cameraObject.yRotated;

	float PUnumFrames = 30.0f;
	//ConfigSettings::getConfig()->getValue("PUnumFrames", PUnumFrames);

	// start
	float PUStartlookX = 45.0f - PUinitialX;
	float PUStartlookY = -20.0f - PUinitialY;

	float PUStartfraction = 0.7f;

	float PUStartlookXIncrement = PUStartlookX*PUStartfraction / PUnumFrames;
	float PUStartlookYIncrement = PUStartlookY*PUStartfraction / PUnumFrames;

	float pullingUpFactor = 3.0f;
	//ConfigSettings::getConfig()->getValue("pullingUpFactor", pullingUpFactor);
	glm::vec3 PUStartvelocityDiff = glm::vec3(0.0f, pullingUpFactor, 0.0f);

	int PUStartCounter = 0;

	// end
	float PUEndlookX = -1.0f*PUEndlookX;
	float PUEndlookY = -1.0f*PUEndlookY;

	float PUEndfraction = 0.3f;

	float PUEndlookXIncrement = PUEndlookX*PUEndfraction / PUnumFrames;
	float PUEndlookYIncrement = PUEndlookY*PUEndfraction / PUnumFrames;

	glm::vec3 PUEndvelocityDiff = glm::vec3(0.0f, pullingUpFactor, 0.0f);

	int PUEndCounter = 0;

	return;
}

void DynamicWorld::startWallRunning(struct playerObject *e, int newDirection, glm::vec3 toAdd, float angle) {

	// WallRunning
	cout << "starting wall running" << endl;
	float WRinitialX = e->cameraObject.xRotated;
	float WRinitialY = e->cameraObject.yRotated;
	glm::vec3 WRinitialUp = e->cameraObject.cameraUp;

	float WRnumFrames = 60.0f;
	//ConfigSettings::getConfig()->getValue("WRnumFrames", WRnumFrames);

	float WRStartfraction = 0.7f;
	//ConfigSettings::getConfig()->getValue("WRStartfraction", WRStartfraction);

	float WREndfraction = 0.3f;
	//ConfigSettings::getConfig()->getValue("WREndfraction", WREndfraction);

	if (newDirection == 0 || newDirection == 1) {
		statesInfo[e->id].Holder.camZ = glm::vec3(e->cameraObject.camZ.x*-1.0f, e->cameraObject.camZ.y, e->cameraObject.camZ.z);
		toAdd.z *= -1.0f;
		statesInfo[e->id].Holder.velocityDiff = glm::vec3(e->velocityDiff.x*-1.0f, e->velocityDiff.y, e->velocityDiff.z);
		statesInfo[e->id].Holder.velocityDiff += toAdd;
		// TODO: change 0.8 to use config file,

		statesInfo[e->id].Start.camZ = glm::vec3(0.0f, 0.0f, 1.0f);
		statesInfo[e->id].Start.camX = glm::vec3(0.0f, -1.0f, 0.0f);
		statesInfo[e->id].Start.camUp = glm::vec3(-0.8f, 0.8f, 0.0f);

	}
	if (newDirection == 4 || newDirection == 5) {
		statesInfo[e->id].Holder.camZ = glm::vec3(e->cameraObject.camZ.x, e->cameraObject.camZ.y, e->cameraObject.camZ.z*-1.0f);
		toAdd.z *= -1.0f;
		statesInfo[e->id].Holder.velocityDiff = glm::vec3(e->velocityDiff.x, e->velocityDiff.y, e->velocityDiff.z*-1.0f);
		statesInfo[e->id].Holder.velocityDiff += toAdd;

		statesInfo[e->id].Start.camZ = glm::vec3(1.0f, 0.0f, 0.0f);
		statesInfo[e->id].Start.camX = glm::vec3(0.0f, 1.0f, 0.0f);
		statesInfo[e->id].Start.camUp = glm::vec3(0.0f, 0.8f, -0.8f);
	}

	if (newDirection % 2 == 1) {
		statesInfo[e->id].Start.camX *= -1.0f;
		statesInfo[e->id].Start.camUp.x *= -1.0f;
		statesInfo[e->id].Start.camUp.z *= -1.0f;
	}

	statesInfo[e->id].Start.camUpIncrement = glm::vec3(statesInfo[e->id].Start.camUp - statesInfo[e->id].initialUp)*statesInfo[e->id].Start.fraction / statesInfo[e->id].numFrames;
	statesInfo[e->id].End.camUpIncrement = -1.0f*glm::vec3(statesInfo[e->id].Start.camUp - statesInfo[e->id].initialUp)*statesInfo[e->id].End.fraction / statesInfo[e->id].numFrames;

	/*	1 +,-
	2 -,+
	3 -,+
	4 +,-*/
	/*cout << "from: " << glm::to_string(m_cam->m_cameraUp) << endl;
	cout << "get to: " << glm::to_string(m_cam->m_cameraUpWallRun) << endl;
	cout << "with: " << glm::to_string(m_physics->m_wallRunLookUpReadjustIncrement) << endl;*/


	if (angle < 90.0f) {

		statesInfo[e->id].Start.lookXIncrement = -1.0f*angle*statesInfo[e->id].Start.fraction / statesInfo[e->id].numFrames;
		statesInfo[e->id].End.lookXIncrement = -1.0f*angle*statesInfo[e->id].End.fraction / statesInfo[e->id].numFrames;

		if (newDirection == 0 || newDirection == 5) {
			statesInfo[e->id].Holder.xRotated = e->cameraObject.xRotated - (2.0f*angle);
		}
		if (newDirection == 1 || newDirection == 4) {
			statesInfo[e->id].Holder.xRotated = e->cameraObject.xRotated - (2.0f*(180.0f - angle));
			statesInfo[e->id].Start.lookXIncrement *= -1.0f;
			statesInfo[e->id].End.lookXIncrement *= -1.0f;
		}
	}
	if (angle > 90.0f) {

		statesInfo[e->id].Start.lookXIncrement = (abs(180.0f - angle))*statesInfo[e->id].Start.fraction / statesInfo[e->id].numFrames;
		statesInfo[e->id].End.lookXIncrement = (abs(180.0f - angle))*statesInfo[e->id].End.fraction / statesInfo[e->id].numFrames;

		if (newDirection == 0 || newDirection == 5) {
			statesInfo[e->id].Holder.xRotated = e->cameraObject.xRotated + (2.0f*(180.0f - angle));
		}
		if (newDirection == 1 || newDirection == 4) {
			statesInfo[e->id].Holder.xRotated = e->cameraObject.xRotated + (2.0f*angle);
			statesInfo[e->id].Start.lookXIncrement *= -1.0f;
			statesInfo[e->id].End.lookXIncrement *= -1.0f;
		}
		statesInfo[e->id].Start.camZ *= -1.0f;
		statesInfo[e->id].Start.camX *= -1.0f;

	}

	e->feetPlanted = true;
	e->currState = PhysicsStates::WallRunning;
}


// TODO: check for feet?
void DynamicWorld::applyGravity()
{
	//cout << "checking for gravity\n";
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject &p = it->second;
		if (p.currState != PhysicsStates::Climbing && p.currState != PhysicsStates::HoldingEdge &&
			p.currState != PhysicsStates::PullingUp && p.currState != PhysicsStates::WallRunning) {
			
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
			}
		}

		p.velocity += p.velocityDiff;
		p.position += p.velocity;
		p.velocity -= p.velocityDiff;
		p.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);

		float playerHeight = 4.0f;
		//ConfigSettings::getConfig()->getValue("playerHeight", playerHeight);

		p.cameraObject.cameraCenter = p.position + playerHeight;
		p.cameraObject.cameraLookAt = p.cameraObject.cameraCenter + p.cameraObject.camZ;
		computeAABB(&p);
	}

}

void DynamicWorld::applyPhysics()
{
	for (map<int, struct playerObject>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
	{
		struct playerObject *p = &it->second;
		switch (p->currState) {
			case PhysicsStates::Climbing:
				applyClimbing(p);
				break;
			case PhysicsStates::HoldingEdge:
				applyHoldingEdge(p);
				break;
			case PhysicsStates::WallRunning:
				applyWallRunning(p);
				break;
		}
		checkForStateChanges(p);
	}
}

void DynamicWorld::applyClimbing(struct playerObject *p) {

	switch (p->currminiState) {
		case innerStates::Starting:
			handleXRotation(p, statesInfo[p->currState].Start.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].Start.lookYIncrement);
			p->velocityDiff = statesInfo[p->currState].Start.velocityDiff;
			p->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			statesInfo[p->currState].Start.counter++;
			if (statesInfo[p->currState].Start.counter >= statesInfo[p->currState].Start.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Mid;
			}
			break;
		case innerStates::Mid:
			handleXRotation(p, statesInfo[p->currState].Mid.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].Mid.lookYIncrement);
			p->velocityDiff = statesInfo[p->currState].Mid.velocityDiff;
			p->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			break;
		case innerStates::Ending:
			p->interactingWithBuildingId = -1;
			handleXRotation(p, statesInfo[p->currState].End.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].End.lookYIncrement);
			p->cameraObject.cameraUp += statesInfo[p->currState].End.camUpIncrement;
			p->velocityDiff = statesInfo[p->currState].End.velocityDiff;
			statesInfo[p->currState].End.counter++;
			if (statesInfo[p->currState].End.counter >= statesInfo[p->currState].End.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Off;
				cout << "ended state: " << p->currState << endl;
				p->currState = PhysicsStates::None;
			}
			break;
	}

}

void DynamicWorld::applyHoldingEdge(struct playerObject *p) {

	switch (p->currminiState) {
		case innerStates::Starting:
			handleXRotation(p, statesInfo[p->currState].Start.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].End.lookYIncrement);
			statesInfo[p->currState].Start.counter++;
			if (statesInfo[p->currState].Start.counter >= statesInfo[p->currState].Start.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Mid;
			}
			break;
		case innerStates::Ending:
			p->interactingWithBuildingId = -1;
			handleXRotation(p, statesInfo[p->currState].End.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].End.lookYIncrement);
			p->velocityDiff = statesInfo[p->currState].End.velocityDiff;
			statesInfo[p->currState].End.counter++;
			if (statesInfo[p->currState].End.counter >= statesInfo[p->currState].End.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Off;
				cout << "ended state: " << p->currState << endl;
				p->currState = PhysicsStates::None;
			}
			break;
	}

}

void DynamicWorld::applyPullingUp(struct playerObject *p) {

	switch (p->currminiState) {
		case innerStates::Starting:
			handleXRotation(p, statesInfo[p->currState].Start.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].Start.lookYIncrement);
			p->velocityDiff = statesInfo[p->currState].Start.velocityDiff;
			statesInfo[p->currState].Start.counter++;
			if (statesInfo[p->currState].Start.counter >= statesInfo[p->currState].Start.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Ending;
			}
			break;
		case innerStates::Ending:
			p->interactingWithBuildingId = -1;
			handleXRotation(p, statesInfo[p->currState].End.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].End.lookYIncrement);
			p->velocityDiff = statesInfo[p->currState].End.velocityDiff;
			statesInfo[p->currState].End.counter++;
			if (statesInfo[p->currState].End.counter >= statesInfo[p->currState].End.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Off;
				cout << "ended state: " << p->currState << endl;
				p->currState = PhysicsStates::None;
			}
			break;
	}

}

void DynamicWorld::applyWallRunning(struct playerObject *p) {
	
	switch (p->currminiState) {
		case innerStates::Starting:
			if (triedToRun[p->id]) {
				handleXRotation(p, statesInfo[p->currState].Start.lookXIncrement);
				handleYRotation(p, statesInfo[p->currState].Start.lookYIncrement);
				p->cameraObject.cameraUp += statesInfo[p->currState].Start.camUpIncrement;
				p->velocityDiff = statesInfo[p->currState].Start.velocityDiff;
				statesInfo[p->currState].Start.counter++;
				if (statesInfo[p->currState].Start.counter >= statesInfo[p->currState].Start.fraction*statesInfo[p->currState].numFrames) {
					p->currminiState = innerStates::Mid;
				}
			}
			else {
				p->currminiState = innerStates::Ending;
			}
			break;
		case innerStates::Mid:
			if (triedToRun[p->id]) {
				p->velocityDiff = statesInfo[p->currState].Mid.velocityDiff;
			}
			else {
				p->currminiState = innerStates::Ending;
			}
			break;
		case innerStates::Ending:
			p->interactingWithBuildingId = -1;
			handleXRotation(p, statesInfo[p->currState].End.lookXIncrement);
			handleYRotation(p, statesInfo[p->currState].End.lookYIncrement);
			p->cameraObject.cameraUp += statesInfo[p->currState].End.camUpIncrement;
			p->velocityDiff = statesInfo[p->currState].End.velocityDiff;
			statesInfo[p->currState].End.counter++;
			if (statesInfo[p->currState].End.counter >= statesInfo[p->currState].End.fraction*statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Off;
				cout << "ended state: " << p->currState << endl;
				p->currState = PhysicsStates::None;
			}
			break;
	}

}

void DynamicWorld::checkForStateChanges(struct playerObject *p) {
	switch (p->currState) {
		case PhysicsStates::Climbing:
			if (staticObjects[p->interactingWithBuildingId].aabb.nearTop(p->position)) {
				startHoldingEdge(p, p->interactingWithBuildingId);
				return;
			}
			//if (/* time elapsed > 3 seconds*/) {
			//	p->currminiState = innerStates::Ending;
			//	return;
			//}
			break;
		case PhysicsStates::HoldingEdge:
			if (staticObjects[p->interactingWithBuildingId].aabb.clearedTop(p->aabb)) {
				startPullingUp(p);
				return;
			}
			else if (staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb)) {
				p->currminiState = innerStates::Ending;
				return;
			}
		case PhysicsStates::WallRunning:
			if (staticObjects[p->interactingWithBuildingId].aabb.fellOffSide(p->aabb)) {
				p->currminiState = innerStates::Ending;
				return;
			}
	}

}

void DynamicWorld::processJumpEvent(int playerId)
{
	struct playerObject *p = &playerMap[playerId];
	//noneJumpEvent(pkt);
	
	switch (p->currState) {
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

	float bounceFactor = 5.0f;
	//ConfigSettings::getConfig()->getValue("bounceFactor", bounceFactor);
	statesInfo[p->id].End.velocityDiff *= bounceFactor;
	p->currminiState = innerStates::Ending;
}

void DynamicWorld::holdingEdgeJumpEvent(int playerId) {
	cout << "decided to jump off while holding edge" << endl;
	struct playerObject *p = &playerMap[playerId];
	p->currminiState = innerStates::Ending;
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
	p->currminiState = innerStates::Ending;
	//TODO: maybe more, set Holders

}


void DynamicWorld::handleXRotation(struct playerObject *e, float angle) {
	glm::vec3 tmp_camZ = glm::vec3(e->cameraObject.camZ.x, 0.0f, e->cameraObject.camZ.z);

	tmp_camZ = glm::rotateY(tmp_camZ, angle);
	e->cameraObject.camX = glm::rotateY(e->cameraObject.camX, angle);

	e->cameraObject.camZ = glm::vec3(tmp_camZ.x, e->cameraObject.camZ.y, tmp_camZ.z);
	e->cameraObject.cameraLookAt = e->cameraObject.cameraCenter + e->cameraObject.camZ;

	e->cameraObject.xRotated += angle;
}

void DynamicWorld::handleYRotation(struct playerObject *e, float angle) {
	// TODO modify upvector too for confuse ray
	if (!(e->cameraObject.yRotated > 80.0f && angle > 0) && !(e->cameraObject.yRotated < -80.0f && angle < 0)) {

		//m_camZ.y+=magnitude*m_yRotationAngle; // both this and the two lines below seem okay
		e->cameraObject.camZ = glm::rotate(e->cameraObject.camZ, angle, e->cameraObject.camX);
		e->cameraObject.yRotated += angle;
		//cout << "ROTATING CAM: " << m_yRotated << ", " << glm::radians(80.0f) << endl;
	}
	//cout << "mcamX: " << glm::to_string(m_camX) << endl;
	//cout << "before: " << glm::to_string(m_cameraLookAt) << endl;
	e->cameraObject.cameraLookAt = e->cameraObject.cameraCenter + e->cameraObject.camZ;

	//cout << "after: " << glm::to_string(m_cameraLookAt) << endl << endl;
}

void DynamicWorld::calculateAxis(struct playerObject *e) {
	glm::vec3 ZCameraDiff = e->cameraObject.cameraLookAt - e->cameraObject.cameraCenter;
	//ZCameraDiff.y = 0.0f;

	// not sure if need to normalize
	e->cameraObject.camZ = glm::normalize(ZCameraDiff);

	float oldmcamXY = e->cameraObject.camX.y;
	// or is it radians
	e->cameraObject.camX = glm::rotateY(e->cameraObject.camZ, -90.0f);
	e->cameraObject.camX = glm::vec3(e->cameraObject.camX.x, oldmcamXY, e->cameraObject.camX.z);

}

void DynamicWorld::checkPlayersCollideWithTrap()
{
	for (map<int, struct playerObject>::iterator pit = playerMap.begin(); pit != playerMap.end(); pit++)
	{
		struct playerObject *p = &pit->second;

		for (map<int, struct trapObject>::iterator it = trapMap.begin(); it != trapMap.end(); it++)
		{
			if (p->id != it->second.ownerId && it->second.aabb.collidesWith(p->aabb) && it->second.eventCode == 0)
			{
				printf("Collision: player %d with trap id %d\n", p->id, it->second.id);
				playerLock[p->id] = true;
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
				default:
					break;
				}

				it->second.eventCode = EVENT_REMOVE_TRAP;

				break;
			}
		}
	}
}
