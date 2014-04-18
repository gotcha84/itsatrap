#include "Geode.h"

namespace sg {

	class Sphere : public Geode {
		public:
			void Sphere::draw(glm::mat4 parent) {
				glPushMatrix();
					glLoadMatrixf(glm::value_ptr(parent));

					glColor3f(color.x, color.y, color.z);
					glutSolidSphere(1, 10, 10);
				glPushMatrix();
			}
	};

}