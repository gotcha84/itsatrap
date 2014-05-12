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

using namespace std;

class AABB {
	public:
		float m_minX;
		float m_minY;
		float m_minZ;
		float m_maxX;
		float m_maxY;
		float m_maxZ;

		float m_nearTopFactor;

		AABB();
		AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		AABB(glm::vec3 pos, float rad);
		~AABB();

		void initCommon();

		void setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void setAABB(glm::vec3 pos, float rad);
		
		bool collidesWith(AABB other);
		bool collidesWithPointer(AABB* other);
		bool inside(glm::vec3 goTo);
		bool inside(AABB other);
		bool nearTop(glm::vec3 goTo);
		bool clearedTop(AABB* other);
		glm::vec3 intersects(glm::vec3 from, glm::vec3 goTo);
		int reflectionIntersection(glm::vec3 from, glm::vec3 goTo);
		float angleIntersection(glm::vec3 from, glm::vec3 goTo);

		void print();
};

#endif