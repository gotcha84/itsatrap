#include "ResourceNode.h"

namespace sg {

	ResourceNode::ResourceNode(int numParticles) 
	: ObjNode() {
		m_particles = new ParticleSystem(numParticles);
	}

	ResourceNode::~ResourceNode() {
		delete m_particles;
		m_particles = nullptr;
	}

	ParticleSystem *ResourceNode::getParticleSystem() {
		return m_particles;
	}

	void ResourceNode::setParticleSystem(ParticleSystem *p) {
		m_particles = p;
	}

	void ResourceNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			m_model->drawModel();
			m_particles->draw();
		glPopMatrix();
	}

	void ResourceNode::print() {
		cout << "(" << this->getObjectID() << " Resource: " << this->getName() << ")";
	}
}