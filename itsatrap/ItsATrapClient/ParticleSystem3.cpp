#include "ParticleSystem3.h"

#define LIFE 20
#define Y_OFFSET 2.0f
#define VEL 3.0f
#define RAND_VEL (frand()*VEL - VEL/2)
#define POW 2.0f

Particle3::Particle3() {
	init(glm::vec3(0,0,0));
	reset(glm::vec3(0,0,0));
	initAge();
}

float frand()
{
	return (float)rand() / RAND_MAX;
}

Particle3::Particle3(int id, glm::vec3 origin, glm::vec4 color) {
	srand(id);
	init(origin);

	this->id = id;
	m_origin = origin + glm::vec3(0, Y_OFFSET, 0);
	m_color = color;
	m_color.a = 1.0f;

	reset(origin);
	initAge();
}

Particle3::Particle3(int id, glm::vec3 origin, int life, glm::vec4 color) {
	srand(id);
	init(origin);

	this->id = id;
	m_origin = origin + glm::vec3(0, Y_OFFSET, 0);
	m_life = life;
	m_color = color;
	m_color.a = 1.0f;

	reset(origin);
	initAge();
}

Particle3::~Particle3() {

};

void Particle3::init(glm::vec3 playerPos) {
	id = 0;
	m_origin = glm::vec3(0, 0, 0);
	m_pos = playerPos;
	m_vel = glm::vec3(RAND_VEL, RAND_VEL, RAND_VEL);
	m_age = 0;
	m_life = LIFE;
	m_reversed = false;
	m_draw = true;
}

void Particle3::initAge() {
	//m_age = rand() % m_life + 1;
	m_age = 0;
}

int Particle3::getID() {
	return id;
}

void Particle3::reset(glm::vec3 playerPos) {
	init(playerPos);
}

void Particle3::step(glm::vec3 playerPos){
	m_age++; // happy birthday particle! age++

	if (m_age > m_life) {
		m_draw = false;
	}
	else {
		m_color.a = 1.0f - ((float)m_age/m_life); 

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

void Particle3::draw(glm::mat4 parent, glm::mat4 cam) {

	if (!m_draw)
		return;

	glEnable(GL_POINT_SMOOTH);
	glPointSize(10);

	glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);
	glBegin(GL_POINTS);
	glVertex3f(m_pos.x, m_pos.y, m_pos.z);
	glEnd();

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadMatrixf(glm::value_ptr(parent * glm::inverse(cam)));
	//glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	//glutSolidCube(1);
	//glPopMatrix();

	//glDisable(GL_POINT_SMOOTH);
}

ParticleSystem3::ParticleSystem3(int numParticles, glm::vec3 playerPos) {
	m_numParticles = numParticles;
	m_origin = playerPos;
	m_color = glm::vec4(1, 0, 0, 1);
	initParticles();
};

ParticleSystem3::~ParticleSystem3() {
	m_particles.clear();
};

void ParticleSystem3::initParticles() {
	m_particles.clear();
	m_particles.resize(m_numParticles);
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i] = Particle3(i, m_origin, m_color);
	}
}

int ParticleSystem3::getNumParticles() {
	return m_numParticles;
}

void ParticleSystem3::setNumParticles(int numParticles) {
	m_numParticles = numParticles;
}

int ParticleSystem3::getMaxLifetime() {
	return m_maxLifetime;
}

void ParticleSystem3::setMaxLifetime(int life) {
	m_maxLifetime = life;
}

glm::vec3 ParticleSystem3::getOrigin() {
	return m_origin;
}

void ParticleSystem3::setOrigin(glm::vec3 origin) {
	m_origin = origin;
}

glm::vec4 ParticleSystem3::getColor() {
	return m_color;
}

void ParticleSystem3::setColor(glm::vec4 color) {
	m_color = color;

	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i].m_color.r = color.r;
		m_particles[i].m_color.g = color.g;
		m_particles[i].m_color.b = color.b;
	}
}

void ParticleSystem3::particlesReset() {
	for (int i = 0; i < m_particles.size(); ++i) {
		m_particles[i].reset(m_origin);
		m_particles[i].initAge();
	}

}

void ParticleSystem3::reset() {
	initParticles();
}

void ParticleSystem3::draw(glm::mat4 parent, glm::mat4 cam) {
	
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i].draw(parent, cam);
		m_particles[i].step(m_origin);
	}
	
}

bool ParticleSystem3::isEnabled() {
	return m_enabled;
}

void ParticleSystem3::enable() {
	m_enabled = true;
}

void ParticleSystem3::disable() {
	m_enabled = false;
}

void ParticleSystem3::reverse() {
	for (int i = 0; i < m_numParticles; i++) {
		m_particles[i].m_reversed = !m_particles[i].m_reversed;
	}
}

bool ParticleSystem3::getReversed() {
	return m_particles[0].m_reversed;
}
