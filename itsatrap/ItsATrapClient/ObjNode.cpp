#include "ObjNode.h"

namespace sg {
	ObjNode::ObjNode() {
		m_model = new ObjModel();
	}

	ObjNode::ObjNode(std::string filename) {
		m_model = new ObjModel();
		m_model->loadModel(filename);
	}

	ObjNode::ObjNode(std::string objFilename, std::string mtlFilename) {
		m_model = new ObjModel();

		m_model->loadModel(objFilename, mtlFilename);
	}

	ObjNode::~ObjNode() {
		delete m_model;
		m_model = nullptr;
	}

	void ObjNode::loadModel(std::string objFilename) {
		m_model->loadModel(objFilename);
	}

	void ObjNode::loadModel(std::string objFilename, std::string mtlFilename) {
		m_model->loadModel(objFilename, mtlFilename);
	}

	ObjModel *ObjNode::getModel() {
		return m_model;
	}

	AABB ObjNode::getBoundingBox() {
		return m_model->getBoundingBox();
	}

	void ObjNode::calculateBoundingBox() {
		m_model->calculateBoundingBox(this->getWorldTransformMatrix());
		//m_model->getBoundingBox().print();
	}

	void ObjNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			m_model->drawModel();
		glPopMatrix();
	}

	void ObjNode::print() {
		cout << "(" << this->getObjectID() << " OBJ: " << this->getName() << ")";
	}
}
