#include "Geode.h"

namespace sg {

	class Sphere : public Geode {
		public:
			void Sphere::draw(glm::mat4 parent, glm::mat4 cam) {
				glm::mat4 mv = glm::inverse(cam) * parent;

				glPushMatrix();
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(glm::value_ptr(mv));

					glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
					glutSolidSphere(1, 10, 10);
				glPopMatrix();
			}

			void print() {
				cout << "(" << this->getObjectID() << " Sphere: " << this->getName() << ")";
			}
	};

}