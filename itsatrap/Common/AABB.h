#ifndef AABB_H
#define AABB_H

#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <algorithm>

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
		float m_onRampXZFactor;
		float m_onRampYFactor;


		AABB();
		AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		AABB(glm::vec3 pos, float rad);
		AABB(AABB *);
		~AABB();

		void initCommon();

		void setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void setAABB(glm::vec3 pos, float rad);
		
		bool collidesWith(AABB other);
		bool collidesWithPointer(AABB* other);
		bool collidesWithSide(AABB other);
		bool collidesWithSidePointer(AABB* other);
		bool collidesWithRampEntrance(glm::vec3 from, AABB other, int entrance); // 0 = -x, 1 = +x, 2 = -y, 3 = +y, 4 = -z, 5 = +z
		bool collidesWithRampInside(AABB other, int entrance, float slope);
		bool inside(glm::vec3 goTo);
		bool inside(AABB other);
		bool nearTop(glm::vec3 goTo);
		bool clearedTop(AABB other);
		bool onTopOfPointer(AABB* other);
		bool onTopOfRamp(AABB other);
		bool fellOffSide(AABB other);

		glm::vec3 intersects(glm::vec3 from, glm::vec3 goTo);
		int reflectionIntersection(glm::vec3 from, glm::vec3 goTo);
		float angleIntersection(glm::vec3 from, glm::vec3 goTo);

		bool cameFromTop(glm::vec3 from, glm::vec3 goTo, AABB player, int buildingId);
		bool cameFromBottom(glm::vec3 from, glm::vec3 goTo, AABB player, int buildingId);
		void draw();
		void update(glm::vec3 pos, AABB *offset);

		void print();
};

#endif