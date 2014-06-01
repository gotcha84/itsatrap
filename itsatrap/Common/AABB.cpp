#include "AABB.h"

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
	this->minX = minX;
	this->minY = minY;
	this->minZ = minZ;
	this->maxX = maxX;
	this->maxY = maxY;
	this->maxZ = maxZ;
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

AABB::AABB(AABB *a)
{
	minX = a->minX;
	minY = a->minY;
	minZ = a->minZ;
	maxX = a->maxX;
	maxY = a->maxY;
	maxZ = a->maxZ;
}

AABB::~AABB() {

}

void AABB::initCommon() {
	m_nearTopFactor = 15.0f;
	m_overTopFactor = 5.0f;
	m_onRampXZFactor = 1.0f;
	m_onRampYFactor = 10.0f;
}

void AABB::setAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
	this->minX = minX;
	this->minY = minY;
	this->minZ = minZ;
	this->maxX = maxX;
	this->maxY = maxY;
	this->maxZ = maxZ;

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

	/*cout << "this: ";
	this->print();
	cout << "other: ";
	other.print();*/

	return (other.maxX >= minX && other.minX <= maxX
		&& other.maxY - m_nearTopFactor >= maxY
		&& other.maxZ >= minZ && other.minZ <= maxZ);
}

bool AABB::collidesWithSidePointer(AABB* other) {
	return (other->maxX >= minX && other->minX <= maxX
		&& other->maxY - m_nearTopFactor >= maxY
		&& other->maxZ >= minZ && other->minZ <= maxZ);
}

bool AABB::onTopOfPointer(AABB* other) {
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
// TODO: imp coming from other entrance of ramp if needed
// this = obj ie building. other = player
bool AABB::collidesWithRampEntrance(glm::vec3 from, AABB other, int entrance) {
		
	if (other.minY > minY + m_onRampYFactor) {
		//cout << "too tall" << endl;
		return false;
	}

	glm::vec3 pos = glm::vec3(other.minX + 5.0f, other.minY + 5.0f, other.minZ + 5.0f);

	/*cout << "building: ";
	this->print();
	cout << "player: ";
	other.print();

	cout << "from: " << glm::to_string(from) << endl;
	cout << "pos:" << glm::to_string(pos) << endl;*/
	//cout << "entrance: " << entrance << endl;

	

	switch (entrance) {
		case 0:
			if (minZ < from.z && maxZ > from.z) {
				//cout << "from entrance" << endl;
				return (other.minX - minX < maxX - other.maxX && minX - m_onRampXZFactor < pos.x && maxX > other.maxX && minZ < other.minZ && maxZ > other.maxZ);
			}
			return false;
			//else {
			//	//cout << "from side" << endl;
			//	return (minX < other.minX + 5.0f && minX + m_onRampXZFactor > other.minX + 5.0f && minZ > other.minZ && maxZ < other.maxZ);
			//}
			break;
		case 1:
			if (minZ < from.z && maxZ > from.z) {
				return (other.minX - minX > maxX - other.maxX && minX < other.minX && maxX + m_onRampXZFactor > pos.x && minZ < other.minZ && maxZ > other.maxZ);
			}
			//else {
			//	return (maxX > other.maxX - 5.0f && maxX - m_onRampXZFactor < other.maxX - 5.0f && minZ > other.minZ && maxZ < other.maxZ);
			//}
			return false;
			break;
		case 2:
			cout << "why come from underground??" << endl;
			break;
		case 3:
			cout << "why come from sky??" << endl;
			break;
		case 4:
			if (minX < from.x && maxX > from.x) {
				//cout << "from entrance" << endl;
				//cout << "first: " << (other.minZ - minZ < maxZ - other.maxZ) << endl;
				//cout << "second: " << (minZ /*- m_onRampXZFactor*/ < pos.z) << endl;
				//cout << "third: " << (maxZ > other.maxZ) << endl;
				//cout << "fourth: " << (minX < other.minX) << endl;
				//cout << "fifth: " << (maxX > other.maxX) << endl << endl;
				return (other.minZ - minZ < maxZ - other.maxZ && minZ /*- m_onRampXZFactor*/ < pos.z && maxZ > other.maxZ && minX < other.minX && maxX > other.maxX);
			}
			/*else {
				return (minZ < other.minZ + 5.0f && minZ + m_onRampXZFactor > other.minZ + 5.0f && minX > other.minX && maxX < other.maxX);
			}*/
			return false;
			break;
		case 5:
			if (minX < from.x && maxX > from.x) {
				return (other.minZ - minZ > maxZ - other.maxZ && minZ < other.minZ && maxZ + m_onRampXZFactor > pos.z && minX < other.minX && maxX > other.maxX);
			}
			/*else {
				return (maxZ > other.maxZ - 5.0f && maxZ - m_onRampXZFactor < other.maxZ - 5.0f && minX > other.minX && maxX < other.maxX);
			}*/
			return false;
			break;
	}

	return false;
}

bool AABB::onTopOfRamp(AABB other) {
	return (maxX >= other.minX && minX <= other.maxX
		//&& other.maxY >= maxY /*&& other->minY >= minY */
		&& maxZ >= other.minZ && minZ <= other.maxZ);
}

// this = player, other = building
bool AABB::collidesWithRampInside(AABB other, int entrance, float slope) {

	if (!(minX > other.minX && maxX < other.maxX
		&& minY > other.minY
		&& minZ > other.minZ && maxZ < other.maxZ)) {
		//cout << "didnt pass prelim check" << endl;
		return false;
	}
	/*cout << "building: ";
	other.print();
	cout << "player: ";
	this->print();

	cout << "slope: " << slope << endl;
	cout << "entrance: " << entrance << endl;*/

	glm::vec3 pos = glm::vec3(other.minX + 5.0f, other.minY /*+ 5.0f*/, other.minZ + 5.0f);
	switch (entrance) {
		case 0:
			return (pos.y < other.minY + (pos.x - other.minX)*slope);
			break;
		case 1:
			return (pos.y < other.minY + (other.maxX - pos.x)*slope);
			break;
		case 2:
			cout << "y come from ground" << endl;
			break;
		case 3:
			cout << "y come from sky" << endl;
			break;
		case 4:
			return (pos.y < other.minY + (pos.z - other.minZ)*slope);
			break;
		case 5:
			return (pos.y < other.minY + (other.maxZ - pos.z)*slope);
			break;
		default:
			break;
	}
}


bool AABB::nearTop(glm::vec3 goTo) {
	return (goTo.y >= maxY-m_nearTopFactor);
}

bool AABB::clearedTop(AABB other) {
	return (other.minY >= maxY+m_overTopFactor);
}

// this = obj ie building. other = player
bool AABB::fellOffSide(AABB other) {
	/*bool tmp = !((minX < other.minX && maxX > other.maxX)
		|| (minZ < other.minZ && maxZ > other.maxZ));*/
	/*cout << "building: ";
	this->print();
	cout << "player: ";
	other.print();
	cout << "first: " << (minX < other.minX && maxX > other.maxX) << endl;
	cout << "second: " << (minZ < other.minZ && maxZ > other.maxZ) << endl;
	cout << "result : " << tmp << endl << endl;*/
	return !((minX < other.minX && maxX > other.maxX)
		|| (minZ < other.minZ && maxZ > other.maxZ));
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

void AABB::draw() {
	glm::vec3 p0(this->minX, this->minY, this->minZ);
	glm::vec3 p1(this->minX, this->minY, this->maxZ);
	glm::vec3 p2(this->maxX, this->minY, this->maxZ);
	glm::vec3 p3(this->maxX, this->minY, this->minZ);
	glm::vec3 p4(this->maxX, this->maxY, this->minZ);
	glm::vec3 p5(this->minX, this->maxY, this->minZ);
	glm::vec3 p6(this->minX, this->maxY, this->maxZ);
	glm::vec3 p7(this->maxX, this->maxY, this->maxZ);

	glColor4f(1, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p5.x, p5.y, p5.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p7.x, p7.y, p7.z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p5.x, p5.y, p5.z);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p6.x, p6.y, p6.z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p7.x, p7.y, p7.z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

void AABB::print() {
	cout << "(" << minX << ", " << minY << ", " << minZ << ") (" << maxX << ", " << maxY << ", " << maxZ << ")" << endl;
}

bool AABB::cameFromTop(glm::vec3 from, glm::vec3 goTo, AABB player, int buildingId) {

	int checkId1 = -1;
	int checkId2 = -2;
	int checkId3 = -3;

	if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
		cout << "buildingid: " << buildingId << endl;
		cout << "from: " << glm::to_string(from) << endl;
		cout << "to: " << glm::to_string(goTo) << endl;
		cout << "my AABB: ";
		print();
		cout << "player AABB: ";
		player.print();
	}

	bool fromInter = (from.x > minX && from.x < maxX && from.z > minZ && from.z < maxZ);
	bool goToInter = (goTo.x > minX && goTo.x < maxX && goTo.z > minZ && goTo.z < maxZ);

	if (goToInter && goTo.y > minY && goTo.y < maxY) {
		return true;
	}

	if (!collidesWith(player) && !(from.y > maxY && goTo.y < minY && (fromInter || goToInter))) {
		if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
			cout << !collidesWith(player) << endl << endl;
		}
		return false;
	}

	glm::vec3 direction = goTo - from;

	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	if (direction.x != 0) {
		xMinCoeff = ((minX - from.x) / direction.x > 0) ? ((minX - from.x) / direction.x) : FLT_MAX;
		xMaxCoeff = ((maxX - from.x) / direction.x > 0) ? ((maxX - from.x) / direction.x) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((minY - from.y) / direction.y > 0) ? ((minY - from.y) / direction.y) : FLT_MAX;
		yMaxCoeff = ((maxY - from.y) / direction.y > 0) ? ((maxY - from.y) / direction.y) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((minZ - from.z) / direction.z > 0) ? ((minZ - from.z) / direction.z) : FLT_MAX;
		zMaxCoeff = ((maxZ - from.z) / direction.z > 0) ? ((maxZ - from.z) / direction.z) : FLT_MAX;
	}
	float coeff = min(min(min(xMinCoeff, xMaxCoeff), min(yMinCoeff, yMaxCoeff)), min(zMinCoeff, zMaxCoeff));

	if (yMaxCoeff < yMinCoeff && goToInter) {
		if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
			cout << "Was true" << endl << endl;
		}
		return true;
	}
	if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
		cout << "Was false" << endl << endl;
	}
	return false;
}

bool AABB::cameFromBottom(glm::vec3 from, glm::vec3 goTo, AABB player, int buildingId) {

	int checkId1 = -1;
	int checkId2 = -2;
	int checkId3 = -3;

	if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
		cout << "buildingid: " << buildingId << endl;
		cout << "from: " << glm::to_string(from) << endl;
		cout << "to: " << glm::to_string(goTo) << endl;
		cout << "my AABB: ";
		print();
		cout << "player AABB: ";
		player.print();
	}

	// didnt come from lower
	if (!(from.y < minY)) {
		return false;
	}
	
	if (!collidesWith(player) && !(from.y < minY && goTo.y > maxY)) {
		return false;
	}

	bool fromInter = (from.x > minX && from.x < maxX && from.z > minZ && from.z < maxZ);
	bool goToInter = (goTo.x > minX && goTo.x < maxX && goTo.z > minZ && goTo.z < maxZ);

	
	if (!(fromInter || goToInter)) {
		return false;
	}

	glm::vec3 direction = goTo - from;

	float xMinCoeff = FLT_MAX;
	float xMaxCoeff = FLT_MAX;
	float yMinCoeff = FLT_MAX;
	float yMaxCoeff = FLT_MAX;
	float zMinCoeff = FLT_MAX;
	float zMaxCoeff = FLT_MAX;

	if (direction.x != 0) {
		xMinCoeff = ((minX - from.x) / direction.x > 0) ? ((minX - from.x) / direction.x) : FLT_MAX;
		xMaxCoeff = ((maxX - from.x) / direction.x > 0) ? ((maxX - from.x) / direction.x) : FLT_MAX;
	}
	if (direction.y != 0) {
		yMinCoeff = ((minY - from.y) / direction.y > 0) ? ((minY - from.y) / direction.y) : FLT_MAX;
		yMaxCoeff = ((maxY - from.y) / direction.y > 0) ? ((maxY - from.y) / direction.y) : FLT_MAX;
	}
	if (direction.z != 0) {
		zMinCoeff = ((minZ - from.z) / direction.z > 0) ? ((minZ - from.z) / direction.z) : FLT_MAX;
		zMaxCoeff = ((maxZ - from.z) / direction.z > 0) ? ((maxZ - from.z) / direction.z) : FLT_MAX;
	}
	float coeff = min(min(min(xMinCoeff, xMaxCoeff), min(yMinCoeff, yMaxCoeff)), min(zMinCoeff, zMaxCoeff));

	if (yMinCoeff < yMaxCoeff) {
		if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
			cout << "Was true" << endl << endl;
		}
		return true;
	}
	if (buildingId == checkId1 || buildingId == checkId2 || buildingId == checkId3) {
		cout << "Was false" << endl << endl;
	}
	return false;
}




void AABB::update(glm::vec3 pos, AABB *offset)
{
	/*cout << "offset->minX: " << offset->minX << endl;
	cout << "offset->minY: " << offset->minY << endl;
	cout << "offset->minZ: " << offset->minZ << endl;
	cout << "offset->maxX: " << offset->maxX << endl;
	cout << "offset->maxY: " << offset->maxY << endl;
	cout << "offset->maxZ: " << offset->maxZ << endl;*/

	minX = pos.x + offset->minX;
	minY = pos.y + offset->minY;
	minZ = pos.z + offset->minZ;

	maxX = pos.x + offset->maxX;
	maxY = pos.y + offset->maxY;
	maxZ = pos.z + offset->maxZ;
}