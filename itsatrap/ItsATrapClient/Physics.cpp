#include "Physics.h"
#include "ClientInstance.h"

extern ClientInstance *client;

Physics::Physics() {

	m_currentState = PhysicsStates::None;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(75.0f, 0.0f, 0.0f);
	
	m_gravityConstant = -0.1f; 
	m_elasticityConstant = 0.9f;
}

Physics::Physics(glm::vec3 pos) {

	cout << "pos: " << glm::to_string(pos) << endl;
	m_currentState = PhysicsStates::None;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(pos.x, pos.y-4.0f, pos.z);
	
	m_gravityConstant = -0.1f; 
	m_elasticityConstant = 0.9f;
}

Physics::~Physics() {

}


// TODO: check for collision detection, not just with heightmap
void Physics::applyGravity() {
	int xIndex = Utilities::roundToInt(m_position.x);
	int zIndex = Utilities::roundToInt(m_position.z);

	/*cout << "entering with position " << xIndex << ", " << zIndex << endl;
	cout << "heightmap's value at this pos is : " << World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] << endl;
	cout << "vs: " << m_position.y - m_gravityConstant << endl;*/

	// TODO: possibly check more cases
	//if (m_currentState != PhysicsStates::Falling) {
	/*
	
	// TODO - uncomment once height map working with building node
	if (World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] > m_position.y + m_gravityConstant) {
		m_position = glm::vec3(m_position.x, World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift], m_position.z);
		// TODO: use velocity
		m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		m_currentState = PhysicsStates::None;
		
		
	}
	else {
		m_position = glm::vec3(m_position.x, m_position.y + m_gravityConstant, m_position.z);
		m_currentState = PhysicsStates::Falling;
	}
	
	*/
	//}

	//cout << "exiting with position: " << glm::to_string(m_position) << endl << endl;

	//m_position = glm::vec3(m_position.x, World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift], m_position.z);
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

