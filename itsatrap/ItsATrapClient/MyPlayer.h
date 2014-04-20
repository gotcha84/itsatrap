#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream> // to convert float to string
#include <string>
#include <math.h>
#include <iomanip> // to round floats

#include "Camera.h"
//#include "Physics.h"

using namespace std;

class MyPlayer {
	public:
		Camera m_cam;
		//Physics m_physics;

		glm::mat4 m_modelMatrix;
		glm::mat4 m_modelviewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewportMatrix;

		float m_xWalkFactor; // how fast you wana walk
		float m_zWalkFactor;	

		MyPlayer();
		~MyPlayer();

		Camera *getCamera();
		glm::mat4 getModelMatrix();
		glm::mat4 getCameraMatrix();
		glm::mat4 getModelViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewPortMatrix();

		void handleMovement(unsigned char key);
		void updateModelViewMatrix();
		void setProjectionMatrix();
		void setViewportMatrix();

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);

		void lookIn(glm::vec3 direction);
		void lookAt(glm::vec3 point);
};

#endif