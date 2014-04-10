#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glut.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <string>

using namespace std;

class Camera {
	public:
		//Vector3 e, d, up;
		//Matrix4 c;
		Camera();
		void inverse();
		//void set(Vector3&, Vector3&, Vector3&);
		void identity();
		//GLfloat* getGLMatrix();
};

#endif