#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

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
	Count = 5
};

class Physics {
	public:
		PhysicsStates m_currentState;

		glm::vec3 m_velocity;
		glm::vec3 m_position;
	
		float m_gravityConstant; 
		float m_elasticityConstant;

		// TODO: find correct sizes
		Physics();
		Physics(glm::vec3 pos);
		~Physics();

		glm::vec3 falling();
		void applyGravity();

		glm::vec3 handleCollisionDetection(glm::vec3 goTo);

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);
};

#endif