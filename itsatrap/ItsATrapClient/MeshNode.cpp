#include "MeshNode.h"

namespace sg {
	MeshNode::MeshNode() {
		m_mesh = new Mesh();
	}

	MeshNode::MeshNode(std::string filename) {
		m_mesh = new Mesh();
		m_mesh->LoadMesh(filename);
	}

	MeshNode::~MeshNode() {
		delete m_mesh;
		m_mesh = nullptr;
	}

	Mesh *MeshNode::getMesh() {
		return m_mesh;
	}

	void MeshNode::loadMesh(std::string filename) {
		m_mesh->LoadMesh(filename);
	}

	void MeshNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			m_mesh->draw();
		glPopMatrix();
	}

	void MeshNode::print() {
		cout << "(" << this->getObjectID() << " Mesh: " << this->getName() << ")";
	}
}