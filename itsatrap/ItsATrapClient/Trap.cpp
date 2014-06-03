#define TRAP_RAD 5.0f

#include "Trap.h"

namespace sg {

	Trap::Trap(glm::vec3 currPos) {
		//m_position = currPos;
		//m_model = glm::translate(currPos);
		//this->m_model1->setColor(glm::vec4(0.8f, 0.8f, 0.8f, 0.8f));
		//// this->updateBoundingBox();
		//this->m_model1->calculateBoundingBox();
	}

	Trap::Trap(int ownerId, glm::vec3 currPos, float rotationAngle, string filename) {
		m_ownerId = ownerId;
		m_position = currPos;
		m_model = glm::translate(currPos);
		this->rotationAngle = rotationAngle;
		
		// this->updateBoundingBox();
		this->m_model1 = new ObjModel(filename);
		this->m_model1->setColor(glm::vec4(0.8f, 0.8f, 0.8f, 0.5f));
		this->m_model1->calculateBoundingBox();
	}
	
	// TODO - implement when traps have different types
	Trap::Trap(glm::vec3 currPos, int type) {

	}

	Trap::~Trap() {
		delete m_model1;
		m_model1 = nullptr;
	}
	
	void Trap::loadModel(std::string objFilename) {
		m_model1 = new ObjModel(objFilename, m_position);
		m_model1->setColor(this->getColor());
	}

	void Trap::loadModel(std::string objFilename, std::string mtlFilename) {
		m_model1 = new ObjModel(objFilename, mtlFilename, m_position);
		m_model1->setColor(this->getColor());
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

		float polyTrapScaleX = 0.0f;
		ConfigSettings::getConfig()->getValue("polyTrapScaleX", polyTrapScaleX);

		float polyTrapScaleY = 0.0f;
		ConfigSettings::getConfig()->getValue("polyTrapScaleY", polyTrapScaleY);

		float polyTrapScaleZ = 0.0f;
		ConfigSettings::getConfig()->getValue("polyTrapScaleZ", polyTrapScaleZ);

		glm::mat4 new_model = glm::translate(this->getPosition()) * glm::scale(glm::vec3(polyTrapScaleX, polyTrapScaleY, polyTrapScaleZ)) * Utilities::rotateY(rotationAngle + 180.0f);
		this->setMatrix(new_model);

		new_model = parent * this->getMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			this->m_model1->draw(new_model, cam);
			//glutSolidCube(5);
		glPopMatrix();
	}

	void Trap::print() {
		cout << "(" << this->m_model1->getObjectID() << " Trap: " << this->m_model1->getName() << ")";
	}

	struct trapObject Trap::getTrapObjectForNetworking() {

		float polyTrapScaleX = 0.0f;
		ConfigSettings::getConfig()->getValue("polyTrapScaleX", polyTrapScaleX);

		float polyTrapScaleY = 0.0f;
		ConfigSettings::getConfig()->getValue("polyTrapScaleY", polyTrapScaleY);

		float polyTrapScaleZ = 0.0f;
		ConfigSettings::getConfig()->getValue("polyTrapScaleZ", polyTrapScaleZ);

		struct trapObject t = {};
		t.ownerId = m_ownerId;
		t.eventCode = 0;
		t.pos = this->getPosition();
		t.aabb.minX = this->m_model1->getBoundingBox().minX*polyTrapScaleX + this->getPosition().x;
		t.aabb.minY = this->m_model1->getBoundingBox().minY*polyTrapScaleY + this->getPosition().y;
		t.aabb.minZ = this->m_model1->getBoundingBox().minZ*polyTrapScaleZ + this->getPosition().z;
		t.aabb.maxX = this->m_model1->getBoundingBox().maxX*polyTrapScaleX + this->getPosition().x;
		t.aabb.maxY = this->m_model1->getBoundingBox().maxY*polyTrapScaleY + this->getPosition().y;
		t.aabb.maxZ = this->m_model1->getBoundingBox().maxZ*polyTrapScaleZ + this->getPosition().z;
		t.rotationAngle = rotationAngle;
		return t;
	}
}