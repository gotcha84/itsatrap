#ifndef STATICWORLD_H
#define STATICWORLD_H

#include <vector>
#include <stdio.h>
#include <iostream>

#include "NetworkObjects.h"

using namespace std;

/*
 * class StaticWorld
 *
 * This class contains information about the world.
 *
 */
class StaticWorld {

private:
	// Variables
    vector<struct staticObject>	objects;	 // This vector should contain information about objects
	                                         // that are currently living in the world

public:
	__declspec(dllexport) int getSize();
	__declspec(dllexport) void printWorld();
	__declspec(dllexport) void addObject(struct staticObject e);
	__declspec(dllexport) struct staticObject getObjectAt(int i);
};

#endif
