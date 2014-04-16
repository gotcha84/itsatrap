#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "glm/ext.hpp"

#include <string>

using namespace std;

class Camera {
	public:
		//Vector3 e, d, up;
		//Matrix4 c;
		glm::vec3 m_cameraCenter;
		glm::vec3 m_cameraLookAt;
		glm::vec3 m_cameraUp;

		glm::vec3 m_camZ;
		glm::vec3 m_camX;

		glm::mat4 m_cameraMatrix;

		float m_xRotationAngle;
		float m_yRotationAngle;



		Camera();
		//void inverse();
		//void set(Vector3&, Vector3&, Vector3&);
		//void identity(); 
		//GLfloat* getGLMatrix();

		void handleXRotation(char);
		void handleYRotation(char);

		void makeCameraMatrix();

		void calculateAxis();
};

#endif