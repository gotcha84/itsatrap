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

	// TODO : implement base class
	//void Geode::calculateBoundingBox() {

	//}

	// TODO: implement base class
	//bool Geode::isInside(glm::vec3 goTo) {
	//	cout << "geode isInside" << endl;
	//	return false;
	//}

}