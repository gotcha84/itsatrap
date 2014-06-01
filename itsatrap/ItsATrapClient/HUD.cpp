#include "HUD.h"

#include <iostream>
bool healthCheck;
// default constructor
HUD::HUD() {
	font = new FTGLPixmapFont("C:/Windows/Fonts/Arial.ttf");
	board = new Scoreboard();
	m_progressTime = -1;
	ouchSound = createIrrKlangDevice(); //declare loop, pause, and track
	deathSound = createIrrKlangDevice(); //declare loop, pause, and track
}

// destructor
HUD::~HUD() {

}

void HUD::draw(int health, int resources, int spawnTime, float flashFade, int hitCrosshairDuration) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
		cout << "Hello this is spawnTime: " << spawnTime << endl;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			if (health <= 0) {
				
				drawDeathTimer(spawnTime);
				
				if (!deathSound->isCurrentlyPlaying("../Sound/death.wav") && spawnTime <= 2500 && spawnTime >= 2000) {
					deathSound->play2D("../Sound/death.wav", false, false, true);
				}
			}else {
				if (hitCrosshairDuration > 0) {
					drawKillSymbol(true);
					if (!ouchSound->isCurrentlyPlaying("../Sound/ouch.wav") )
						ouchSound->play2D("../Sound/ouch.wav", false, false, true);
				}else
					drawKillSymbol(false);
				drawCrossHair();
				drawHealthBar(health);
				drawResource(resources);
				if( m_progressTime > -1 ) drawProgressBar(m_progressTime);
				drawFlashbag(flashFade);
				
			}
			
		glPopMatrix();
	glPopMatrix();
		
}

void HUD::drawCrossHair() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		std::string text = ".";
		glColor4f(0,0,0,1);
		glRasterPos2f(-0.02f, -0.02f);
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
	for (int i = 0; i<health / 10; i++) {
		// make health turn more red/less green with lower
		glColor4f(1.0f - health / 100.0f, health / 100.0f, 0, 1);

		// transform modelview matrix to new position
		glLoadIdentity();
		glTranslatef(-0.9 + 0.05*i, 0.85, 0);
		glScalef(1, 2.5f, 1);
		glutSolidCube(0.05);
	}

	// black outline for health bar
	for (int i = 0; i<10; i++) {
		glColor4f(0, 0, 0, 0.5f);
		glLoadIdentity();
		glTranslatef(-0.9 + 0.05*i, 0.85, 0);
		glScalef(1, 2.5f, 1);
		glutSolidCube(0.05);
	}

	std::string text = std::to_string(static_cast<long long>(health));
	glRasterPos2f(-0.95f, 0.9f);
	for (int i = 0; i<text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
	glPopMatrix();

}

void HUD::drawResource(int resource) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		
		glColor4f(0,20,0,1); // green
		font->FaceSize(75);
		font->CharMap(ft_encoding_symbol);
		glRasterPos2f(0.65f, 0.8f);
		font->Render(to_string(resource).c_str());

	glPopMatrix();
}

void HUD::drawDeathTimer(int respawnTime) {

	font->FaceSize(50);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(-0.55f, 0.0f);
	font->Render("You are Dead");

	font->FaceSize(50);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(-0.05f, -0.3f);
	std::string  text = std::to_string(respawnTime/1000);
	font->Render(text.c_str());

	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScaled(10.0f, 10.0f, 10.0f);
	glutSolidCube(0.2f);
}

void HUD::drawProgressBar(int time) {
	if (time >= 10) {
		for (int i = 0; i < time / 10; i++) {
			glColor3f(1.0f, 1.0f, 1.0f);
			glLoadIdentity();
			glTranslatef(-0.315 + 0.07*i, 0.0f, 0.0f);
			glScalef(1.5f, 1.5f, 1.0f);
			glutSolidCube(0.05f);
		}

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScaled(1.5f, 0.2f, 0.2f);
		glutSolidCube(0.5f);
	}
}

void HUD::drawFlashbag(float fade) {
	glColor4f(1.0f, 1.0f, 1.0f, fade);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScaled(10.0f, 10.0f, 10.0f);
	glutSolidCube(0.2f);
}

void HUD::drawKillSymbol(bool hit) {
	if (hit) {
		glColor3f(0.0f, 0.0f, 0.0f);
		font->FaceSize(100);
		font->CharMap(ft_encoding_symbol);
		glRasterPos2f(-0.02f, -0.02f);
		font->Render("X");
	}
}