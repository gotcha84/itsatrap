#pragma once
#ifndef PARTICLESYSTEM2_H
#define PARTICLESYSTEM2_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <stdlib.h> // srand + rand
#include <time.h> // for random seed
#include <iostream>

using namespace std;

struct Particle2
{
	int id;
	glm::vec3 m_origin;
	glm::vec3 m_pos;
	glm::vec3 m_vel;
	glm::vec4 m_color;
	int m_age;
	int m_life;

	bool m_reversed;

	Particle2();
	Particle2(int id, glm::vec3 origin, glm::vec4 color);
	Particle2(int id, glm::vec3 origin, int life, glm::vec4 color);
	~Particle2();

	void init();
	void initAge();

	int getID();

	void reset();
	void step();
	void draw(glm::mat4 parent, glm::mat4 cam);
};

class ParticleSystem2 {
public:
	bool m_enabled;

	int m_numParticles;
	float m_maxLifetime;
	vector<Particle2> m_particles;

	glm::vec3 m_origin;
	glm::vec4 m_color;

	ParticleSystem2(int numParticles);
	~ParticleSystem2();

	void initParticles();

	int getNumParticles();
	void setNumParticles(int numParticles);

	int getMaxLifetime();
	void setMaxLifetime(int life);

	glm::vec3 getOrigin();
	void setOrigin(glm::vec3 origin);

	glm::vec4 getColor();
	void setColor(glm::vec4 color);

	void particlesReset();

	void reset();
	void draw(glm::mat4 parent, glm::mat4 camera);

	bool isEnabled();
	void enable();
	void disable();

	void reverse();
	bool getReversed();
};

#endif