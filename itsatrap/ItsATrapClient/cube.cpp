#define RAD 0.5f
#define ENABLE_SHADER 0

#include "Geode.h"
#include "MatrixTransform.h"

namespace sg {

	class Cube : public Geode {
		public:
			GLuint m_texID;

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
					//glutSolidCube(2*RAD);
					drawCube();
				glPopMatrix();
				//glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
				//glDisable(GL_TEXTURE_GEN_T);
				//glDisable(GL_TEXTURE_2D);
				
			}

			void drawCube() {
				if (m_texID != 0) {
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, m_texID);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

				glBegin(GL_QUADS);
					// Draw front face:
					glNormal3f(0.0, 0.0, 1.0);
					glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.5, 0.5, 0.5);
					glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5, 0.5, 0.5);
					glTexCoord2f(0.5f, 0.0f); glVertex3f(0.5, -0.5, 0.5);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0.5);

					// Draw left side:
					glNormal3f(-1.0, 0.0, 0.0);
					glTexCoord2f(0.5f, 0.5f); glVertex3f(-0.5, 0.5, 0.5);
					glTexCoord2f(0.5f, 0.0f); glVertex3f(-0.5, 0.5, -0.5);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
					glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.5, -0.5, 0.5);

					// Draw right side:
					glNormal3f(1.0, 0.0, 0.0);
					glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5, 0.5, 0.5);
					glTexCoord2f(0.5f, 0.0f); glVertex3f(0.5, 0.5, -0.5);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
					glTexCoord2f(0.0f, 0.5f); glVertex3f(0.5, -0.5, 0.5);

					// Draw back face:
					glNormal3f(0.0, 0.0, -1.0);
					glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.5, 0.5, -0.5);
					glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5, 0.5, -0.5);
					glTexCoord2f(0.5f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);

					// Draw top side:
					glNormal3f(0.0, 1.0, 0.0);
					glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.5, 0.5, 0.5);
					glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5, 0.5, 0.5);
					glTexCoord2f(0.5f, 0.0f); glVertex3f(0.5, 0.5, -0.5);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 0.5, -0.5);

					// Draw bottom side:
					glNormal3f(0.0, -1.0, 0.0);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
					glTexCoord2f(0.5f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
					glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5, -0.5, 0.5);
					glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.5, -0.5, 0.5);
				glEnd();

				// unbind texture
				if (m_texID != 0) {
					glBindTexture(GL_TEXTURE_2D, 0);
					glDisable(GL_TEXTURE_2D);
				}
			}

			void setTexture(GLuint tex) {
				m_texID = tex;
			}

			void print() {
				cout << "(" << this->getObjectID() << " Cube: " << this->getName() << ")";
			}
	};

}