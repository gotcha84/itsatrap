#include "Geode.h"

Geode::Geode() {
	m_trans = glm::mat4();
}

Geode::Geode(glm::mat4  m) {
	m_trans = glm::mat4(m);
}

Geode::~Geode() {

}

void Geode::setMatrix(glm::mat4 m) {
	m_trans = glm::mat4(m);
}

glm::mat4 Geode::getMatrix() {
	return m_trans;
}

void Geode::draw(glm::mat4 parent) {
	glm::mat4 product = parent * m_trans;
	render(product);
}
