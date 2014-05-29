#include "ResourceNode.h"

namespace sg {

	ResourceNode::ResourceNode(int id, int numParticles)
		: ObjNode() {
		m_particles = new ParticleSystem(numParticles);
		m_particles2 = new ParticleSystem(numParticles);

		resourceId = id;
		owner = -1;
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

	void ResourceNode::setOwnerColor(int playerId)
	{
		float r, g, b, a;
		if (playerId % 2 == 0) {
			ConfigSettings::getConfig()->getValue("Team1R", r);
			ConfigSettings::getConfig()->getValue("Team1G", g);
			ConfigSettings::getConfig()->getValue("Team1B", b);
			ConfigSettings::getConfig()->getValue("Team1A", a);
		}
		else {
			ConfigSettings::getConfig()->getValue("Team2R", r);
			ConfigSettings::getConfig()->getValue("Team2G", g);
			ConfigSettings::getConfig()->getValue("Team2B", b);
			ConfigSettings::getConfig()->getValue("Team2A", a);
		}

		this->m_model->setColor(glm::vec4(r, g, b, a - 0.3));
		this->m_particles->setColor(glm::vec4(r, g, b, a));
		this->m_particles2->setColor(glm::vec4(r, g, b, a));
	}

	void ResourceNode::resetOwnerColor()
	{

		this->m_model->setColor(glm::vec4(0.9, 0.9, 0.9, 0.7));
		this->m_particles->setColor(glm::vec4(0.5, 0.5, 0.5, 1));
		this->m_particles2->setColor(glm::vec4(0.7, 0.7, 0.7, 1));
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