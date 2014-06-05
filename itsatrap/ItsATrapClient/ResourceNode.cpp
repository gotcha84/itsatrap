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

	void ResourceNode::setParticleSystemOrigin(glm::vec3 origin) {
		m_particles->setOrigin(origin);
		m_particles2->setOrigin(origin);

		m_particles->reset();
		m_particles2->reset();
		m_particles2->reverse();
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
		bool team1 = false;
		float r, g, b, a;

		if (playerId % 2 == 0) {
			team1 = true;
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

		enableParticles();

		this->m_model->setColor(glm::vec4(r, g, b, a));
		this->m_particles->setColor(glm::vec4(r + 0.2f, g + 0.2f, b + 0.2f, a));
		this->m_particles2->setColor(glm::vec4(r + 0.2f, g + 0.2f, b + 0.2f, a));
	}

	void ResourceNode::setInactiveColor()
	{
		this->m_model->setColor(glm::vec4(0.2f, 0.2f, 0.2f, 1));
		this->m_particles->setColor(glm::vec4(0.7f, 0.7f, 0.7f, 1));
		this->m_particles2->setColor(glm::vec4(0.9f, 0.9f, 0.9f, 1));
	}

	void ResourceNode::setActiveColor()
	{
		this->m_model->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1));
		this->m_particles->setColor(glm::vec4(0.7f, 0.7f, 0.7f, 1));
		this->m_particles2->setColor(glm::vec4(0.9f, 0.9f, 0.9f, 1));
	}

	void ResourceNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(mv));

		glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
		m_model->drawModel();

		glDisable(GL_LIGHTING);
		m_particles->draw();
		m_particles2->draw();
		glEnable(GL_LIGHTING);

		glPopMatrix();
	}

	void ResourceNode::print() {
		cout << "(" << this->getObjectID() << " Resource: " << this->getName() << ")";
	}

}