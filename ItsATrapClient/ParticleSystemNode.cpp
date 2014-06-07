#include "ParticleSystemNode.h"

namespace sg {

	ParticleSystemNode::ParticleSystemNode(int numParticles) {
		m_particles = new ParticleSystem2(numParticles);
	}

	ParticleSystemNode::~ParticleSystemNode() {
		delete m_particles;
		m_particles = nullptr;
	}

	void ParticleSystemNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			m_particles->draw(parent, cam);
		glPopMatrix();
	}

	void ParticleSystemNode::print() {
		cout << "(" << this->getObjectID() << " PS Node: " << this->getName() << ")";
	}
}