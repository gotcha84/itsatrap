#ifndef DYNAMICWORLD_H
#define DYNAMICWORLD_H

#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "NetworkObjects.h"
#include "Packet.h"

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
    map<int, struct playerObject>	playerMap;
	vector<struct staticObject>		staticObjects;

	// Functions
	bool checkCollision(struct aabb a, struct aabb b);
public:
	// Functions
	__declspec(dllexport) DynamicWorld();
	__declspec(dllexport) DynamicWorld(struct packet *packet);
	__declspec(dllexport) int serialize(char **ptr);
	__declspec(dllexport) void printWorld();

	__declspec(dllexport) void updatePlayer(struct playerObject e);
	__declspec(dllexport) int getNumPlayers();
	__declspec(dllexport) vector<struct playerObject> getAllPlayers();

	__declspec(dllexport) void addStaticObject(struct staticObject);
	__declspec(dllexport) int getNumStaticObjects();
};

#endif
