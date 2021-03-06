#include "HUD.h"

#include "Texture.h"
extern Texture *textures;

#include <iostream>
bool healthCheck;
// default constructor
HUD::HUD() {
	//font = new FTGLPixmapFont("C:/Windows/Fonts/Arial.ttf");
	font = new FTGLPixmapFont("C:/Windows/Fonts/Corbel.ttf");
	board = new Scoreboard();
	m_progressTime = -1;
	ouchSound = createIrrKlangDevice(); //declare loop, pause, and track
	deathSound = createIrrKlangDevice(); //declare loop, pause, and track

	flashStatus = new sg::Cube();
	flashStatus->setColor(glm::vec4(1, 1, 1, 1));
	flashStatus->setTexture(textures->m_texID[Textures::Flashed]);
	slowStatus = new sg::Cube();
	slowStatus->setColor(glm::vec4(1, 1, 1, 1));
	slowStatus->setTexture(textures->m_texID[Textures::Slowed]);
	stunStatus = new sg::Cube();
	stunStatus->setColor(glm::vec4(1, 1, 1, 1));
	stunStatus->setTexture(textures->m_texID[Textures::Stunned]);
}

// destructor
HUD::~HUD() {
	delete flashStatus;
	flashStatus = nullptr;
	delete slowStatus;
	slowStatus = nullptr;
	delete stunStatus;
	stunStatus = nullptr;
}

void HUD::draw(int health, int resources, int spawnTime, float flashFade, float bloodFade, int hitCrosshairDuration, int recallElapsed, string msg, int gameTime, int slowDuration, int stunDuration) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();

			if (health <= 0) {
				drawDeathTimer(spawnTime);

				if (!deathSound->isCurrentlyPlaying("../Sound/death.wav") && spawnTime <= 2500 && spawnTime >= 2000) {
					deathSound->play2D("../Sound/death.wav", false, false, true);
				}
			}
			else {
				if (hitCrosshairDuration > 0) {
					drawKillSymbol(true);
					if (!ouchSound->isCurrentlyPlaying("../Sound/ouch.wav"))
						ouchSound->play2D("../Sound/ouch.wav", false, false, true);
				}
				else {
					drawKillSymbol(false);
				}
				drawCrossHair();
				drawHealthBar(health);
				drawResource(resources);
				drawClock(gameTime);
				drawInfoMessage(msg);
				if (m_progressTime > -1) {
					drawProgressBar(m_progressTime);
				}
				if (recallElapsed > 0)
				{
					int recallChannelTime = 1;
					ConfigSettings::getConfig()->getValue("RecallChannelTime", recallChannelTime);
					int time = ((float)recallElapsed / recallChannelTime) * 100;
					if (time > 100)
						time = 100;
					else if (time < 0)
						time = 0;
					drawProgressBar(time);
					if (time > 50)
						drawFlashbag((time - 50.0f) / 50.0f);
				}
				drawFlashbag(flashFade);
				drawBlood(bloodFade);

				// Flash status
				/*if (flashFade > 0)
				{
				glLoadIdentity();
				glTranslatef(0.85f, 0.5f, 0);
				glScalef(0.2, 0.2, 2);
				flashStatus->drawCube();
				}*/

				float offset = 0;

				// Stun status
				if (stunDuration > 0)
				{
					glPushMatrix();
					glLoadIdentity();
					glTranslatef(0.85f, 0.25f + offset, 2);
					glScalef(0.2, 0.2, 2);
					stunStatus->drawCube();
					glPopMatrix();

					offset -= 0.25;
				}

				// Slow status
				if (slowDuration > 0)
				{
					glPushMatrix();
					glLoadIdentity();
					glTranslatef(0.85f, 0.25f + offset, 2);
					glScalef(0.2, 0.2, 2);
					slowStatus->drawCube();
					glPopMatrix();

					offset -= 0.25;
				}
			}

	glPopMatrix();
	glPopMatrix();

}

void HUD::drawCrossHair() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	std::string text = ".";
	glColor4f(20.0f, 20.0f, 20.0f, 20.0f);
	glRasterPos2f(-0.02f, -0.02f);
	for (int i = 0; i<text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
	glPopMatrix();
}

void HUD::drawHealthBar(int health) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// draw health bar (bunch of cubes)
	for (int i = 0; i<health; i++) {
		// make health turn more red/less green with lower
		glColor4f(1.0f - health / 100.0f, health / 100.0f, 0, 1);

		// transform modelview matrix to new position
		glLoadIdentity();
		glTranslatef(-0.9 + 0.005*i, 0.85, 0);
		glScalef(0.1f, 2.5f, 1);
		glutSolidCube(0.05);
	}

	// black outline for health bar
	for (int i = 0; i<100; i++) {
		glColor4f(0, 0, 0, 0.5f);
		glLoadIdentity();
		glTranslatef(-0.9 + 0.005*i, 0.85, 0);
		glScalef(0.1f, 2.5f, 1);
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
	glColor4f(0.22f, 20.0f, 20.0f, 20.0f); // teal
	font->FaceSize(75);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(0.65f, 0.8f);
	font->Render(to_string(resource).c_str());

	glPopMatrix();
}

void HUD::drawInfoMessage(string msg) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(0.22f, 1.0f, 1.0f, 1.0f); // teal
	font->FaceSize(30);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(-0.9f, 0.6f);
	font->Render(msg.c_str());

	glPopMatrix();
}

void HUD::drawDeathTimer(int respawnTime) {

	glColor3f(1, 0, 0); // green
	font->FaceSize(50);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(-0.20f, 0.0f);
	font->Render("You are Dead");

	font->FaceSize(50);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(-0.05f, -0.3f);
	std::string  text = std::to_string(respawnTime / 1000);
	font->Render(text.c_str());

	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScaled(10.0f, 10.0f, 10.0f);
	glutSolidCube(0.2f);
}

void HUD::drawProgressBar(int time) {
	if (time >= 10) {

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScaled(1.5f, 0.2f, 0.2f);
		glutSolidCube(0.5f);

		for (int i = 0; i < time / 10; i++) {
			glColor4f(10.0f, 10.0f, 10.0f, 1.0f);
			glLoadIdentity();
			glTranslatef(-0.315 + 0.07*i, 0.0f, 0.5f);
			glScalef(1.5f, 1.5f, 1.0f);
			glutSolidCube(0.05f);
		}
	}
}

void HUD::drawFlashbag(float fade) {
	glColor4f(1.0f, 1.0f, 1.0f, fade);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScaled(10.0f, 10.0f, 10.0f);

	glutSolidCube(0.2f);
}

void HUD::drawBlood(float fade) {
	glColor4f(1.0f, 0, 0, fade);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScaled(10.0f, 10.0f, 10.0f);
	glutSolidCube(0.2f);
}

void HUD::drawKillSymbol(bool hit) {
	if (hit) {
		glColor3f(1.0f, 1.0f, 1.0f);
		font->FaceSize(100);
		font->CharMap(ft_encoding_symbol);
		glRasterPos2f(-0.065f, -0.0975f);
		font->Render("X");
	}
}

void HUD::drawClock(int time) {
	string result;
	int remainTime = 0;
	int minutes = 0;
	int seconds = 0;
	int duration = 0;
	int yellowTime = 60;
	int redTime = 10;


	ConfigSettings::getConfig()->getValue("GameDuration", duration);
	ConfigSettings::getConfig()->getValue("TimeBeforeRed", redTime);
	ConfigSettings::getConfig()->getValue("TimeBeforeYellow", yellowTime);

	remainTime = duration - time;

	minutes = (remainTime / 1000) / 60;
	seconds = (remainTime / 1000) % 60;

	if (minutes != 0) {
		result = to_string(minutes) + ":";
	}
	else {
		result = "0:";
	}

	if (seconds < 10) {
		if (seconds == 0) {
			result += "00";
		}
		else {
			result += "0" + to_string(seconds);
		}
	}
	else {
		result += to_string(seconds);
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (remainTime <= redTime) {
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		font->FaceSize(150);
		glRasterPos2f(-0.15f, 0.7f);
	}
	else if (remainTime <= yellowTime) {
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		font->FaceSize(75);
		glRasterPos2f(-0.05f, 0.8f);
	}
	else {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		font->FaceSize(50);
		glRasterPos2f(-0.05f, 0.8f);
	}
	//font->FaceSize(50);
	font->CharMap(ft_encoding_symbol);
	font->Render(result.c_str());

	glPopMatrix();
}