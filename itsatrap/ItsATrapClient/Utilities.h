#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <GL/glut.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

class Utilities {
	public:

		GLfloat* matrixToGLMatrix(glm::mat4);
		
};

#endif