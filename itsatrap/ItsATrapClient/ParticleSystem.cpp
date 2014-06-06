#include "ParticleSystem.h"

Particle::Particle() {
	init();
	reset();
	initAge();
}

Particle::Particle(int id, glm::vec3 origin, glm::vec4 color) {
	srand(id);
	init();

	this->id = id;
	m_origin = origin;
	m_color = color;
	m_color.a = 1.0f;

	reset();
	initAge();
}

Particle::Particle(int id, glm::vec3 origin, int life, glm::vec4 color) {
	srand(id);
	init();

	this->id = id;
	m_origin = origin;
	m_life = life;
	m_color = color;
	m_color.a = 1.0f;

	reset();
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
	m_life = 4000;
	m_reversed = false;

	reset();
}

void Particle::initAge() {
	m_age = rand() % m_life + 1;
}

int Particle::getID() {
	return id;
}

void Particle::reset() {
	m_age = 0;
	m_theta = 0;
	if (m_reversed) {
		m_rad = ((rand() % 100 + 1) * -0.0025f) - 0.25;
	}
	else {
		m_rad = ((rand() % 100 + 1) * 0.0025f) + 0.25;
	}
	m_pos = m_origin + glm::vec3(m_rad, 0, 0);
	m_vel = glm::vec3(0, 0.1f, 0.0f);
	m_color.a = 1.0f;
}

void Particle::step() {
	m_age++; // happy birthday particle! age++

	if (m_age > m_life) {
		reset();
	}
	else {
		m_vel = glm::vec3(m_rad, 0.2f, 0);
		m_vel = glm::rotateY(m_vel, m_theta);

		m_color.a = 1.0f - m_pos.y / (0.1f*m_life); // turn particles black toward the top

		m_pos += m_vel;
		if (m_reversed) {
			m_theta -= 1.0f;
			m_rad -= 0.1f / m_life;
		}
		else {
			m_theta += 1.0f;
			m_rad += 0.1f / m_life;
		}
	}
}

void Particle::draw(glm::mat4 parent, glm::mat4 cam) {
	//glEnable(GL_POINT_SMOOTH);
	//glPointSize(10);

	glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);
	//glBegin(GL_POINTS);
	//glVertex3f(m_pos.x, m_pos.y, m_pos.z);
	//glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadMatrixf(glm::value_ptr(parent * glm::inverse(cam)));
		glTranslatef(m_pos.x, m_pos.y, m_pos.z);
		glutSolidCube(1);
	glPopMatrix();

	//glDisable(GL_POINT_SMOOTH);
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
		m_particles[i] = Particle(i, m_origin, m_color);
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

glm::vec3 ParticleSystem::getOrigin() {
	return m_origin;
}

void ParticleSystem::setOrigin(glm::vec3 origin) {
	m_origin = origin;
}

glm::vec4 ParticleSystem::getColor() {
	return m_color;
}

void ParticleSystem::setColor(glm::vec4 color) {
	m_color = color;

	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i].m_color.r = color.r;
		m_particles[i].m_color.g = color.g;
		m_particles[i].m_color.b = color.b;
	}
}

void ParticleSystem::particlesReset() {
	for (int i = 0; i < m_particles.size(); ++i) {
		m_particles[i].reset();
		m_particles[i].initAge();
	}

}

void ParticleSystem::reset() {
	initParticles();
}

void ParticleSystem::draw(glm::mat4 parent, glm::mat4 cam) {
	if (m_enabled) {
		for (int i = 0; i < m_numParticles; i++) {
			m_particles[i].draw(parent, cam);
			m_particles[i].step();
		}
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

void ParticleSystem::reverse() {
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i].m_reversed = !m_particles[i].m_reversed;
	}
}

bool ParticleSystem::getReversed() {
	return m_particles[0].m_reversed;
}
