#pragma once
#ifndef STATELOGIC_H
#define STATELOGIC_H

//#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "ConfigSettings.h"
#include "NetworkConfig.h"
#include "NetworkObjects.h"
//#include "World.h"

#include "Exports.h"

using namespace std;

static class COMMON_API StateLogic {
public:

	static struct stateInfo			statesInfo[MAX_PLAYERS];
	
	static void clearStateInfo(int id);

	static void startClimbing(struct playerObject *e, int buildingId);
	static void startHoldingEdge(struct playerObject *e, int buildingId);
	static void startPullingUp(struct playerObject *e, int buildingId);
	static void startWallRunning(struct playerObject *e, int newDirection, glm::vec3 toAdd, float angle, int buildingId);
	
	static void applyPullingUp(struct playerObject *p);
	static void applyHoldingEdge(struct playerObject *p);
	static void applyWallRunning(struct playerObject *p);
	static void applyClimbing(struct playerObject *p);

	static void handleXRotation(struct playerObject *e, float angle);
	static void handleYRotation(struct playerObject *e, float angle);
	static void calculateAxis(struct playerObject *e);

};

#endif