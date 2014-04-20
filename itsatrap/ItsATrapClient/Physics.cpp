#include <math.h>

#include "Physics.h"

using namespace std;

Physics::Physics() {

	m_currentState = PhysicsStates::None;

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(0.0f, 4.0f, 0.0f);
	
	m_gravityConstant = -0.1f; 
	m_elasticityConstant = 0.9f;

}

void Physics::applyGravity() {

	
	int xIndex = Utilities::roundToInt(m_position.x);
	int zIndex = Utilities::roundToInt(m_position.z);

	/*cout << "entering with position " << xIndex << ", " << zIndex << endl;
	cout << "heightmap's value at this pos is : " << World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift] << endl;
	cout << "vs: " << m_position.y - m_gravityConstant << endl;*/

	// TODO: possibly check more cases
	//if (m_currentState != PhysicsStates::Falling) {
	
	
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
	//}

	//cout << "exiting with position: " << glm::to_string(m_position) << endl << endl;

	//m_position = glm::vec3(m_position.x, World::m_heightMap[xIndex+World::m_heightMapXShift][zIndex+World::m_heightMapZShift], m_position.z);
}



