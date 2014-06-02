//GameOver.cpp
#include "GameOver.h"
#include <GL/glut.h>
#include <fstream>
#include <algorithm>
using namespace std;

GameOver::GameOver() {
	font = new FTGLPixmapFont("C:/Windows/Fonts/Arial.ttf");
}

GameOver::~GameOver() {

}

void GameOver::draw() {

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
	glRasterPos2f(-0.3f, 0.8f);

	font->Render("Thanks for playing");	
	displayWin();
	drawCube(0.0f, 0.0f, 0.0f);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GameOver::drawCube(float x, float y, float z) {
	glPushMatrix();
	glm::mat4 matrix;
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	matrix = glm::scale(matrix, glm::vec3(3.0f, 3.0f, 0.0f));
	glColor3f(0.0f, 0.0f, 0.0f);
	glLoadMatrixf(glm::value_ptr(matrix));
	glutSolidCube(1);
	glPopMatrix();
}

void GameOver::setTeamScore(int one, int two) {
	team1 = one;
	team2 = two;
}

void GameOver::displayWin() {
		if (team1 < team2) {
		glColor3f(1.0f, 0.0f, 0.0f);		font->FaceSize(75);		font->CharMap(ft_encoding_symbol);		glRasterPos2f(-0.2f, 0.5f);		font->Render("Team2 Won");
	}
	else if (team1 > team2) {
		glColor3f(1.0f, 0.0f, 0.0f);		font->FaceSize(75);		font->CharMap(ft_encoding_symbol);		glRasterPos2f(-0.2f, 0.5f);		font->Render("Team1 Won");
	}
	else if (team1 == team2) {
		glColor3f(1.0f, 0.0f, 0.0f);		font->FaceSize(75);		font->CharMap(ft_encoding_symbol);		glRasterPos2f(-0.05f, 0.5);		font->Render("Tie");
	}
}