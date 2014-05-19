#define RAD 0.5f

#include "Geode.h"
#include "MatrixTransform.h"

namespace sg {

	class Cube : public Geode {
		public:
			void draw(glm::mat4 parent, glm::mat4 cam) {
				glm::mat4 mv = glm::inverse(cam) * parent;

				glPushMatrix();
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(glm::value_ptr(mv));

					glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
					glutSolidCube(2*RAD);
				glPopMatrix();
			}

			void print() {
				cout << "(" << this->getObjectID() << " Cube: " << this->getName() << ")";
			}
	};

}