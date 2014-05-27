#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <stdlib.h> // srand + rand
#include <time.h> // for random seed
#include <iostream>

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

	float m_rad;
	float m_theta;

	Particle();
	Particle(int id, glm::vec3 origin, glm::vec4 color);
	Particle(int id, glm::vec3 origin, int life, glm::vec4 color);
	~Particle();

	void init();
	void initAge();

	int getID();

	void reset();
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
		glm::vec4 m_color;

		ParticleSystem(int numParticles);
		~ParticleSystem();
		
		void initParticles();

		int getNumParticles();
		void setNumParticles(int numParticles);
		
		int getMaxLifetime();
		void setMaxLifetime(int life);

		glm::vec3 getOrigin();
		void setOrigin(glm::vec3 origin);

		glm::vec4 getColor();
		void setColor(glm::vec4 color);

		void reset();
		void draw();

		bool isEnabled();
		void enable();
		void disable();
};

#endif