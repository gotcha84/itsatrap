#ifndef DYNAMICWORLD_H
#define DYNAMICWORLD_H

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
    vector<struct playerObject>	playerObjects; // This vector should contain information about objects
	                                         // that are currently living in the world
	
public:
	// Functions
	// NOTE: __declspec(dllexport) is used to export this function
	
	__declspec(dllexport) DynamicWorld();
	__declspec(dllexport) DynamicWorld(struct packet *packet);
	__declspec(dllexport) int getNumPlayers();
	__declspec(dllexport) int serialize(char **ptr);
	__declspec(dllexport) void printWorld();
	__declspec(dllexport) void updatePlayer(struct playerObject e);
	__declspec(dllexport) struct playerObject getObjectAt(int i);
};

#endif
