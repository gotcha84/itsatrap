#include "Trap.h"

namespace sg {

	Trap::Trap(glm::vec3 currPos) {
		m_position = currPos;
		m_objFilename = "Can.obj";
		m_model = glm::translate(currPos);
		m_color = glm::vec3(1,0,0);
		m_turtleScale = 0.01f;
		m_cityScale = 0.1f;
		m_canScale = 5.0f;
		m_defaultScale = 1.0f;

		if (m_objFilename == "Can.obj") {
			m_scaleVec = glm::vec3(m_canScale, m_canScale, m_canScale);
		}
		else {
			m_scaleVec = glm::vec3(m_defaultScale, m_defaultScale, m_defaultScale);
		}

		m_physics = Physics(currPos, FLT_MAX);
		
		loadData();
	}

	Trap::Trap(int ownerId, glm::vec3 pos) {
		m_ownerId = ownerId;
		m_position = pos;
		m_model = glm::translate(m_position);
		m_color = glm::vec3(1,0,0);
		m_boundingBox = AABB(m_position, 5.0f);
	}
	
	// TODO - implement when traps have different types
	Trap::Trap(glm::vec3 currPos, int type) {
	}

	Trap::~Trap() {
	}
	
	void Trap::print() {
		cout << "(" << this->getObjectID() << " Trap: " << this->getName() << ")";
	}

	struct trapObject Trap::getTrapObjectForNetworking()
	{
		struct trapObject t = {};
		t.ownerId = m_ownerId;
		t.eventCode = 0;
		t.x = this->getPosition().x;
		t.y = this->getPosition().y;
		t.z = this->getPosition().z;
		t.aabb.minX = this->getBoundingBox().m_minX;
		t.aabb.minY = this->getBoundingBox().m_minY;
		t.aabb.minZ = this->getBoundingBox().m_minZ;
		t.aabb.maxX = this->getBoundingBox().m_maxX;
		t.aabb.maxY = this->getBoundingBox().m_maxY;
		t.aabb.maxZ = this->getBoundingBox().m_maxZ;
		return t;
	}
}