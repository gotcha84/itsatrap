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

	void ObjNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			if (this->getName() == "skybox") {
				glPushAttrib(GL_ENABLE_BIT);
					//glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
					
					m_model->drawModel();

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glEnable(GL_CULL_FACE);
					//glEnable(GL_DEPTH_TEST);
				glPopAttrib();
			}
			else {
				m_model->drawModel();
			}
		glPopMatrix();
	}

	void ObjNode::print() {
		cout << "(" << this->getObjectID() << " OBJ: " << this->getName() << ")";
	}
}
