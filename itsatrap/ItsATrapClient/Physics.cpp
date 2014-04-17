#include <math.h>

#include "Physics.h"

using namespace std;

Physics::Physics() {

	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	
	m_gravityConstant = -10.0f; 
	m_elasticityConstant = 0.9f;

}

void Physics::applyGravity() {
	int xIndex = Utilities::closestInt(m_position.x);
	int yIndex = Utilities::closestInt(m_position.y);

	/*if (World::m_heightMap[xIndex][yIndex] > m_position.y - m_gravityConstant) {
		m_position = glm::vec3(m_position.x, World::m_heightMap[xIndex][yIndex], m_position.z);
		m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else {
		m_position = glm::vec3(m_position.x, m_position.x - m_gravityConstant, m_position.z)
	}*/
}



