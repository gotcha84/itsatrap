#include "ParticleSystem.h"

Particle::Particle() {
	init();
}

Particle::Particle(glm::vec3 origin) {
	init();

	m_origin = origin;
	m_pos = m_origin;

	initAge();
}

Particle::Particle(glm::vec3 origin, int life) {
	init();

	m_origin = origin;
	m_pos = m_origin;
	m_life = life;

	initAge();
}

Particle::~Particle() {

};

void Particle::init() {
	id = 0;
	m_origin = glm::vec3(0, 0, 0);
	m_pos = m_origin;
	m_vel = glm::vec3(0, 0, 0);
	m_color = glm::vec4(1, 1, 1, 1);
	m_age = 0;
	m_life = 100;
}

void Particle::initAge() {
	srand(time(NULL));
	m_age = rand() % m_life + 1;
}

int Particle::getID() {
	return id;
}

void Particle::step() {
	m_age++; // happy birthday particle! age++
	
	if (m_age > m_life) {
		m_age = 0;
		m_pos = m_origin;
	}
	else {
		m_vel = glm::vec3(0,0.5f,0);
		m_pos += m_vel;
	}
}

void Particle::draw() {
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10);

	glBegin(GL_POINTS);
		glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);
		glVertex3f(m_pos.x, m_pos.y, m_pos.z);
	glEnd();
}

ParticleSystem::ParticleSystem(int numParticles) {
	m_numParticles = numParticles;
	initParticles();
};

ParticleSystem::~ParticleSystem() {
	m_particles.clear();
};

void ParticleSystem::initParticles() {
	m_particles.clear();
	m_particles.resize(m_numParticles);
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i] = Particle(m_origin);
		m_particles[i].id = i;
	}
}

int ParticleSystem::getNumParticles() {
	return m_numParticles;
}

void ParticleSystem::setNumParticles(int numParticles) {
	m_numParticles = numParticles;
}

int ParticleSystem::getMaxLifetime() {
	return m_maxLifetime;
}

void ParticleSystem::setMaxLifetime(int life) {
	m_maxLifetime = life;
}

void ParticleSystem::setOrigin(glm::vec3 origin) {
	m_origin = origin;
}

glm::vec3 ParticleSystem::getOrigin() {
	return m_origin;
}

void ParticleSystem::reset() {
	initParticles();
}

void ParticleSystem::step() {
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i].step();
	}
}

void ParticleSystem::draw() {
	if (m_enabled) {
		for (int i = 0; i < m_numParticles; i++) {
			m_particles[i].draw();
		}

		step();
	}
}

bool ParticleSystem::isEnabled() {
	return m_enabled;
}

void ParticleSystem::enable() {
	m_enabled = true;
}

void ParticleSystem::disable() {
	m_enabled = false;
}