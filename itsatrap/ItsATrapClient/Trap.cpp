#define TRAP_RAD 5.0f

#include "Trap.h"

namespace sg {

	Trap::Trap(glm::vec3 currPos) {
		m_position = currPos;
		m_model = glm::translate(currPos);
		this->setColor(glm::vec4(1,0,0,1));

		this->initModel(m_model1);
	}

	Trap::Trap(int ownerId, glm::vec3 currPos) {
		m_ownerId = ownerId;
		m_position = currPos;
		m_model = glm::translate(currPos);
		this->setColor(glm::vec4(1,0,0,1));

		this->initModel(m_model1);
	}
	
	// TODO - implement when traps have different types
	Trap::Trap(glm::vec3 currPos, int type) {

	}

	Trap::~Trap() {
		delete m_model1;
		m_model1 = nullptr;
	}
	
	void Trap::initModel(ObjModel *model) {
		model = new ObjModel("Can.obj");
		model->setColor(this->getColor());
	}

	void Trap::updateBoundingBox() {
		this->m_model1->setBoundingBox(this->getPosition(), TRAP_RAD);
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

	void Trap::draw(glm::mat4 parent, glm::mat4 cam) {
		this->setMatrix(glm::translate(this->getPosition()) * glm::scale(glm::vec3(1.0f, 0.5f, 1.0f)));

		glm::mat4 new_model = parent * this->getMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			glutSolidCube(5);
		glPopMatrix();
	}

	void Trap::print() {
		cout << "(" << this->m_model1->getObjectID() << " Trap: " << this->m_model1->getName() << ")";
	}

	struct trapObject Trap::getTrapObjectForNetworking() {
		struct trapObject t = {};
		t.ownerId = m_ownerId;
		t.eventCode = 0;
		t.x = this->getPosition().x;
		t.y = this->getPosition().y;
		t.z = this->getPosition().z;
		t.aabb.minX = this->m_model1->getBoundingBox().m_minX;
		t.aabb.minY = this->m_model1->getBoundingBox().m_minY;
		t.aabb.minZ = this->m_model1->getBoundingBox().m_minZ;
		t.aabb.maxX = this->m_model1->getBoundingBox().m_maxX;
		t.aabb.maxY = this->m_model1->getBoundingBox().m_maxY;
		t.aabb.maxZ = this->m_model1->getBoundingBox().m_maxZ;
		return t;
	}
}