#include "Geode.h"

namespace sg {

	class Cone : public Geode {
		public:
			void draw(glm::mat4 parent, glm::mat4 cam) {
				glm::mat4 mv = glm::inverse(cam) * parent;

				glPushMatrix();
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(glm::value_ptr(mv));

					glColor3f(color.x, color.y, color.z);
					glutSolidCone(1, 1, 10, 10);
				glPopMatrix();
			}

			void print() {
				cout << "(" << this->getObjectID() << " Cone: " << this->getName() << ")";
			}
	};

}