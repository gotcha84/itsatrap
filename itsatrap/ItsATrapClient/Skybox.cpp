#include "Skybox.h"

namespace sg {

	Skybox::Skybox()
		: ObjNode() {
	}

	Skybox::Skybox(std::string objFilename, std::string mtlFilename)
		: ObjNode(objFilename, mtlFilename) {

	}

	Skybox::~Skybox() {

	}

	void Skybox::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_CULL_FACE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glDisable(GL_LIGHTING);
			m_model->drawModel();
			glEnable(GL_LIGHTING);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glEnable(GL_CULL_FACE);
			glPopAttrib();
		glPopMatrix();
	}

	void Skybox::drawModel() {
		m_model->drawModel();
	}

	void Skybox::print() {
		cout << "(" << this->getObjectID() << " Skybox: " << this->getName() << ")";
	}
}