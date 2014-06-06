//Welcome.cpp
#include "Welcome.h"
#include <GL/glut.h>
#include <fstream>
#include <algorithm>
using namespace std;

Welcome::Welcome() {
	font = new FTGLPixmapFont("C:/Windows/Fonts/Arial.ttf");
}

Welcome::~Welcome() {

}

void Welcome::draw() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 0.0f, 0.0f);
	font->FaceSize(75);
	font->CharMap(ft_encoding_symbol);
	glRasterPos3f(-0.3f, 0.8f, 0.8f);
	font->Render("Welcome Gamers!!!");
	glRasterPos3f(-0.43f, 0.0f, 0.8f);
	font->Render("Press Enter to continue");
	drawCube(0.0f, 0.0f, 0.1f);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void Welcome::drawCube(float x, float y, float z) {
	glPushMatrix();
	glm::mat4 matrix;
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	matrix = glm::scale(matrix, glm::vec3(3.0f, 3.0f, 0.0f));
	glColor3f(0.0f, 0.0f, 0.0f);
	glLoadMatrixf(glm::value_ptr(matrix));
	glutSolidCube(1);
	glPopMatrix();
}

