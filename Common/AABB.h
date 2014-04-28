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

		__declspec(dllexport) AABB();
		__declspec(dllexport) AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		__declspec(dllexport) AABB(glm::vec3 pos, float rad);
		__declspec(dllexport) ~AABB();

		__declspec(dllexport) void setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		__declspec(dllexport) void setAABB(glm::vec3 pos, float rad);
		
		__declspec(dllexport) bool collidesWith(AABB other);
		__declspec(dllexport) bool inside(glm::vec3 goTo);

		__declspec(dllexport) void print();
};

#endif