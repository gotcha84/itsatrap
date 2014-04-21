#ifndef AABB_H
#define AABB_H

#include <math.h>

using namespace std;

class AABB {
	public:
		float m_minX;
		float m_minY;
		float m_minZ;
		float m_maxX;
		float m_maxY;
		float m_maxZ;

		AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		~AABB();

		bool collidesWith(AABB);
};

#endif