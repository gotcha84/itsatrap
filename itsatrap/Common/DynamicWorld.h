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

struct infoMsg {
	int destination;
	string msg;
};

/*
 * class DynamicWorld
 *
 * This class contains information about the world.
 *
 */
class COMMON_API DynamicWorld {

private:
	// Variables
	int									currentId;
	vector<struct staticObject>			staticObjects;
	vector<struct staticResourceObject>	staticResourceObjects;	// ANDRE: Resource Tower - Add BB
	vector<glm::vec3>					team1RespawnPoints, team2RespawnPoints;
	int									team1CurrRP, team2CurrRP;
	map<int, struct trapObject *>		portalMap;
	bool								playerLock[MAX_PLAYERS];
	map<int, AABB>						aabbOffsets;
	
	bool checkCollision(struct aabb a, struct aabb b);
	int checkCollisionsWithAllNonTraps(struct playerObject *e);
	int checkSideCollisionsWithAllBuildings(glm::vec3 from, glm::vec3 goTo, struct playerObject *e);
	int checkTopCollisionsWithAllBuildings(glm::vec3 from, glm::vec3 goTo, struct playerObject *e);
	int checkCollisionsWithAllRampsEntrance(struct playerObject *e);
	//int checkOnTopOfWithAllRamps(struct playerObject *e);
	int checkCollisionsWithAllRampsInside(struct playerObject *e);
	void addNewPlayer(struct playerObject p);
	void respawnPlayer(struct playerObject *p);
	void computeAABB(struct playerObject *p);
	void computeAABB(struct trapObject *t);
	void addInfoMessage(int destination, string msg);

	//float handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, struct aabb other, int buildingId);

public:
	// Variables
	map<int, struct trapObject>		trapMap;
	map<int, struct playerObject>	playerMap;
	vector<struct infoMsg>			infoMsgQueue;

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
	void addStaticResourceObject(struct staticResourceObject);
	int getNumStaticObjects();
	int getNumStaticResourceObjects();
	AABB getStaticObjectBB(int buildingId);
	AABB getStaticResourceBB(int resourceId);

	void playerDamage(struct playerObject *attacker, struct playerObject *target, int damage);

	void resetWorldInfo();
	void applyCollisions();
	void applyGravity();
	void applyPhysics();
	void applyAdjustments();
	void manuallyUncollide();
	void applyTrapGravity();

	void applyMoveEvents();
	void noneMoveEvent(int playerId);
	void climbingMoveEvent(int playerId);
	void pullingUpMoveEvent(int playerId);
	void holdingEdgeMoveEvent(int playerId);
	void wallRunningMoveEvent(int playerId);

	void noneJumpEvent(int playerId);
	void climbingJumpEvent(int playerId);
	void pullingUpJumpEvent(int playerId);
	void holdingEdgeJumpEvent(int playerId);
	void wallRunningJumpEvent(int playerId);

	void checkPlayersCollideWithTrap();

	void checkForStateChanges(struct playerObject *e);

	void addAABBInfo(int type, AABB aabb);
	void handleKnifeEvent(int knifer);
};

#endif
