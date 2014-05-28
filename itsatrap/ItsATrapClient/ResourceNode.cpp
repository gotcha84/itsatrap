#include "ResourceNode.h"

namespace sg {

	ResourceNode::ResourceNode(int id, int numParticles)
		: ObjNode() {
		m_particles = new ParticleSystem(numParticles);
		m_particles2 = new ParticleSystem(numParticles);

		resourceId = id;
		owner = -1;
		isActive = false;
		isChanneling = false;
	}

	ResourceNode::~ResourceNode() {
		delete m_particles;
		m_particles = nullptr;

		delete m_particles2;
		m_particles2 = nullptr;
	}

	ParticleSystem *ResourceNode::getParticleSystem() {
		return m_particles;
	}

	void ResourceNode::setParticleSystem(ParticleSystem *p) {
		m_particles = p;
	}

	void ResourceNode::enableParticles() {
		m_particles->enable();
		m_particles2->enable();
	}

	void ResourceNode::disableParticles() {
		m_particles->disable();
		m_particles2->disable();
	}

	int ResourceNode::getResourceId() {
		return resourceId;
	}

	void ResourceNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(mv));

		glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
		m_model->drawModel();
		m_particles->draw();
		m_particles2->draw();
		glPopMatrix();
	}

	void ResourceNode::print() {
		cout << "(" << this->getObjectID() << " Resource: " << this->getName() << ")";
	}

}