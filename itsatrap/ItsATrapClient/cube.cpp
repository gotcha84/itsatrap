#include "Geode.h"

namespace sg {

	class Cube : public Geode {
		public:
			void Cube::draw(glm::mat4 parent, glm::mat4 camera) {
				glm::mat4 cam_inverse = glm::inverse(camera);
				glm::mat4 mv = cam_inverse * parent;

				glPushMatrix();
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(glm::value_ptr(mv));

					glColor3f(color.x, color.y, color.z);
					glutSolidCube(1);
				glPushMatrix();
			}
	};

}