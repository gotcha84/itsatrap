#define TRAP_RAD 5.0f

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
		m_boundingBox = AABB(m_position, TRAP_RAD);
	}
	
	// TODO - implement when traps have different types
	Trap::Trap(glm::vec3 currPos, int type) {
	}

	Trap::~Trap() {
	}
	
	void Trap::updateBoundingBox() {
		this->setBoundingBox(this->getPosition(), TRAP_RAD);
	}

	void Trap::setPosition(glm::vec3 pos) {
		m_position = pos;
		this->updateBoundingBox();
	}

	glm::vec3 Trap::getPosition() {
		return m_position;
	}

	void Trap::setMatrix(glm::mat4 model) {
		m_model = model;
	}

	glm::mat4 Trap::getMatrix() {
		return m_model;
	}

	void Trap::setColor(glm::vec3 color) {
		m_color = color;
	}

	glm::vec3 Trap::getColor() {
		return m_color;
	}

	void Trap::draw(glm::mat4 parent, glm::mat4 cam) {
		this->setMatrix(glm::translate(this->getPosition()) * glm::scale(glm::vec3(1.0f, 0.5f, 1.0f)));

		glm::mat4 new_model = parent * this->getMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor3f(this->getColor().x, this->getColor().y, this->getColor().z);
			glutSolidCube(5);
		glPopMatrix();
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