#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <GL/glut.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Camera.h"
#include <string>

using namespace std;

class Player {
	public:

		Camera m_cam;

		glm::mat4 m_viewportMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_modelMatrix;
		glm::mat4 m_modelviewMatrix;

		float m_xWalkFactor;		// how fast you wana walk
		float m_zWalkFactor;	

		Player();
		void handleMovement(unsigned char);
		void updateModelViewMatrix();
};

#endif