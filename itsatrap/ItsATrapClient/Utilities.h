#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <GL/glut.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <sstream> // to convert float to string
#include <iomanip> // to round floats

//#include "SceneGraph.h"

using namespace std;

class Utilities {
	public:

		static GLfloat* matrixToGLMatrix(glm::mat4);

		// not needed if we make heightmap more specific
		static int roundToInt(float);
		static void writeIntVectorToFile(vector<int> vec, string filename);
		static void writeFloatVectorToFile(vector<float> vec, string filename);
		static void writeIntArrayToFile(int* arr, int size, string filename);
		static void writeFloatArrayToFile(float* arr, int size, string filename);

		//static int getMaxDepth(sg::Node *node);
};



#endif