#include "HUD.h"
	
// default constructor
HUD::HUD() {

}

// destructor
HUD::~HUD() {

}

void HUD::draw(int health) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();

			drawCrossHair();
			drawHealthBar(health);

		glPopMatrix();
	glPopMatrix();
		
}

void HUD::drawCrossHair() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		std::string text = "X";
		glColor3f(0,0,0);
		glRasterPos2f(0.0f, 0.0f);
		for (int i=0; i<text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}
	glPopMatrix();
}

void HUD::drawHealthBar(int health) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		std::string text = std::to_string(static_cast<long long>(health));
		if (health > 50) {
			glColor3f(0,20,0); // green
		}
		else if (health > 25) {
			glColor3f(20,20,0); // yellow
		}
		else {
			glColor3f(20,0,0); // red
		}
			
		glRasterPos2f(-0.95f, 0.9f);
		for (int i=0; i<text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}

		/*
		// TODO - draw health bar(s) instead of number
		glm::mat4 matrix;
		matrix = glm::translate(matrix, glm::vec3(-0.9f, -0.9f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(0.5f, 0.2f, 0.0f));
		glColor3f(0.0f, 1.0f, 0.0f);
			
		glLoadMatrixf(glm::value_ptr(matrix));
		glutSolidCube(1);
		*/
	glPopMatrix();
}
	
//GLuint HUD::loadAndBufferImage( const char *filename) {
//	GLuint Texture;
//	glGenTextures(1, &Texture);
//	glBindTexture(GL_TEXTURE_2D, Texture);
//	if( glfwLoadTexture2D( filename, GLFW_BUILD_MIPMAPS_BIT)) {
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		return Texture;
//	}else return -1;

	/*GLFWimage imageData;
	int tmp = 100;
	tmp = glfwReadImage(filename, &imageData, NULL);
	if(tmp == GL_FALSE) exit(1);
	GLuint textureBufferID = NULL;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.Width, imageData.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.Data);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glfwFreeImage(&imageData);
	return textureBufferID;*/
//}