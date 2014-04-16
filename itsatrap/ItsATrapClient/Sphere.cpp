#include "Geode.h"

class Sphere : public Geode {
	public:
		void Sphere::draw(glm::mat4 parent) {
			glPushMatrix();
				glLoadMatrixf(glm::value_ptr(parent));

				glColor3f(1,0,0);
				glutSolidSphere(1, 10, 10);
			glPushMatrix();
		}
};