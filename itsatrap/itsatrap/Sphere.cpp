#include "Sphere.h"

void Sphere::render(glm::mat4 modelview) {
	glPushMatrix();
		glLoadMatrixf(glm::value_ptr(modelview));

		glColor3f(1,0,0);
		glutSolidSphere(1, 10, 10);
	glPushMatrix();
}