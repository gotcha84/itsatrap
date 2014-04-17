#include "Geode.h"

class Cube : public Geode {
	public:
		void Cube::draw(glm::mat4 parent) {
			glPushMatrix();
				glLoadMatrixf(glm::value_ptr(parent));

				glColor3f(color.x, color.y, color.z);
				glutSolidCube(1);
			glPushMatrix();
		}
};
