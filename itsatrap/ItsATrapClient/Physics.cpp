#include "Physics.h"
#include "ClientInstance.h"

extern ClientInstance *client;

Physics::Physics() {
	initCommon();
}

Physics::Physics(glm::vec3 pos) {
	m_position = glm::vec3(pos.x, pos.y, pos.z);
	initCommon();
}

Physics::Physics(glm::vec3 pos, float mass) {
	m_position = glm::vec3(pos.x, pos.y, pos.z);
	m_mass = mass;
	initCommon();
}

Physics::~Physics() {

}

void Physics::initCommon() {
	m_currentState = PhysicsStates::None;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lastMoved = glm::vec3(0.0f, 0.0f, 0.0f);
	
	m_yJumpFactor = 0.75f;

	m_gravityConstant = -0.01f; 
	m_gravity  = glm::vec3(0.0f, m_gravityConstant, 0.0f);
	m_elasticityConstant = 0.9f;

	m_restConstant = glm::vec3(0.01f, 0.01f, 0.01f);
	m_lastTeleported = clock();
	m_teleportDelay = 5.0f;
	m_slideDelay = 0.5f;

	m_climbLookUp = 70.0f;
	//cout << "MwalljumP: " << glm::to_string(m_climbLookUp) << endl;
	m_climbLookDown = -20.0f;
	m_climbLookUpDownNumFrames = 28.0f;
	m_climbLookDownReadjustNumFrames = 60.0f;
	m_climbLookDownNumFramesFraction = 0.3f;
	m_climbLookReadjustNumFramesFraction = 0.7f;
	m_climbLookUpIncrement = m_climbLookUp/m_climbLookUpDownNumFrames;
	m_climbLookDownIncrement = m_climbLookDown*2.0f/(m_climbLookDownReadjustNumFrames*m_climbLookDownNumFramesFraction);
	m_climbLookReadjustIncrement = -1.0f*m_climbLookDown*2.0f/(m_climbLookDownReadjustNumFrames*m_climbLookReadjustNumFramesFraction);
	m_climbLookedDown = false;

	m_climbLookXHolder = 0.0f;
	m_climbLookRight = 45.0f;
	m_climbLookXsNumFrames = 200.0f;
	m_climbLookXNumFramesFraction = 0.5f;
	m_climbLookBackXNumFramesFraction = 0.5f;
	m_climbLookXIncrement = m_climbLookRight/(m_climbLookXsNumFrames*m_climbLookXNumFramesFraction);
	m_climbLookBackXIncrement = -1.0f*m_climbLookRight/(m_climbLookXsNumFrames*m_climbLookBackXNumFramesFraction);;
	m_climbLookedRight = false;

	m_triedToRun = false;

	m_wallRunLookUpAdjustNumFrames = 40.0f;
	m_wallRunLookUpReadjustNumFrames = 20.0f;

	m_wallRunLookXAdjustNumFrames = 40.0f;
	m_wallRunLookXReadjustNumFrames = 20.0f;

	m_wallRunAdjustedLookUp = false;
	m_wallRunReadjustedLookUp = false;
}

// TODO: check for collision detection, not just with heightmap
// TODO: check player center vs player feet
int Physics::applyGravity(AABB* player) {
	m_feetPlanted = false;
	int buildingId = -2;
	if (m_currentState != PhysicsStates::Climbing && m_currentState != PhysicsStates::PullingUp && m_currentState != PhysicsStates::HoldingEdge  && m_currentState != PhysicsStates::WallRunning) {
	
		int xIndex = Utilities::roundToInt(m_position.x+m_velocity.x);
		int zIndex = Utilities::roundToInt(m_position.z+m_velocity.z);

		//cout << "entering with position " << xIndex << ", " << zIndex << endl;
		//cout << "heightmap's value at this pos is : " << World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] << endl;
		///cout << "vs: " << m_position.y - m_gravityConstant << endl;*/
		
		m_velocity += m_gravity;
		
		// if landed on ground
		if (World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] > m_position.y + m_velocity.y) {
			m_position.y = World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift];
			m_feetPlanted = true;
			m_canJump = true;
			m_velocity.y = 0.0f; /* = glm::vec3(m_velocity.x, 0.0f, m_velocity.z); */
			m_velocityDiff.y = 0.0f;
			if (m_currentState != PhysicsStates::Sliding) {
				m_currentState = PhysicsStates::None;
			}
			sg::City* city;
	
			for (int i=0; i < client->root->getNumChildren(); i++) {
				city = dynamic_cast<sg::City*>(client->root->m_child[i]);
				if (city != nullptr) {
					break;
				}
			}

			bool tmp = false;
			for (int i = 0; i < city->getNumChildren(); i++) {
				sg::Building *b = dynamic_cast<sg::Building*>(city->m_child[i]);
				if (b != nullptr) {
					tmp = b->onTopOf(player);
				}
				if (tmp) {
					//cout << "landed on : " << i << endl;
					if (i == 40) {
						return -1;
					}
					//cout << "i: " << i << ", COLLISION DETECTED" << endl;
					//cout << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minX << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minY << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minZ << endl;
					//cout << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxX << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxY << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxZ << endl;
					//cout << "goTo: " << glm::to_string(goTo) << endl << endl;
					
					return i;
				}	
			}
		
		}
	}
	return buildingId;
}

bool Physics::atRest() {
	if (abs(m_lastMoved.x) < m_restConstant.x && abs(m_lastMoved.z) < m_restConstant.z) {
		return true;
	}
	return false;
}

void Physics::toggleTriedToRun() {
	m_triedToRun = true;
}

// TODO: make it so you move right into the wall even if presing w would make you go past it, not just not move at all if you would
// move into the wall
int Physics::handleCollisionDetection(AABB* other) {
	sg::City* city;
	
	for (int i=0; i < client->root->getNumChildren(); i++) {
		city = dynamic_cast<sg::City*>(client->root->m_child[i]);
		if (city != nullptr) {
			break;
		}
	}

	int tmp = -1;
	for (int i = 0; i < city->getNumChildren(); i++) {
		sg::Building *b = dynamic_cast<sg::Building*>(city->m_child[i]);
		if (b != nullptr) {
			tmp = b->collidesWith(other);
		}
		
		if (tmp != -1) {
			//cout << "i: " << i << ", COLLISION DETECTED" << endl;
			//cout << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minX << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minY << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minZ << endl;
			//cout << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxX << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxY << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxZ << endl;
			//cout << "goTo: " << glm::to_string(goTo) << endl << endl;
			return tmp;
		}	
	}

	return -1;
}

float Physics::handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other, int buildingId) {
	sg::City* city;
	float angle = -1.0f;
	for (int i=0; i < client->root->getNumChildren(); i++) {
		city = dynamic_cast<sg::City*>(client->root->m_child[i]);
		if (city != nullptr) {
			break;
		}
	}

	int tmp = false;
	sg::Building *b = dynamic_cast<sg::Building*>(city->m_child[buildingId]);
	if (b != nullptr) {
		tmp = b->collidesWith(other);
	}
		
	if (tmp != -1) {

		float angle = b->angleIntersection(from, goTo);
		return angle;
	}	
	return angle;
}

int Physics::handleReflectionIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other, int buildingId) {
	sg::City* city;
	int newDirection = -1;
	for (int i=0; i < client->root->getNumChildren(); i++) {
		city = dynamic_cast<sg::City*>(client->root->m_child[i]);
		if (city != nullptr) {
			break;
		}
	}

	int tmp = -1;
	sg::Building *b = dynamic_cast<sg::Building*>(city->m_child[buildingId]);
	if (b != nullptr) {
		tmp = b->collidesWith(other);
	}
		
	if (tmp != -1) {
		newDirection = b->reflectionIntersection(from, goTo);
		return newDirection;
	}	
	return newDirection;
}

bool Physics::handleNearTop(glm::vec3 from, int buildingId) {
	sg::City* city;
	int newDirection = -1;
	for (int i=0; i < client->root->getNumChildren(); i++) {
		city = dynamic_cast<sg::City*>(client->root->m_child[i]);
		if (city != nullptr) {
			break;
		}
	}

	sg::Building *b = dynamic_cast<sg::Building*>(city->m_child[buildingId]);
	if (b != nullptr) {
		bool nearTop = b->nearTop(from);
		return nearTop;
	}
	return false;
}


bool Physics::handleClearedTop(AABB *other, int buildingId) {
	sg::City* city;
	int newDirection = -1;
	for (int i=0; i < client->root->getNumChildren(); i++) {
		city = dynamic_cast<sg::City*>(client->root->m_child[i]);
		if (city != nullptr) {
			break;
		}
	}

	sg::Building *b = dynamic_cast<sg::Building*>(city->m_child[buildingId]);
	if (b != nullptr) {
		bool clearedTop = b->clearedTop(other);
		return clearedTop;
	}

	return false;
}


void Physics::move(glm::vec3 delta) {
	m_position += delta;
}

void Physics::moveTo(glm::vec3 pos) {
	m_position = pos;
}
