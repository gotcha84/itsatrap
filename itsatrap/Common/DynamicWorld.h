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
#include "StateLogic.h"
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
	vector<struct staticObject>		staticRampObjects;
	bool							playerLock[MAX_PLAYERS];
	
	// 
	bool checkCollision(struct aabb a, struct aabb b);
	int checkCollisionsWithAllNonTraps(struct playerObject *e);
	int checkSideCollisionsWithAllBuildings(struct playerObject *e);
	int checkCollisionsWithAllRamps(struct playerObject *e);
	int checkOnTopOfWithAllRamps(struct playerObject *e);
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

	void cleanStateInfo(int id);

	void updatePlayer(struct playerObject e);
	int getNumPlayers();
	vector<struct playerObject> getAllPlayers();
	void updateTimings(int timeDiff);
	void processMoveEvent(int playerId, Direction dir);
	void processJumpEvent(int playerId);
	void processLookEvent(int playerId, struct cameraObject *cam);

	void addTrap(struct trapObject t);

	void addStaticObject(struct staticObject);
	void addStaticRampObject(struct staticObject);
	int getNumStaticObjects();
	int getNumStaticRampObjects();

	void playerDamage(struct playerObject *attacker, struct playerObject *target, int damage);

	void resetWorldInfo();
	void applyGravity();
	void applyPhysics();
	void applyAdjustments();

	void noneMoveEvent(int playerId, Direction dir);
	void climbingMoveEvent(int playerId, Direction dir);
	void pullingUpMoveEvent(int playerId, Direction dir);
	void holdingEdgeMoveEvent(int playerId, Direction dir);
	void wallRunningMoveEvent(int playerId, Direction dir);

	void noneJumpEvent(int playerId);
	void climbingJumpEvent(int playerId);
	void pullingUpJumpEvent(int playerId);
	void holdingEdgeJumpEvent(int playerId);
	void wallRunningJumpEvent(int playerId);

	void checkPlayersCollideWithTrap();

	void checkForStateChanges(struct playerObject *e);

};

#endif
