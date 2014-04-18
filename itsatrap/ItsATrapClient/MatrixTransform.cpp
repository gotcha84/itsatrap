#include "MatrixTransform.h"

namespace sg {

	MatrixTransform::MatrixTransform() {
		m_trans = glm::mat4();
	}

	MatrixTransform::MatrixTransform(glm::mat4 m) {
		m_trans = glm::mat4(m);
	}

	MatrixTransform::~MatrixTransform() {

	}

	void MatrixTransform::setMatrix(glm::mat4 m) {
		m_trans = glm::mat4(m);
	}

	glm::mat4 MatrixTransform::getMatrix() {
		return m_trans;
	}

	void MatrixTransform::draw() {
		for (int i=0; i<m_nChild; i++) {
			m_child[i]->draw(m_trans);
		}
	}

	void MatrixTransform::draw(glm::mat4 parent) {
		glm::mat4 product = parent * m_trans;

		for (int i=0; i<m_nChild; i++) {
			m_child[i]->draw(product);
		}
	}

}