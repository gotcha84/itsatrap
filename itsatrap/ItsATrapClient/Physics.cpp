#include "Physics.h"
#include "ClientInstance.h"

extern ClientInstance *client;

Physics::Physics() {

	m_currentState = PhysicsStates::None;
	
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(75.0f, 0.0f, 0.0f);
	m_lastMoved = glm::vec3(0.0f, 0.0f, 0.0f);

	m_yJumpFactor = 2.0f;

	m_gravityConstant = -0.1f;
	m_gravity  = glm::vec3(0.0f, m_gravityConstant, 0.0f);
	m_elasticityConstant = 0.9f;

	m_restConstant = glm::vec3(0.01f, 0.01f, 0.01f);
	m_lastTeleported = clock();
	m_lastSlid = clock();
	m_teleportDelay = 5.0f;
	m_slideDelay = 0.5f;
}

Physics::Physics(glm::vec3 pos) {

	//cout << "pos: " << glm::to_string(pos) << endl;
	m_currentState = PhysicsStates::None;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(pos.x, pos.y, pos.z);
	m_lastMoved = glm::vec3(0.0f, 0.0f, 0.0f);

	m_yJumpFactor = 2.0f;
	
	m_gravityConstant = -0.1f; 
	m_gravity  = glm::vec3(0.0f, m_gravityConstant, 0.0f);
	m_elasticityConstant = 0.9f;

	m_restConstant = glm::vec3(0.01f, 0.01f, 0.01f);
	m_lastTeleported = clock();
	m_lastSlid = clock();
	m_teleportDelay = 5.0f;
	m_slideDelay = 0.5f;
}

Physics::Physics(glm::vec3 pos, float mass) {

	//cout << "pos: " << glm::to_string(pos) << endl;
	m_currentState = PhysicsStates::None;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(pos.x, pos.y, pos.z);
	m_lastMoved = glm::vec3(0.0f, 0.0f, 0.0f);
	
	m_yJumpFactor = 2.0f;

	m_gravityConstant = -0.1f; 
	m_gravity  = glm::vec3(0.0f, m_gravityConstant, 0.0f);
	m_elasticityConstant = 0.9f;

	m_restConstant = glm::vec3(0.01f, 0.01f, 0.01f);
	m_lastTeleported = clock();
	m_teleportDelay = 5.0f;
	m_slideDelay = 0.5f;

	m_mass = mass;
}

Physics::~Physics() {

}


// TODO: check for collision detection, not just with heightmap
void Physics::applyGravity() {

	if (m_currentState != WallJumping) {
	
		int xIndex = Utilities::roundToInt(m_position.x+m_velocity.x);
		int zIndex = Utilities::roundToInt(m_position.z+m_velocity.z);

		//TODO add collision detection for gravity

		//cout << "entering with position " << xIndex << ", " << zIndex << endl;
		//cout << "heightmap's value at this pos is : " << World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] << endl;
		///cout << "vs: " << m_position.y - m_gravityConstant << endl;*/
		
		// TODO: possibly check more cases
		//if (m_currentState != PhysicsStates::Falling) {
			//cout << "position before: " << glm::to_string(m_position) << endl;
			//cout << "velocity before: " << glm::to_string(m_velocity) << endl;
			//cout << "gravity: " << glm::to_string(m_gravity) << endl;

			//m_position += m_velocity;
		m_velocity += m_gravity;
	
		/*cout << "position after: " << glm::to_string(m_position) << endl;
		cout << "velocity after: " << glm::to_string(m_velocity) << endl;
		cout << "gravity: " << glm::to_string(m_gravity) << endl << endl;*/

		// TODO - uncomment once height map working with building node
		// if landed on ground
		if (World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] > m_position.y + m_velocity.y) {
			m_position.y = World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift];
			// TODO: use velocity
			
			m_velocity.y = 0.0f; /* = glm::vec3(m_velocity.x, 0.0f, m_velocity.z); */
			m_velocityDiff.y = 0.0f;
			if (m_currentState != PhysicsStates::Sliding) {
				m_currentState = PhysicsStates::None;
			}
		
		}
	}
		/*if (m_velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
			cout << "velocity: " << glm::to_string(m_velocity) << endl;
		}*/
		//else {
			//m_position = glm::vec3(m_position.x, m_position.y + m_gravityConstant, m_position.z);
			//m_currentState = PhysicsStates::Falling;
		//}
	
	
	//}

	//cout << "exiting with position: " << glm::to_string(m_position) << endl << endl;

	//m_position = glm::vec3(m_position.x, World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift], m_position.z);
}



bool Physics::atRest() {
	//cout << "lastmoved: " << glm::to_string(m_lastMoved) << endl;
	//if (abs(m_lastMoved.x) < m_restConstant.x && abs(m_lastMoved.y-1.9f) < m_restConstant.y && abs(m_lastMoved.z) < m_restConstant.z) {
	//	return true;
	//}
	if (abs(m_lastMoved.x) < m_restConstant.x && abs(m_lastMoved.z) < m_restConstant.z) {
		return true;
	}
	return false;
}

// TODO: make it so you move right into the wall even if presing w would make you go past it, not just not move at all if you would
// move into the wall
glm::vec3 Physics::handleCollisionDetection(glm::vec3 goTo) {
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
			tmp = b->isInside(goTo);
		}
		
		if (tmp) {
			//cout << "i: " << i << ", COLLISION DETECTED" << endl;
			//cout << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minX << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minY << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_minZ << endl;
			//cout << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxX << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxY << ", " << ((sg::Building*)city->m_child[i])->m_boundingBox.m_maxZ << endl;
			//cout << "goTo: " << glm::to_string(goTo) << endl << endl;
			return m_position;
		}	
	}

	return goTo;
}

glm::vec3 Physics::handleIntersection(glm::vec3 from, glm::vec3 goTo) {
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
			tmp = b->isInside(goTo);
		}
		
		if (tmp) {
			glm::vec3 intersectionPoint = b->intersects(from, goTo);

			float angle = b->angleIntersection(from, goTo);
			cout << "angle: " << angle << endl;

			bool nearTop = b->nearTop(goTo);
			cout << "near top: " << nearTop << endl;
			return m_position;
		}	
	}
	return goTo;
}

void Physics::move(glm::vec3 delta) {
	m_position += delta;
}

void Physics::moveTo(glm::vec3 pos) {
	m_position = pos;
}
