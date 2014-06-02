//TrapMenu.cpp
#include "TrapMenu.h"
#include <GL/glut.h>
#include <fstream>
#include <algorithm>
using namespace std;

TrapMenu::TrapMenu() {
	infoState = 0;
}

TrapMenu::~TrapMenu() {

}

void TrapMenu::draw() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	
		//drawCube(0.8f, -0.3f, 0.0f);
		//drawCube(0.8f, -0.575f, 0.0f);
		trapInfo(infoState);
		drawCube(0.8f, -0.85f, 0.0f);
		
		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void TrapMenu::drawCube(float x, float y, float z) {
	glPushMatrix();
	glm::mat4 matrix;
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	matrix = glm::scale(matrix, glm::vec3(0.25f, 0.25f, 0.0f));
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glLoadMatrixf(glm::value_ptr(matrix));
	glutSolidCube(1);
	glPopMatrix();
}

void TrapMenu::trapInfo(int state) {
	std::string text = "";
	glColor3f(1, 0, 0);
	switch (state) {
		case 0:
			glRasterPos2f(0.735f, -0.87f);
			text = "1. Freeze Trap";
			break;
		case 1:
			glRasterPos2f(0.73f, -0.87f);
			text = "2. Trampoline Trap";
			break;
		case 2:
			glRasterPos2f(0.745f, -0.87f);
			text = "3. Slow Trap";
			break;
		case 3:
			glRasterPos2f(0.745f, -0.87f);
			text = "4. Push Trap";
			break;
		case 4:
			glRasterPos2f(0.73f, -0.87f);
			text = "5. Lightning Trap";
			break;
		case 5:
			glRasterPos2f(0.745f, -0.87f);
			text = "6. Portal Trap";
			break;
		case 6:
			glRasterPos2f(0.745f, -0.87f);
			text = "7. Flash Trap";
		default:
			break;
	}
	for (int i = 0; i<text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}

void TrapMenu::setInfoState(int state) {
	infoState = state;
}

int TrapMenu::getInfoState() {
	return infoState;
}