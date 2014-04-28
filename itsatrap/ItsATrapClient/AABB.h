#ifndef AABB_H
#define AABB_H

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

		AABB();
		AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		AABB(glm::vec3 pos, float rad);
		~AABB();

		void setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void setAABB(glm::vec3 pos, float rad);
		
		bool collidesWith(AABB other);
		bool inside(glm::vec3 goTo);

		void print();
};

#endif