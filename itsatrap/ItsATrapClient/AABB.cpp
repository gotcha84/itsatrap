#include "AABB.h"
#include "Client.h"

AABB::AABB() {
}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
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

	Client::sendStaticObject(minX, minY, minZ, maxX, maxY, maxZ);
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
