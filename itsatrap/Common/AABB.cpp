#include "AABB.h"
//#include "Client.h"

AABB::AABB() {
	minX = 0;
	minY = 0;
	minZ = 0;
	maxX = 0;
	maxY = 0;
	maxZ = 0;
	initCommon();
}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	minX = minX;
	minY = minY;
	minZ = minZ;
	maxX = maxX;
	maxY = maxY;
	maxZ = maxZ;
	initCommon();
}

AABB::AABB(glm::vec3 pos, float rad) {
	minX = pos.x - rad;
	minY = pos.y - rad;
	minZ = pos.z - rad;
	maxX = pos.x + rad;
	maxY = pos.y + rad;
	maxZ = pos.z + rad;
	initCommon();
}

AABB::~AABB() {

}

void AABB::initCommon() {
	m_nearTopFactor = 15.0f;
	m_overTopFactor = 5.0f;

}

void AABB::setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	minX = minX;
	minY = minY;
	minZ = minZ;
	maxX = maxX;
	maxY = maxY;
	maxZ = maxZ;

	//Client::sendStaticObject(minX, minY, minZ, maxX, maxY, maxZ);
}

void AABB::setAABB(glm::vec3 pos, float rad) {
	minX = pos.x - rad;
	minY = pos.y - rad;
	minZ = pos.z - rad;
	maxX = pos.x + rad;
	maxY = pos.y + rad;
	maxZ = pos.z + rad;

	//Client::sendStaticObject(minX, minY, minZ, maxX, maxY, maxZ);
}
// this = obj ie building. other = player
bool AABB::collidesWith(AABB other) {
	bool tmp = (other.maxX >= minX && other.minX <= maxX
		&& other.maxY >= minY && other.minY <= maxY
		&& other.maxZ >= minZ && other.minZ <= maxZ);

	if (tmp) {
		return (!(other.maxY >= maxY));
	}
	return tmp;
}

bool AABB::collidesWithPointer(AABB* other) {
	bool tmp = (other->maxX >= minX && other->minX <= maxX
		&& other->maxY >= minY && other->minY <= maxY
		&& other->maxZ >= minZ && other->minZ <= maxZ);

	if (tmp) {
		return (!(other->maxY >= maxY));
	}
	return tmp;
}

bool AABB::collidesWithSide(AABB other) {
	return (other.maxX >= minX && other.minX <= maxX
		&& other.maxY <= maxY - m_nearTopFactor
		&& other.maxZ >= minZ && other.minZ <= maxZ);
}

bool AABB::collidesWithSidePointer(AABB* other) {
	return (other->maxX >= minX && other->minX <= maxX
		&& other->maxY <= maxY - m_nearTopFactor
		&& other->maxZ >= minZ && other->minZ <= maxZ);
}

bool AABB::onTopOf(AABB* other) {
	return (other->maxX >= minX && other->minX <= maxX
		&& other->maxY >= maxY /*&& other->minY >= minY */
		&& other->maxZ >= minZ && other->minZ <= maxZ);
}

bool AABB::inside(glm::vec3 goTo) {
	return (goTo.x >= minX && goTo.x <= maxX 
		&& goTo.y >= minY &&  goTo.y <= maxY
		&& goTo.z >= minZ && goTo.z <= maxZ); 
}

bool AABB::inside(AABB other) {
	return (minX > other.minX && maxX < other.maxX 
		&& minY > other.minY && maxY < other.maxY 
		&& minZ > other.minZ && maxZ < other.maxZ);
	
}

bool AABB::nearTop(glm::vec3 goTo) {
	return (goTo.y >= maxY-m_nearTopFactor);
}

bool AABB::clearedTop(AABB other) {
	return (other.minY >= maxY+m_overTopFactor);
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
		xMinCoeff = ((minX - from.x)/direction.x > 0) ? ((minX - from.x)/direction.x > 0) : FLT_MAX;
		xMaxCoeff = ((maxX - from.x)/direction.x > 0) ? ((maxX - from.x)/direction.x > 0) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((minY - from.y)/direction.y > 0) ? ((minY - from.y)/direction.y > 0) : FLT_MAX;
		yMaxCoeff = ((maxY - from.y)/direction.y > 0) ? ((maxY - from.y)/direction.y > 0) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((minZ - from.z)/direction.z > 0) ? ((minZ - from.z)/direction.z > 0) : FLT_MAX;
		zMaxCoeff = ((maxZ - from.z)/direction.z > 0) ? ((maxZ - from.z)/direction.z > 0) : FLT_MAX;
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
		xMinCoeff = ((minX - from.x)/direction.x > 0) ? ((minX - from.x)/direction.x) : FLT_MAX;
		xMaxCoeff = ((maxX - from.x)/direction.x > 0) ? ((maxX - from.x)/direction.x) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((minY - from.y)/direction.y > 0) ? ((minY - from.y)/direction.y) : FLT_MAX;
		yMaxCoeff = ((maxY - from.y)/direction.y > 0) ? ((maxY - from.y)/direction.y) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((minZ - from.z)/direction.z > 0) ? ((minZ - from.z)/direction.z) : FLT_MAX;
		zMaxCoeff = ((maxZ - from.z)/direction.z > 0) ? ((maxZ - from.z)/direction.z) : FLT_MAX;
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
		xMinCoeff = ((minX - from.x)/direction.x > 0) ? ((minX - from.x)/direction.x) : FLT_MAX;
		xMaxCoeff = ((maxX - from.x)/direction.x > 0) ? ((maxX - from.x)/direction.x) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((minY - from.y)/direction.y > 0) ? ((minY - from.y)/direction.y) : FLT_MAX;
		yMaxCoeff = ((maxY - from.y)/direction.y > 0) ? ((maxY - from.y)/direction.y) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((minZ - from.z)/direction.z > 0) ? ((minZ - from.z)/direction.z) : FLT_MAX;
		zMaxCoeff = ((maxZ - from.z)/direction.z > 0) ? ((maxZ - from.z)/direction.z) : FLT_MAX;
	}
	float tmpAngle;
	float coeff = min(min(min(xMinCoeff, xMaxCoeff), min(yMinCoeff, yMaxCoeff)), min(zMinCoeff, zMaxCoeff));

	if (coeff == xMinCoeff || coeff == xMaxCoeff) {
		tmpAngle = 180.0f*(acos((direction.z*direction.z)/(glm::length(direction)*direction.z)))/(atan(1.0f)*4.0f);

		if (coeff == xMinCoeff) {
			cout << "tmpAngle1: " << tmpAngle << endl;
		}
		else {
			cout << "tmpAngle2: " << tmpAngle << endl;
		}

		return tmpAngle;
	}
	else if (coeff == yMinCoeff || coeff == yMaxCoeff) {
		return -1.0f;
	}
	else {
		tmpAngle = 180.0f*(acos((direction.x*direction.x)/(glm::length(direction)*direction.x)))/(atan(1.0f)*4.0f);
		if (coeff == zMinCoeff) {
			cout << "tmpAngle3: " << tmpAngle << endl;
		}
		else {
			cout << "tmpAngle4: " << tmpAngle << endl;
		}
		return tmpAngle;
	}
}

void AABB::print() {
	cout << "(" << minX << ", " << minY << ", " << minZ << ") (" << maxX << ", " << maxY << ", " << maxZ << ")" << endl;
}