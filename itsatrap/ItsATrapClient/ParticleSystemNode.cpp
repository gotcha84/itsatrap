#include "ParticleSystemNode.h"

namespace sg {

	ParticleSystemNode::ParticleSystemNode(int numParticles) {
	}

	ParticleSystemNode::~ParticleSystemNode() {
	}

	void ParticleSystemNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glPopMatrix();
	}

	void ParticleSystemNode::print() {
		cout << "(" << this->getObjectID() << " PS Node: " << this->getName() << ")";
	}
}