#include "Utilities.h"

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

int Utilities::roundToInt(float num) {
	return int(floor(num+0.5));
}

void Utilities::writeIntVectorToFile(vector<int> vec, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i=0; i<vec.size(); i++) {
		outputFile << vec[i] << endl;
	}

}

void Utilities::writeFloatVectorToFile(vector<float> vec, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i=0; i<vec.size(); i++) {
		outputFile << vec[i] << endl;
	}

}


void Utilities::writeIntArrayToFile(int* arr, int size, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i=0; i<size; i++) {
		outputFile << arr[i] << endl;
	}

}

void Utilities::writeFloatArrayToFile(float* arr, int size, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i=0; i<size; i++) {
		outputFile << arr[i] << endl;
	}

}

/*
int Utilities::getMaxDepth(sg::Node *node) {
	sg::Group *group = dynamic_cast<sg::Group*>(node);

	if (group == nullptr) {
		// if current node is a geode, reached bottom of this subtree
		return 0;
	}
	else
	{
		// compute the depth of each subtree
		vector<int> subtreeDepth(group->getNumChildren(), 0);
		for (int i=0; i<group->getNumChildren(); i++) {
			subtreeDepth[i] = getMaxDepth(group->getChild(i));
		}

		int maxIndex = 0;
		int maxValue = 0;

		for (int i=0; i<subtreeDepth.size(); i++) {
			if (subtreeDepth[i] > maxValue) {
				maxIndex = i;
				maxValue = subtreeDepth[i];
			}
		}

		return maxValue + 1;
	}
}
*/
