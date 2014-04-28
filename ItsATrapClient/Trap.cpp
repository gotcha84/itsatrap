#include "Trap.h"

namespace sg {

	Trap::Trap() {
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_model = glm::translate(m_position);
		m_color = glm::vec3(1,0,0);
		m_boundingBox = new AABB(m_position, 5.0f);
	}

	Trap::Trap(glm::vec3 pos) {
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

	struct dynamicObject Trap::getDynamicObject()
	{
		struct dynamicObject obj = {};
		obj.objectId = -1;
		obj.type = TYPE_TRAP;

		struct objectProperty prop = {};
		prop.x = this->getPosition().x;
		prop.y = this->getPosition().y;
		prop.z = this->getPosition().z;
		prop.minX = this->getBoundingBox()->m_minX;
		prop.minY = this->getBoundingBox()->m_minY;
		prop.minZ = this->getBoundingBox()->m_minZ;
		prop.maxX = this->getBoundingBox()->m_maxX;
		prop.maxY = this->getBoundingBox()->m_maxY;
		prop.maxZ = this->getBoundingBox()->m_maxZ;

		obj.prop = prop;

		return obj;
	}
}