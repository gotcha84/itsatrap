#include "GameOver.h""

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
	displayCredits();
	drawCube(0.0f, 0.0f, -0.2f);

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
		glColor3f(1.0f, 0.0f, 0.0f);
		font->FaceSize(75);
		font->CharMap(ft_encoding_symbol);
		glRasterPos2f(-0.2f, 0.5f);

		font->Render("Team2 Won");
	}
	else if (team1 > team2) {
		glColor3f(1.0f, 0.0f, 0.0f);
		font->FaceSize(75);
		font->CharMap(ft_encoding_symbol);
		glRasterPos2f(-0.2f, 0.5f);

		font->Render("Team1 Won");
	}
	else if (team1 == team2) {
		glColor3f(1.0f, 0.0f, 0.0f);
		font->FaceSize(75);
		font->CharMap(ft_encoding_symbol);
		glRasterPos2f(-0.05f, 0.5);

		font->Render("Tie");
	}
}

void GameOver::displayCredits() {
	glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
	font->FaceSize(50);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(-0.1f, 0.2f);
	font->Render("Group 5");
	glRasterPos2f(-0.5f, 0.0f);
	font->Render("Enrico Bern Hardy Tanuwidjaja");
	glRasterPos2f(-0.5f, -0.2f);
	font->Render("Anurag Thrush Kalavakunta");
	glRasterPos2f(-0.5f, -0.4f);
	font->Render("Kevin Ngo");
	glRasterPos2f(-0.5f, -0.6f);
	font->Render("Mike Boulrice");
	glRasterPos2f(0.3f, 0.0f);
	font->Render("Andre So");
	glRasterPos2f(0.3f, -0.2f);
	font->Render("Yukitoshi Murase");
	glRasterPos2f(0.3f, -0.4f);
	font->Render("Michael Yao");

}