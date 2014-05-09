#include "Geode.h"

namespace sg {

	Geode::Geode() {
		m_color = glm::vec4(1,0,0,1);
	}

	Geode::~Geode() {

	}

	void Geode::setPosition(glm::vec3 pos) {
		m_position = pos;
		//this->updateBoundingBox();
	}
	
	glm::vec3 Geode::getPosition() {
		return m_position;
	}

	void Geode::setMatrix(glm::mat4 model) {
		m_model = model;
	}

	glm::mat4 Geode::getMatrix() {
		return m_model;
	}

	void Geode::setColor(glm::vec4 color) {
		m_color = color;
	}

	glm::vec4 Geode::getColor() {
		return m_color;
	}

	AABB Geode::getBoundingBox() {
		return m_boundingBox;
	}

	void Geode::setBoundingBox(glm::vec3 pos, float rad) {
		m_boundingBox.setAABB(pos, rad);
	}

	void Geode::print() {
		cout << "(" << this->getObjectID() << " Geode: " << this->getName() << ")";
	}

	// TODO : implement base class
	void Geode::calculateBoundingBox() {

	}

	// TODO : implement base class
	bool Geode::isInside(glm::vec3 goTo) {
		cout << "geode isInside" << endl;
		return false;
	}

	// void Geode::collidesWith(Geode b) {

	// }
	
	void Geode::setMaterial() {

	}
}

