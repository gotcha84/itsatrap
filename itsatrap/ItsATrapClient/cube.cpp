#define RAD 0.5f
#define ENABLE_SHADER 0

#include "Geode.h"
#include "MatrixTransform.h"

namespace sg {

	class Cube : public Geode {
		public:

			//Cube() {
				//if (ENABLE_SHADER) {
				//	shader = new Shader();
				//	light = shader->lightShader("../Shaders/phongandtexture.frag", "../Shaders/phongandtexture.vert");

				//}

				////this let the shaders on texture, so when you try to use shader on texture you add this on eline of a code
				//texture = new Texture();
				//if (ENABLE_SHADER) {
				//	glUniform1i(glGetUniformLocation(light, "../Models/building1.ppm"), 0);
				//}
				//texturePPM = texture->loadTexture("../Models/building1.ppm");

			//}


			void draw(glm::mat4 parent, glm::mat4 cam) {
				//glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
				//glEnable(GL_TEXTURE_GEN_T);
				//glEnable(GL_TEXTURE_2D);
				//glBindTexture(GL_TEXTURE_2D, texturePPM);
				glm::mat4 mv = glm::inverse(cam) * parent;

				glPushMatrix();
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(glm::value_ptr(mv));

					glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
					//glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
					glutSolidCube(2*RAD);
				glPopMatrix();
				//glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
				//glDisable(GL_TEXTURE_GEN_T);
				//glDisable(GL_TEXTURE_2D);
				
			}

			void print() {
				cout << "(" << this->getObjectID() << " Cube: " << this->getName() << ")";
			}
	};

}