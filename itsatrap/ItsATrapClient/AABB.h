#ifndef AABB_H
#define AABB_H

#include "SceneGraph.h"

using namespace std;

class AABB {
	
	public:
	
	float m_minX;
	float m_minY;
	float m_minZ;
	float m_maxX;
	float m_maxY;
	float m_maxZ;

	AABB(float, float, float, float, float, float);

	bool collidesWith(AABB);
};

#endif