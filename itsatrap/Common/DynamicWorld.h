#ifndef DYNAMICWORLD_H
#define DYNAMICWORLD_H

#include <../glm/glm/glm.hpp>
#include <../glm/glm/ext.hpp>

#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "NetworkObjects.h"
#include "Packet.h"
#include "ConfigSettings.h"

using namespace std;

/*
 * class DynamicWorld
 *
 * This class contains information about the world.
 *
 */
class DynamicWorld {

private:
	// Variables
	int								currentId;
	vector<struct staticObject>		staticObjects;

	// Functions
	bool checkCollision(struct aabb a, struct aabb b);
	bool checkCollisionWithAllNonTraps(struct playerObject e);
	void addNewPlayer(struct playerObject p);

public:
	// Variables
	map<int, struct trapObject>		trapMap;
	map<int, struct playerObject>	playerMap;

	// Functions
	__declspec(dllexport) DynamicWorld();
	__declspec(dllexport) DynamicWorld(struct packet *packet);
	__declspec(dllexport) int serialize(char **ptr);
	__declspec(dllexport) void printWorld();

	__declspec(dllexport) void updatePlayer(struct playerObject e);
	__declspec(dllexport) int getNumPlayers();
	__declspec(dllexport) vector<struct playerObject> getAllPlayers();
	__declspec(dllexport) void updatePlayerBuffs(int timeDiff);

	__declspec(dllexport) void addTrap(struct trapObject t);

	__declspec(dllexport) void addStaticObject(struct staticObject);
	__declspec(dllexport) int getNumStaticObjects();

	
};

#endif
