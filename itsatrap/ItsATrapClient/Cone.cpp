#include "Geode.h"

class Cone : public Geode {
	public:
		void Cone::draw(glm::mat4 parent) {
			glPushMatrix();
				glLoadMatrixf(glm::value_ptr(parent));

				glColor3f(color.x, color.y, color.z);
				glutSolidCone(1, 1, 10, 10);
			glPushMatrix();
		}
};
