#pragma once
#ifndef CAMERA_H
#define CAMERA_H

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

using namespace std;

class Camera {
	public:
		glm::vec3 m_playerHeight;
		glm::vec3 m_playerCenter;

		glm::vec3 m_cameraCenter;
		glm::vec3 m_cameraLookAt;
		glm::vec3 m_cameraUp;

		glm::vec3 m_camZ;
		glm::vec3 m_camX;

		glm::mat4 m_cameraMatrix;

		float m_xRotationAngle;
		float m_yRotationAngle;

		Camera();
		Camera(glm::vec3 pos);

		glm::mat4 getCameraMatrix();
		glm::vec3 getCameraCenter();
		glm::vec3 getCameraLookAt();
		glm::vec3 getCameraUp();

		void handleXRotation(char);
		void handleYRotation(char);
		void calculateAxis();

		void updateCameraMatrix();
		
		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);

		void lookIn(glm::vec3 direction);
		void lookAt(glm::vec3 point);
};

#endif