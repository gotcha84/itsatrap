#include "Utilities.h"

// unused since glLoadMatrixf(glm::value_ptr(modelview)); modelview is a mat4 works apparently according to anurag
GLfloat* Utilities::matrixToGLMatrix(glm::mat4 matrix) {

	GLfloat retval[16];

	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			retval[(i * 4) + j] = matrix[j][i];
		}
	}

	return retval;
}

int Utilities::roundToInt(float num) {
	return int(floor(num + 0.5));
}

void Utilities::writeIntVectorToFile(vector<int> vec, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i = 0; i<vec.size(); i++) {
		outputFile << vec[i] << endl;
	}

}

void Utilities::writeFloatVectorToFile(vector<float> vec, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i = 0; i<vec.size(); i++) {
		outputFile << vec[i] << endl;
	}

}


void Utilities::writeIntArrayToFile(int* arr, int size, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i = 0; i<size; i++) {
		outputFile << arr[i] << endl;
	}

}

void Utilities::writeFloatArrayToFile(float* arr, int size, string filename) {
	ofstream outputFile = ofstream(filename);
	for (int i = 0; i<size; i++) {
		outputFile << arr[i] << endl;
	}

}


vector<float> Utilities::modifyVec(vector<float> oldVec, float scale, float xTranslate, float yTranslate, float zTranslate) {
	vector<float> vec;
	for (int i = 0; i < oldVec.size(); i++) {
		if (i % 3 == 0) {
			vec.push_back((oldVec[i] * scale) + xTranslate);
		}
		else if (i % 3 == 1) {
			vec.push_back((oldVec[i] * scale) + yTranslate);
		}
		else if (i % 3 == 2) {
			vec.push_back((oldVec[i] * scale) + zTranslate);
		}
	}
	return vec;
}

vector<float> Utilities::modifyVec(vector<float> oldVec, glm::vec3 scale, float xTranslate, float yTranslate, float zTranslate) {
	vector<float> vec;
	for (int i = 0; i < oldVec.size(); i++) {
		if (i % 3 == 0) {
			vec.push_back((oldVec[i] * scale.x) + xTranslate);
		}
		else if (i % 3 == 1) {
			vec.push_back((oldVec[i] * scale.y) + yTranslate);
		}
		else if (i % 3 == 2) {
			vec.push_back((oldVec[i] * scale.z) + zTranslate);
		}
	}
	return vec;
}

vector<float> Utilities::findMinsMaxs(string inputfile) {

	vector<tinyobj::shape_t> shapes;

	string err = tinyobj::LoadObj(shapes, inputfile.c_str());

	vector<float> tmp;
	float minx = FLT_MAX;
	float minz = FLT_MAX;
	float maxx = -1.0f*FLT_MAX;
	float maxz = -1.0f*FLT_MAX;



	for (int j = 0; j < shapes.size(); j++) {
		tmp = findMinsMaxsHelper(shapes[j].mesh.positions);
		if (tmp[0] < minx) {
			minx = tmp[0];
		}
		if (tmp[1] < minz) {
			minz = tmp[1];
		}
		if (tmp[2] > maxx) {
			maxx = tmp[2];
		}
		if (tmp[3] > maxz) {
			maxz = tmp[3];
		}

	}

	vector<float> returnMe;
	returnMe.push_back(minx);
	returnMe.push_back(minz);
	returnMe.push_back(maxx);
	returnMe.push_back(maxz);
	return returnMe;
}

vector<float> Utilities::findMinsMaxsHelper(vector<float> vec) {
	float minx = FLT_MAX;
	float minz = FLT_MAX;
	float maxx = -1.0f*FLT_MAX;
	float maxz = -1.0f*FLT_MAX;
	for (int i = 0; i < vec.size(); i++) {
		if (i % 3 == 0) {
			if (vec[i] < minx) {
				minx = vec[i];
			}
			if (vec[i] > maxx) {
				maxx = vec[i];
			}
		}
		if (i % 3 == 2) {
			if (vec[i] < minz) {
				minz = vec[i];
			}
			if (vec[i] > maxz) {
				maxz = vec[i];
			}
		}

	}
	vector<float> returnMe;
	returnMe.push_back(minx);
	returnMe.push_back(minz);
	returnMe.push_back(maxx);
	returnMe.push_back(maxz);
	return returnMe;
}

glm::vec3 Utilities::findAverage(string inputfile) {
	vector<tinyobj::shape_t> shapes;
	string err = tinyobj::LoadObj(shapes, inputfile.c_str());

	int xCount = 0;
	int zCount = 0;

	float yMin = FLT_MAX;

	float xTotal = 0.0f;
	float zTotal = 0.0f;


	for (int i = 0; i < shapes.size(); i++) {
		for (int j = 0; j < shapes[i].mesh.positions.size(); j++) {
			if (j % 3 == 0) {
				xTotal += shapes[i].mesh.positions[j];
				xCount++;
			}
			else if (j % 3 == 1) {
				if (shapes[i].mesh.positions[j] < yMin) {
					yMin = shapes[i].mesh.positions[j];
				}
			}
			else if (j % 3 == 2) {
				zTotal += shapes[i].mesh.positions[j];
				zCount++;
			}
		}
	}
	cout << "min found: " << yMin << endl;
	float xAvg = xTotal / (float)xCount;
	float zAvg = zTotal / (float)zCount;

	glm::vec3 returnMe = glm::vec3(xAvg, yMin, zAvg);

	return returnMe;
}

glm::mat4 Utilities::rotateY(float theta) { // theta = angle in degrees
	theta = glm::radians(theta);

	return glm::transpose(glm::mat4(
		glm::cos(theta), 0, glm::sin(theta), 0,
		0, 1, 0, 0,
		-glm::sin(theta), 0, glm::cos(theta), 0,
		0, 0, 0, 1
		));
}

glm::mat4 Utilities::rotateZ(float theta) { // theta = angle in degrees
	theta = glm::radians(theta);

	return glm::transpose(glm::mat4(
		glm::cos(theta), -glm::sin(theta), 0, 0,
		glm::sin(theta), glm::cos(theta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		));
}

glm::mat4 Utilities::rotateX(float theta) { // theta = angle in degrees
	theta = glm::radians(theta);

	return glm::transpose(glm::mat4(
		1, 0, 0, 0,
		0, glm::cos(theta), -glm::sin(theta), 0,
		0, glm::sin(theta), glm::cos(theta), 0,
		0, 0, 0, 1
		));
}

float Utilities::degreesToRad(float deg) {
	return (atan(1.0f)*4.0f)*deg / 180.0f;
}

string Utilities::intToBaseThree(int num) {
	int oldNum = num;
	string result = "";
	int rem;
	string tmpstr;
	while (num > 0) {
		rem = num % 3;
		tmpstr = to_string(rem);
		result.append(tmpstr);
		num /= 3;
	}
	while (result.length() != 3) {
		result.append("0");
	}
	return result;
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
