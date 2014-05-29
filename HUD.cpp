#include "HUD.h"
#include <iostream>

// default constructor
HUD::HUD() {
	
}

// destructor
HUD::~HUD() {

}

void HUD::draw(int health, int resources) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();

			drawCrossHair();
			drawHealthBar(health);
			drawResource(resources);

			//drawDeathTimer(5.0f);
	glPopMatrix();
		
}

void HUD::drawCrossHair() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		std::string text = ".";
		glColor4f(0,0,0,1);
		glRasterPos2f(0,0);
		for (int i=0; i<text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}
	glPopMatrix();
}

void HUD::drawHealthBar(int health) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		// draw health bar (bunch of cubes)
		for (int i=0; i<health/10; i++) {
			// make health turn more red/less green with lower
			glColor4f(1.0f-health/100.0f, health/100.0f, 0, 1);
			
			// transform modelview matrix to new position
			glLoadIdentity();
			glTranslatef(-0.9+0.05*i, 0.85, 0);
			glScalef(1,2.5f,1);
			glutSolidCube(0.05);
		}

		// black outline for health bar
		for (int i=0; i<10; i++) {
			glColor4f(0,0,0,0.5f);
			glLoadIdentity();
			glTranslatef(-0.9+0.05*i, 0.85, 0);
			glScalef(1,2.5f,1);
			glutSolidCube(0.05);
		}

		std::string text = std::to_string(static_cast<long long>(health));			
		glRasterPos2f(-0.95f, 0.9f);
		for (int i=0; i<text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}

		/*
		// TODO - draw health bar(s) instead of number
		glm::mat4 matrix;
		matrix = glm::translate(matrix, glm::vec3(-0.9f, -0.9f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(0.5f, 0.2f, 0.0f));
		glColor4f(0,1,0,1);
			
		glLoadMatrixf(glm::value_ptr(matrix));
		glutSolidCube(1);
		*/
	glPopMatrix();
}

void HUD::drawResource(int resource) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		
		glColor4f(0,20,0,1); // green

		std::string text = std::to_string(static_cast<long long>(resource));			
		glRasterPos2f(0.8f, 0.9f);
		for (int i=0; i<text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}

		/*
		// TODO - draw health bar(s) instead of number
		glm::mat4 matrix;
		matrix = glm::translate(matrix, glm::vec3(-0.9f, -0.9f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(0.5f, 0.2f, 0.0f));
		glColor4f(0,1,0,1);
			
		glLoadMatrixf(glm::value_ptr(matrix));
		glutSolidCube(1);
		*/
	glPopMatrix();
}

void HUD::drawDeathTimer(float respawnTime) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		glPushMatrix();
			glTranslatef(0,0,-0.5f);
			glColor4f(10,0,0,0.1f);
			glutSolidCube(2.0f);
		glPopMatrix();

		glColor4f(0,0,0,1);
		std::string text = std::to_string(static_cast<long long>(respawnTime));
		glRasterPos2f(0, 0);
		for (int i=0; i<text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}
	glPopMatrix();
}