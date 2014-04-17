#include <math.h>
#include <sstream> // to convert float to string
#include <iomanip> // to round floats

#include "Utilities.h"

using namespace std;


// unused since glLoadMatrixf(glm::value_ptr(modelview)); modelview is a mat4 works apparently according to anurag
GLfloat* Utilities::matrixToGLMatrix(glm::mat4 matrix) {

	GLfloat retval[16];

	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			retval[(i*4)+j] = matrix[j][i];
		}
	}

	return retval;
}

int Utilities::closestInt(float num) {
	return int(floor(num+0.5));
}

