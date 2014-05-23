#ifndef AABB_H
#define AABB_H

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Exports.h"

using namespace std;

class COMMON_API AABB {
	public:
		float minX;
		float minY;
		float minZ;
		float maxX;
		float maxY;
		float maxZ;

		float m_nearTopFactor;
		float m_overTopFactor;


		AABB();
		AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		AABB(glm::vec3 pos, float rad);
		~AABB();

		void initCommon();

		void setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void setAABB(glm::vec3 pos, float rad);
		
		bool collidesWith(AABB other);
		bool collidesWithPointer(AABB* other);
		bool collidesWithSide(AABB other);
		bool collidesWithSidePointer(AABB* other);
		bool inside(glm::vec3 goTo);
		bool inside(AABB other);
		bool nearTop(glm::vec3 goTo);
		bool clearedTop(AABB other);
		bool onTopOf(AABB* other);
		bool fellOffSide(AABB other);

		glm::vec3 intersects(glm::vec3 from, glm::vec3 goTo);
		int reflectionIntersection(glm::vec3 from, glm::vec3 goTo);
		float angleIntersection(glm::vec3 from, glm::vec3 goTo);

		void print();
};

#endif