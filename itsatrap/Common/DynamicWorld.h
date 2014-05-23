#ifndef DYNAMICWORLD_H
#define DYNAMICWORLD_H

#include <../glm/glm/glm.hpp>
#include <../glm/glm/ext.hpp>

#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "NetworkObjects.h"
#include "NetworkConfig.h"
#include "Packet.h"
#include "ConfigSettings.h"
#include "Physics.h"
#include "World.h"

using namespace std;

/*
 * class DynamicWorld
 *
 * This class contains information about the world.
 *
 */
class COMMON_API DynamicWorld {

private:
	// Variables
	int								currentId;
	vector<struct staticObject>		staticObjects;
	bool							playerLock[MAX_PLAYERS];
	vector<struct stateInfo>		stateInfos;
	vector<int>						wallJumpingBuildingIds;						



	// 
	void initStateInfo();
	bool checkCollision(struct aabb a, struct aabb b);
	int checkCollisionsWithAllNonTraps(struct playerObject *e);
	int checkSideCollisionsWithAllNonTraps(struct playerObject *e);
	void addNewPlayer(struct playerObject p);
	void respawnPlayer(struct playerObject *p);
	void computeAABB(struct playerObject *p);

	

	//float handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, struct aabb other, int buildingId);

public:
	// Variables
	map<int, struct trapObject>		trapMap;
	map<int, struct playerObject>	playerMap;

	// Functions
	DynamicWorld();
	DynamicWorld(struct packet *packet);
	int serialize(char *ptr);
	void printWorld();

	void updatePlayer(struct playerObject e);
	int getNumPlayers();
	vector<struct playerObject> getAllPlayers();
	void updatePlayerBuffs(int timeDiff);
	void processMoveEvent(struct moveEventPacket *pkt);
	void processJumpEvent(struct jumpEventPacket *pkt);
	void processLookEvent(struct lookEventPacket *pkt);

	void addTrap(struct trapObject t);

	void addStaticObject(struct staticObject);
	int getNumStaticObjects();

	void playerDamage(struct playerObject *attacker, struct playerObject *target, int damage);

	void applyGravity();
	void applyPhysics();

	/*void noneMoveEvent(struct moveEventPacket *pkt);
	void climbingMoveEvent(struct moveEventPacket *pkt);
	void pullingUpMoveEvent(struct moveEventPacket *pkt);
	void holdingEdgeMoveEvent(struct moveEventPacket *pkt);
	void wallRunningMoveEvent(struct moveEventPacket *pkt);*/

	void startClimbing(struct playerObject *e, int buildingId);
	void startWallRunning(struct playerObject *e, int newDirection, glm::vec3 toAdd, float angle);
	void noneMoveEvent(struct moveEventPacket *pkt);
	void climbingMoveEvent(struct moveEventPacket *pkt);
	void pullingUpMoveEvent(struct moveEventPacket *pkt);
	void holdingEdgeMoveEvent(struct moveEventPacket *pkt);
	void wallRunningMoveEvent(struct moveEventPacket *pkt);

	void noneJumpEvent(struct jumpEventPacket *pkt);
	void climbingJumpEvent(struct jumpEventPacket *pkt);
	void pullingUpJumpEvent(struct jumpEventPacket *pkt);
	void holdingEdgeJumpEvent(struct jumpEventPacket *pkt);
	void wallRunningJumpEvent(struct jumpEventPacket *pkt);

	void checkPlayersCollideWithTrap();
};

#endif
