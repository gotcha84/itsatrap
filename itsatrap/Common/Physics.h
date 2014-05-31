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

class COMMON_API Physics {
	public:

		static bool atRest();
		static void toggletriedForward();
		static int handleCollisionDetection(AABB* other);
		static float handleIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other); // not used
		
		static float handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e);
		static int handleReflectionIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e);
		static bool handleNearTop(glm::vec3 from, struct staticObject e);
		static bool handleClearedTop(AABB other, struct staticObject e);

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);
};

#endif