#include "AABB.h"
#include "Client.h"

AABB::AABB() {
	m_minX = 0;
	m_minY = 0;
	m_minZ = 0;
	m_maxX = 0;
	m_maxY = 0;
	m_maxZ = 0;
	initCommon();
}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
	initCommon();
}

AABB::AABB(glm::vec3 pos, float rad) {
	m_minX = pos.x - rad;
	m_minY = pos.y - rad;
	m_minZ = pos.z - rad;
	m_maxX = pos.x + rad;
	m_maxY = pos.y + rad;
	m_maxZ = pos.z + rad;
	initCommon();
}

AABB::~AABB() {

}

void AABB::initCommon() {
	m_nearTopFactor = 15.0f;
}

void AABB::setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;

	//Client::sendStaticObject(m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ);
}

void AABB::setAABB(glm::vec3 pos, float rad) {
	m_minX = pos.x - rad;
	m_minY = pos.y - rad;
	m_minZ = pos.z - rad;
	m_maxX = pos.x + rad;
	m_maxY = pos.y + rad;
	m_maxZ = pos.z + rad;

	//Client::sendStaticObject(m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ);
}
// this = obj ie building. other = player
bool AABB::collidesWith(AABB other) {
	bool tmp = (other.m_maxX >= m_minX && other.m_minX <= m_maxX
		&& other.m_maxY >= m_minY && other.m_minY <= m_maxY
		&& other.m_maxZ >= m_minZ && other.m_minZ <= m_maxZ);

	if (tmp) {
		return (!(other.m_maxY >= m_maxY));
	}
	return tmp;
}

bool AABB::collidesWithPointer(AABB* other) {
	bool tmp = (other->m_maxX >= m_minX && other->m_minX <= m_maxX
		&& other->m_maxY >= m_minY && other->m_minY <= m_maxY
		&& other->m_maxZ >= m_minZ && other->m_minZ <= m_maxZ);

	if (tmp) {
		return (!(other->m_maxY >= m_maxY));
	}
	return tmp;
}

bool AABB::onTopOf(AABB* other) {
	return (other->m_maxX >= m_minX && other->m_minX <= m_maxX
		&& other->m_maxY >= m_maxY /*&& other->m_minY >= m_minY */
		&& other->m_maxZ >= m_minZ && other->m_minZ <= m_maxZ);
}

bool AABB::inside(glm::vec3 goTo) {
	return (goTo.x >= m_minX && goTo.x <= m_maxX 
		&& goTo.y >= m_minY &&  goTo.y <= m_maxY
		&& goTo.z >= m_minZ && goTo.z <= m_maxZ); 
}

bool AABB::inside(AABB other) {
	return (m_minX > other.m_minX && m_maxX < other.m_maxX 
		&& m_minY > other.m_minY && m_maxY < other.m_maxY 
		&& m_minZ > other.m_minZ && m_maxZ < other.m_maxZ);
	
}

bool AABB::nearTop(glm::vec3 goTo) {
	return (goTo.y >= m_maxY-m_nearTopFactor);
}

bool AABB::clearedTop(AABB* other) {
	return (other->m_minY >= m_maxY+m_nearTopFactor);
}

glm::vec3 AABB::intersects(glm::vec3 from, glm::vec3 goTo) {
	glm::vec3 direction = goTo-from;

	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	if (direction.x != 0) {
		xMinCoeff = ((m_minX - from.x)/direction.x > 0) ? ((m_minX - from.x)/direction.x > 0) : FLT_MAX;
		xMaxCoeff = ((m_maxX - from.x)/direction.x > 0) ? ((m_maxX - from.x)/direction.x > 0) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((m_minY - from.y)/direction.y > 0) ? ((m_minY - from.y)/direction.y > 0) : FLT_MAX;
		yMaxCoeff = ((m_maxY - from.y)/direction.y > 0) ? ((m_maxY - from.y)/direction.y > 0) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((m_minZ - from.z)/direction.z > 0) ? ((m_minZ - from.z)/direction.z > 0) : FLT_MAX;
		zMaxCoeff = ((m_maxZ - from.z)/direction.z > 0) ? ((m_maxZ - from.z)/direction.z > 0) : FLT_MAX;
	}

	float coeff = min(min(min(xMinCoeff, xMaxCoeff), min(yMinCoeff, yMaxCoeff)), min(zMinCoeff, zMaxCoeff));

	return from+(coeff*direction);

}

int AABB::reflectionIntersection(glm::vec3 from, glm::vec3 goTo) {
	glm::vec3 direction = goTo-from;

	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	if (direction.x != 0) {
		xMinCoeff = ((m_minX - from.x)/direction.x > 0) ? ((m_minX - from.x)/direction.x) : FLT_MAX;
		xMaxCoeff = ((m_maxX - from.x)/direction.x > 0) ? ((m_maxX - from.x)/direction.x) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((m_minY - from.y)/direction.y > 0) ? ((m_minY - from.y)/direction.y) : FLT_MAX;
		yMaxCoeff = ((m_maxY - from.y)/direction.y > 0) ? ((m_maxY - from.y)/direction.y) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((m_minZ - from.z)/direction.z > 0) ? ((m_minZ - from.z)/direction.z) : FLT_MAX;
		zMaxCoeff = ((m_maxZ - from.z)/direction.z > 0) ? ((m_maxZ - from.z)/direction.z) : FLT_MAX;
	}
	float coeff = min(min(min(xMinCoeff, xMaxCoeff), min(yMinCoeff, yMaxCoeff)), min(zMinCoeff, zMaxCoeff));

	if (coeff == xMinCoeff) {
		return 0;
	}
	if (coeff == xMaxCoeff) {
		return 1;
	}
		
	if (coeff == zMinCoeff) {
		return 4;
	}
	if (coeff == zMaxCoeff) {
		return 5;
	}

	else {
		return -1;
	}

}

float AABB::angleIntersection(glm::vec3 from, glm::vec3 goTo) {
	glm::vec3 direction = goTo-from;

	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	if (direction.x != 0) {
		xMinCoeff = ((m_minX - from.x)/direction.x > 0) ? ((m_minX - from.x)/direction.x) : FLT_MAX;
		xMaxCoeff = ((m_maxX - from.x)/direction.x > 0) ? ((m_maxX - from.x)/direction.x) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((m_minY - from.y)/direction.y > 0) ? ((m_minY - from.y)/direction.y) : FLT_MAX;
		yMaxCoeff = ((m_maxY - from.y)/direction.y > 0) ? ((m_maxY - from.y)/direction.y) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((m_minZ - from.z)/direction.z > 0) ? ((m_minZ - from.z)/direction.z) : FLT_MAX;
		zMaxCoeff = ((m_maxZ - from.z)/direction.z > 0) ? ((m_maxZ - from.z)/direction.z) : FLT_MAX;
	}
	float tmpAngle;
	float coeff = min(min(min(xMinCoeff, xMaxCoeff), min(yMinCoeff, yMaxCoeff)), min(zMinCoeff, zMaxCoeff));

	if (coeff == xMinCoeff || coeff == xMaxCoeff) {
		tmpAngle = 180.0f*(acos((direction.z*direction.z)/(glm::length(direction)*direction.z)))/(atan(1.0f)*4.0f);
		/*if (coeff == xMinCoeff) {
			cout << "tmpAngle1: " << tmpAngle << endl;
		}
		else {
			cout << "tmpAngle2: " << tmpAngle << endl;
		}*/
		return tmpAngle;
	}
	else if (coeff == yMinCoeff || coeff == yMaxCoeff) {
		return -1.0f;
	}
	else {
		tmpAngle = 180.0f*(acos((direction.x*direction.x)/(glm::length(direction)*direction.x)))/(atan(1.0f)*4.0f);
		/*if (coeff == zMinCoeff) {
			cout << "tmpAngle3: " << tmpAngle << endl;
		}
		else {
			cout << "tmpAngle4: " << tmpAngle << endl;
		}*/
		return tmpAngle;
	}
}

void AABB::print() {
	cout << "(" << m_minX << ", " << m_minY << ", " << m_minZ << ") (" << m_maxX << ", " << m_maxY << ", " << m_maxZ << ")" << endl;
}