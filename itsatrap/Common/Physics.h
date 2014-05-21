#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

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

#include "NetworkObjects.h"
#include "AABB.h"
//#include "World.h"

#include "Exports.h"

using namespace std;
/*
enum PhysicsStates {
	None = 0,
	Colliding = 2,
	Moving = 3,
	Climbing = 5,
	Sliding = 6,
	WallJumping = 7,
	PullingUp = 8,
	HoldingEdge = 9,
	WallRunning = 10
};

enum WallRunningYStates {
	WRY_Off = 0, // same as readjusted
	WRY_Adjusting = 1,
	WRY_Adjusted = 2,
	WRY_Readjusting = 3
};

enum WallRunningXStates {
	WRX_Off = 0, // same as readjusted
	WRX_Adjusting = 1,
	WRX_Adjusted = 2,
	WRX_Readjusting = 3
};
*/
class COMMON_API Physics {
	public:

		static int applyGravity(AABB* player);
		static bool atRest();
		static void toggleTriedToRun();
		static int handleCollisionDetection(AABB* other);
		static float handleIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other); // not used
		static float handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e);
		static int handleReflectionIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e);
		static bool handleNearTop(glm::vec3 from, int buildingId);
		static bool handleClearedTop(AABB* other, int buildingId);

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);
};

#endif