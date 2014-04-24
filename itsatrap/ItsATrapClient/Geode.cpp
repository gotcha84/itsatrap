#include "Geode.h"

namespace sg {

	Geode::Geode() {
		color = glm::vec3(1,0,0);
	}

	Geode::~Geode() {

	}

	void Geode::print() {
		cout << "(" << this->getObjectID() << " Geode: " << this->getName() << ")";
	}
}