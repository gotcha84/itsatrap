#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "Utilities.h"
#include "World.h"

using namespace std;

// TODO: add more if needed
enum PhysicsStates {
	None = 0,
	Jumping = 1,
	Colliding = 2,
	Moving = 3,
	Falling = 4,
	WallJumping = 5
};

class Physics {
	public:

		PhysicsStates m_currentState;
		clock_t m_stateStart;

		glm::vec3 m_gravity;
		glm::vec3 m_velocity;
		glm::vec3 m_velocityDiff;
		glm::vec3 m_position;
		glm::vec3 m_lastMoved;

		float m_gravityConstant; 
		float m_elasticityConstant;
		glm::vec3 m_restConstant;

		float m_mass;
		float m_yJumpFactor;

		// TODO: find correct sizes
		Physics();
		Physics(glm::vec3 pos);
		Physics(glm::vec3 pos, float mass);
		~Physics();

		glm::vec3 falling();
		void applyGravity();

		bool atRest();

		glm::vec3 handleCollisionDetection(glm::vec3 goTo);

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);
};

#endif