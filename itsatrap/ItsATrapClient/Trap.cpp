#include "Trap.h"

namespace sg {

	Trap::Trap() {
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_model = glm::translate(m_position);
		m_color = glm::vec3(1,0,0);
		m_boundingBox = new AABB(m_position, 5.0f);
	}

	Trap::Trap(int ownerId, glm::vec3 pos) {
		m_ownerId = ownerId;
		m_position = pos;
		m_model = glm::translate(m_position);
		m_color = glm::vec3(1,0,0);
		m_boundingBox = new AABB(m_position, 5.0f);
	}

	Trap::~Trap() {
		delete m_boundingBox;
		m_boundingBox = nullptr;
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

	void Trap::setBoundingBox(AABB *box) {
		m_boundingBox = box;
	}

	AABB *Trap::getBoundingBox() {
		return m_boundingBox;
	}

	void Trap::updateBoundingBox() {
		m_boundingBox->setAABB(this->getPosition(), 5.0f);
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
		t.x = getPosition().x;
		t.y = getPosition().y;
		t.z = getPosition().z;
		t.aabb.minX = getBoundingBox()->m_minX;
		t.aabb.minY = getBoundingBox()->m_minY;
		t.aabb.minZ = getBoundingBox()->m_minZ;
		t.aabb.maxX = getBoundingBox()->m_maxX;
		t.aabb.maxY = getBoundingBox()->m_maxY;
		t.aabb.maxZ = getBoundingBox()->m_maxZ;
		return t;
	}
}