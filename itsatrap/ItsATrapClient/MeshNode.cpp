#include "MeshNode.h"

namespace sg {
	MeshNode::MeshNode() {
		m_model = new Mesh();
	}

	MeshNode::MeshNode(std::string filename) {
		m_model = new Mesh();
		m_model->LoadMesh(filename);
	}

	MeshNode::~MeshNode() {

	}

	void MeshNode::loadModel(std::string filename) {

	}

	Mesh *MeshNode::loadModel() {

	}

	void MeshNode::draw(glm::mat4 parent, glm::mat4 cam) {

	}

	void MeshNode::print() {

	}
}