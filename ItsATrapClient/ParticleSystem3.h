#pragma once
#ifndef PARTICLESYSTEM3_H
#define PARTICLESYSTEM3_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <stdlib.h> // srand + rand
#include <time.h> // for random seed
#include <iostream>

using namespace std;

struct Particle3
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
	bool m_reversed;
	bool m_draw;

	Particle3();
	Particle3(int id, glm::vec3 origin, glm::vec4 color);
	Particle3(int id, glm::vec3 origin, int life, glm::vec4 color);
	~Particle3();

	void init(glm::vec3 playerPos);
	void initAge();

	int getID();

	void reset(glm::vec3 playerPos);
	void step(glm::vec3 playerPos);
	void draw(glm::mat4 parent, glm::mat4 cam);
};

class ParticleSystem3 {
	public:
		bool m_enabled;
		glm::vec3 m_playerPos;

		int m_numParticles;
		float m_maxLifetime;
		vector<Particle3> m_particles;

		glm::vec3 m_origin;
		glm::vec4 m_color;

		ParticleSystem3(int numParticles, glm::vec3 playerPos);
		~ParticleSystem3();

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
		void draw(glm::mat4 parent, glm::mat4 cam);

		bool isEnabled();
		void enable();
		void disable();

		void reverse();
		bool getReversed();
};

#endif