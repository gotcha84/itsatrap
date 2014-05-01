#pragma once
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
#include "Physics.h"
#include "AABB.h"
#include "NetworkObjects.h"

using namespace std;

class MyPlayer {
	public:
		Camera *m_cam;
		Physics *m_physics;
		AABB *m_boundingBox;

		glm::mat4 m_transMatrix;

		glm::mat4 m_modelMatrix;
		glm::mat4 m_modelviewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewportMatrix;

		float m_xWalkFactor; // how fast you want to walk
		float m_zWalkFactor;
		float m_xSlowWalkFactor;
		float m_zSlowWalkFactor;

		MyPlayer();
		MyPlayer(glm::vec3 pos);
		~MyPlayer();

		Camera *getCamera();
		glm::vec3 getPosition();
		Physics *getPhysics();
		AABB *getAABB();
		glm::mat4 getTransMatrix();
		glm::mat4 getModelMatrix();
		glm::mat4 getCameraMatrix();
		glm::mat4 getModelViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewPortMatrix();

		void handleMovement(unsigned char key);
		void updateModelViewMatrix();
		
		void setTransMatrix(glm::mat4 m);
		void setModelMatrix(glm::mat4 m);
		void setProjectionMatrix();
		void setViewportMatrix();

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);

		void lookIn(glm::vec3 direction);
		void lookAt(glm::vec3 point);

		void updateBoundingBox();
		bool collidesWith(MyPlayer *other);
		bool knifeHitWith(MyPlayer *other);
};

#endif