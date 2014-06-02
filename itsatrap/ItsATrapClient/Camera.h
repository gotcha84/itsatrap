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
		glm::vec3 m_slidingHeight;

		glm::vec3 m_playerCenter;

		glm::vec3 m_cameraCenter;
		glm::vec3 m_cameraLookAt;
		glm::vec3 m_cameraUp;
		glm::vec3 m_camZ;
		glm::vec3 m_camX;

		glm::vec3 m_cameraCenterClimb;
		glm::vec3 m_cameraLookAtClimb;
		glm::vec3 m_cameraUpClimb;
		glm::vec3 m_camZClimb;
		glm::vec3 m_camXClimb;

		glm::vec3 m_cameraCenterWallRunHolder;
		glm::vec3 m_cameraLookAtWallRunHolder;
		glm::vec3 m_cameraUpWallRunHolder;
		glm::vec3 m_camZWallRunHolder;
		glm::vec3 m_camXWallRunHolder;

		glm::vec3 m_cameraCenterWallRun;
		glm::vec3 m_cameraLookAtWallRun;
		glm::vec3 m_cameraUpWallRun;
		glm::vec3 m_camZWallRun;
		glm::vec3 m_camXWallRun;

		glm::vec3 m_camZSliding;
	
		glm::mat4 m_cameraMatrix;

		float m_xRotationAngle;
		float m_yRotationAngle;

		float m_xRotated;
		float m_yRotated;
		float m_xRotatedWallRunHolder;
		float m_yRotatedWallRunHolder;

		Camera();
		Camera(glm::vec3 pos);

		void initCommon();

		glm::mat4 getCameraMatrix();
		glm::vec3 getCameraCenter();
		glm::vec3 getCameraLookAt();
		glm::vec3 getCameraUp();

		float calculateXRotated();
		float calculateYRotated();
		
		float getXRotated();
		void setXRotated(float xrot);
		float getYRotated();
		void setYRotated(float yrot);

		void handleXRotation(float magnitude);
		void handleYRotation(float magnitude);
		void calculateAxis();

		void updateCameraMatrix();
		
		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);

		void lookIn(glm::vec3 direction);
		void lookAt(glm::vec3 point);
};

#endif