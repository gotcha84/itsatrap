#include "AABB.h"
#include "Client.h"

AABB::AABB() {
	m_minX = 0;
	m_minY = 0;
	m_minZ = 0;
	m_maxX = 0;
	m_maxY = 0;
	m_maxZ = 0;

	m_nearTopFactor = 10.0f;
}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;

	m_nearTopFactor = 10.0f;

}

AABB::AABB(glm::vec3 pos, float rad) {
	m_minX = pos.x - rad;
	m_minY = pos.y - rad;
	m_minZ = pos.z - rad;
	m_maxX = pos.x + rad;
	m_maxY = pos.y + rad;
	m_maxZ = pos.z + rad;

	m_nearTopFactor = 10.0f;
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

bool AABB::collidesWith(AABB other) {
	return (other.m_maxX >= m_minX && other.m_minX <= m_maxX
		&& other.m_maxY >= m_minY && other.m_minY <= m_maxY
		&& other.m_maxZ >= m_minZ && other.m_minZ <= m_maxZ);
}

bool AABB::collidesWithPointer(AABB* other) {
	return (other->m_maxX >= m_minX && other->m_minX <= m_maxX
		&& other->m_maxY >= m_minY && other->m_minY <= m_maxY
		&& other->m_maxZ >= m_minZ && other->m_minZ <= m_maxZ);
}

bool AABB::inside(glm::vec3 goTo) {
	return (goTo.x >= m_minX && goTo.x <= m_maxX 
		&& goTo.y >= m_minY && goTo.y <= m_maxY
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
	return (other->m_minY >= m_maxY);
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

	cout << "from: " << glm::to_string(from) << endl;
	cout << "direction: " << glm::to_string(direction) << endl;
	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	// TODO: fix properly
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

	if (coeff == xMinCoeff || coeff == xMaxCoeff) {
		return 0;
	}
		
	if (coeff == zMinCoeff || coeff == zMaxCoeff) {
		return 2;
	}

	else {
		return 1;
	}

}

float AABB::angleIntersection(glm::vec3 from, glm::vec3 goTo) {
	glm::vec3 direction = goTo-from;

	cout << "from: " << glm::to_string(from) << endl;
	cout << "direction: " << glm::to_string(direction) << endl;
	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	// TODO: fix properly
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
		if (coeff == xMinCoeff) {
			cout << "xMinCoeff" << endl;
		}
		else {
			cout << "xMaxCoeff" << endl;
		}
		tmpAngle = 180.0f*(acos((direction.z*direction.z)/(glm::length(direction)*direction.z)))/(atan(1.0f)*4.0f);
		cout << "tmpAngle" << tmpAngle << endl; 
		return abs(90.0f-tmpAngle);
	}
	else if (coeff == yMinCoeff || coeff == yMaxCoeff) {
		return -1.0f;
	}
	else {
		if (coeff == zMinCoeff) {
			cout << "zMinCoeff" << endl;
		}
		else {
			cout << "zMaxCoeff" << endl;
		}
		tmpAngle = 180.0f*(acos((direction.x*direction.x)/(glm::length(direction)*direction.x)))/(atan(1.0f)*4.0f);
		cout << "tmpAngle" << tmpAngle << endl; 
		return abs(90.0f-tmpAngle);
	}
	/*while (tmpAngle > 90.0f) {
		tmpAngle -= 90.0f;
	}
	return abs(45.0f-tmpAngle);*/
}

void AABB::print() {
	cout << "(" << m_minX << ", " << m_minY << ", " << m_minZ << ") (" << m_maxX << ", " << m_maxY << ", " << m_maxZ << ")" << endl;
}