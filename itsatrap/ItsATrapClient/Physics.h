#ifndef PHYSICS_H
#define PHYSICS_H

#include "World.h"
#include "SceneGraph.h"
#include "Utilities.h"
#include <vector>

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/ext.hpp>
using namespace std;


// TODO: add more if needed
//enum PhysicsStates {
//	Jumping = 0,
//	Colliding = 1,
//	Moving = 2
//
//};

class Physics {
	
	public:
		glm::vec3 m_velocity;
		glm::vec3 m_position;
	
		float m_gravityConstant; 
		float m_elasticityConstant;


		// TODO: find correct siszes
		Physics();

		glm::vec3 falling();
		void applyGravity();
};

#endif