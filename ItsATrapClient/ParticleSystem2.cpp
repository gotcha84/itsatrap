#include "ParticleSystem2.h"

Particle2::Particle2() {
	init();
	reset();
	initAge();
}

Particle2::Particle2(int id, glm::vec3 origin, glm::vec4 color) {
	srand(id);
	init();

	this->id = id;
	m_origin = origin;
	m_color = color;
	m_color.a = 1.0f;

	reset();
	initAge();
}

Particle2::Particle2(int id, glm::vec3 origin, int life, glm::vec4 color) {
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

Particle2::~Particle2() {

};

void Particle2::init() {
	id = 0;
	m_origin = glm::vec3(0, 0, 0);
	m_pos = m_origin;
	m_vel = glm::vec3(0, 0, 0);
	m_color = glm::vec4(1, 1, 1, 1);
	m_age = 0;
	m_life = 300;
	m_reversed = false;

	reset();
}

void Particle2::initAge() {
	m_age = rand() % m_life + 1;
}

int Particle2::getID() {
	return id;
}

void Particle2::reset() {
	// 1500, 2500 (x, z)
	m_age = 0;

	int x = rand() % 3000 - 1500;
	int z = rand() % 5000 - 2500;
	if (m_reversed) {
		m_pos = glm::vec3(x, 170, z);
		m_vel = glm::vec3(0, -2.5f, 0.0f);
	}
	else {
		m_pos = glm::vec3(x, -170, z);
		m_vel = glm::vec3(0, 2.5f, 0.0f);
	}
	float red = 0.3f + rand() % 50 / 100.0f;
	float green = 0.3f + rand() % 50 / 100.0f;
	float blue = 0.3 + rand() % 50 / 100.0f;
	m_color = glm::vec4(1, 1, 1, 1.0f);
}

void Particle2::step() {
	m_age++; // happy birthday particle! age++

	if (m_age > m_life) {
		reset();
	}
	else {
		//m_color.a = 1.0f - m_pos.y / (0.1f*m_life); // turn particles black toward the top

		float x = (rand() % 100 - 50) / 50.0f;
		//m_vel.x = x;

		m_pos += m_vel;
		
		if (m_reversed) {

		}
		else {

		}
	}
}

void Particle2::draw(glm::mat4 parent, glm::mat4 cam) {
	//glEnable(GL_POINT_SMOOTH);
	//glPointSize(10);

	glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);
	//glBegin(GL_POINTS);
	//glVertex3f(m_pos.x, m_pos.y, m_pos.z);
	//glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		glLoadMatrixf(glm::value_ptr(parent * glm::inverse(cam)));
		glTranslatef(m_pos.x, m_pos.y, m_pos.z);
		glutSolidSphere(1.0f, 10, 10);
	glPopMatrix();

	//glDisable(GL_POINT_SMOOTH);
}

ParticleSystem2::ParticleSystem2(int numParticles) {
	m_numParticles = numParticles;

	initParticles();
};

ParticleSystem2::~ParticleSystem2() {
	m_particles.clear();
};

void ParticleSystem2::initParticles() {
	m_particles.clear();
	m_particles.resize(m_numParticles);
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i] = Particle2(i, m_origin, m_color);
	}
}

int ParticleSystem2::getNumParticles() {
	return m_numParticles;
}

void ParticleSystem2::setNumParticles(int numParticles) {
	m_numParticles = numParticles;
}

int ParticleSystem2::getMaxLifetime() {
	return m_maxLifetime;
}

void ParticleSystem2::setMaxLifetime(int life) {
	m_maxLifetime = life;
}

glm::vec3 ParticleSystem2::getOrigin() {
	return m_origin;
}

void ParticleSystem2::setOrigin(glm::vec3 origin) {
	m_origin = origin;
}

glm::vec4 ParticleSystem2::getColor() {
	return m_color;
}

void ParticleSystem2::setColor(glm::vec4 color) {
	m_color = color;

	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i].m_color.r = color.r;
		m_particles[i].m_color.g = color.g;
		m_particles[i].m_color.b = color.b;
	}
}

void ParticleSystem2::particlesReset() {
	for (int i = 0; i < m_particles.size(); ++i) {
		m_particles[i].reset();
		m_particles[i].initAge();
	}

}

void ParticleSystem2::reset() {
	initParticles();
}

void ParticleSystem2::draw(glm::mat4 parent, glm::mat4 cam) {
	if (m_enabled) {
		for (int i = 0; i < m_numParticles; i++) {
			m_particles[i].draw(parent, cam);
			m_particles[i].step();
		}
	}
}

bool ParticleSystem2::isEnabled() {
	return m_enabled;
}

void ParticleSystem2::enable() {
	m_enabled = true;
}

void ParticleSystem2::disable() {
	m_enabled = false;
}

void ParticleSystem2::reverse() {
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i].m_reversed = !m_particles[i].m_reversed;
	}
}

bool ParticleSystem2::getReversed() {
	return m_particles[0].m_reversed;
}
