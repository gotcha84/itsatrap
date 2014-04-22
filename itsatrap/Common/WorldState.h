#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include "StateEntry.h"
#include "Packet.h"

using namespace std;

/*
 * class WorldState
 *
 * This class contains information about the world.
 *
 */
class WorldState {

private:
	// Variables
    vector<struct stateEntry>		entries; // This vector should contain information about objects
	                                         // that are currently living in the world
	
public:

	// Functions
	// NOTE: __declspec(dllexport) is used to export this function
	
	__declspec(dllexport) WorldState();
	__declspec(dllexport) WorldState(struct packet *packet);
	__declspec(dllexport) int getSize();
	__declspec(dllexport) int serialize(char **ptr);
	__declspec(dllexport) void printWorld();
	__declspec(dllexport) void updateEntry(struct stateEntry e);
	__declspec(dllexport) struct stateEntry getEntryAt(int i);
};

#endif
