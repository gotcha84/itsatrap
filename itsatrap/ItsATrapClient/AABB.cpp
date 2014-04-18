#include <math.h>

#include "AABB.h"

using namespace std;

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
}


bool AABB::collidesWith(AABB other) {
	return (other.m_maxX >= m_minX && other.m_minX <= m_maxX
			&& other.m_maxY >= m_minY && other.m_minY <= m_maxY
			&& other.m_maxZ >= m_minZ && other.m_minZ <= m_maxZ);
		/*return ((this->bmax.x() >= b.bmin.x() && this->bmin.x() <= b.bmax.x())
		&& (this->bmax.y() >= b.bmin.y() && this->bmin.y() <= b.bmax.y())
		&& (this->bmax.z() >= b.bmin.z() && this->bmin.z() <= b.bmax.z()));{*/
}