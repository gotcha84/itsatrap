#include "AABB.h"

AABB::AABB() {
	m_minX = 0;
	m_minY = 0;
	m_minZ = 0;
	m_maxX = 0;
	m_maxY = 0;
	m_maxZ = 0;
}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
}

AABB::AABB(glm::vec3 pos, float rad) {
	m_minX = pos.x - rad;
	m_minY = pos.y - rad;
	m_minZ = pos.z - rad;
	m_maxX = pos.x + rad;
	m_maxY = pos.y + rad;
	m_maxZ = pos.z + rad;
}

AABB::~AABB() {

}

void AABB::setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
}

void AABB::setAABB(glm::vec3 pos, float rad) {
	m_minX = pos.x - rad;
	m_minY = pos.y - rad;
	m_minZ = pos.z - rad;
	m_maxX = pos.x + rad;
	m_maxY = pos.y + rad;
	m_maxZ = pos.z + rad;
}

bool AABB::collidesWith(AABB other) {
	return (other.m_maxX >= m_minX && other.m_minX <= m_maxX
			&& other.m_maxY >= m_minY && other.m_minY <= m_maxY
			&& other.m_maxZ >= m_minZ && other.m_minZ <= m_maxZ);
		/*return ((this->bmax.x() >= b.bmin.x() && this->bmin.x() <= b.bmax.x())
		&& (this->bmax.y() >= b.bmin.y() && this->bmin.y() <= b.bmax.y())
		&& (this->bmax.z() >= b.bmin.z() && this->bmin.z() <= b.bmax.z()));{*/
}

bool AABB::inside(glm::vec3 goTo) {

	//cout << "minx: " << m_minX << endl;
	//cout << "miny: " << m_minY << endl;
	//cout << "minz: " << m_minZ << endl;
	//cout << "maxx: " << m_maxX << endl;
	//cout << "maxy: " << m_maxY << endl;
	//cout << "maxz: " << m_maxZ << endl;
	bool tmp = (goTo.x >= m_minX && goTo.x <= m_maxX 
		   && goTo.y >= m_minY && goTo.y <= m_maxY
		   && goTo.z >= m_minZ && goTo.z <= m_maxZ);
	//cout << "collisiondetected?:" << tmp << endl; 


	//if (!tmp) {
		/*cout << "minx: " << m_minX << endl;
		cout << "miny: " << m_minY << endl;
		cout << "minz: " << m_minZ << endl;
		cout << "maxx: " << m_maxX << endl;
		cout << "maxy: " << m_maxY << endl;
		cout << "maxz: " << m_maxZ << endl;
		cout << "goto: " << glm::to_string(goTo) << endl;*/
	//}

	return (goTo.x >= m_minX && goTo.x <= m_maxX 
		   && goTo.y >= m_minY && goTo.y <= m_maxY
		   && goTo.z >= m_minZ && goTo.z <= m_maxZ); 
}

void AABB::print() {
	cout << "(" << m_minX << ", " << m_minY << ", " << m_minZ << ") (" << m_maxX << ", " << m_maxY << ", " << m_maxZ << ")" << endl;
}
