#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <stdlib.h> // srand + rand
#include <time.h> // for random seed

using namespace std;

struct Particle
{
	int id;
	glm::vec3 m_origin;
	glm::vec3 m_pos;
	glm::vec3 m_vel;
	glm::vec4 m_color;
	int m_age;
	int m_life;

	Particle();
	Particle(glm::vec3 origin);
	Particle(glm::vec3 origin, int life);
	~Particle();

	void init();
	void initAge();

	int getID();

	void step();
	void draw();
};

class ParticleSystem {
	public:
		bool m_enabled;

		int m_numParticles;
		float m_maxLifetime;
		vector<Particle> m_particles;
		
		glm::vec3 m_origin;

		ParticleSystem(int numParticles);
		~ParticleSystem();
		
		void initParticles();

		int getNumParticles();
		void setNumParticles(int numParticles);
		int getMaxLifetime();
		void setMaxLifetime(int life);

		void setOrigin(glm::vec3 origin);
		glm::vec3 getOrigin();

		void reset();
		void step();
		void draw();

		bool isEnabled();
		void enable();
		void disable();
};

#endif