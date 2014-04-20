#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Camera.h"
#include "Physics.h"
#include <string>

using namespace std;

class MyPlayer {
	public:

		Camera m_cam;
		Physics m_physics;

		glm::mat4 m_viewportMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_modelMatrix;
		glm::mat4 m_modelviewMatrix;

		float m_xWalkFactor;		// how fast you wana walk
		float m_zWalkFactor;
		float m_xSlowWalkFactor;
		float m_zSlowWalkFactor;

		MyPlayer();
		void handleMovement(unsigned char);
		void updateModelViewMatrix();
		void setProjectionMatrix();
		void setViewportMatrix();
};

#endif