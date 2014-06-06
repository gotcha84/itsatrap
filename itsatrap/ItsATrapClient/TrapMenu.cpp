#include "TrapMenu.h"

TrapMenu::TrapMenu() {
	infoState = 0;
	font = new FTGLPixmapFont("C:/Windows/Fonts/Arial.ttf");
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

			if ((infoState - 1) < 0) trapInfo(6, -0.82f, 1.0f, 0.0f, 0.0f, 75);
			else trapInfo(infoState - 1, -0.82f, 1.0f, 0.0f, 0.0f, 75);
			trapInfo(infoState, -0.87f, 1.0f, 1.0f, 0.0f, 100);
			if ((infoState + 1) > 6) trapInfo(0, -0.92f, 1.0f, 0.0f, 0.0f, 75);
			else trapInfo(infoState + 1, -0.92f, 1.0f, 0.0f, 0.0f, 75);
		
		drawCube(0.8f, -0.85f, 0.0f);
		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void TrapMenu::drawCube(float x, float y, float z) {
	glPushMatrix();
	glm::mat4 matrix;
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	//matrix = glm::scale(matrix, glm::vec3(0.35f, 0.25f, 0.0f));
	matrix = glm::scale(matrix, glm::vec3(0.85f, 0.75f, 0.0f));
	glColor4f(0.0f, 0.0f, 0.0f, 0.25f);
	glLoadMatrixf(glm::value_ptr(matrix));
	glutSolidCube(1);
	glPopMatrix();
}

void TrapMenu::trapInfo(int state, float y, float r, float g, float b, int faceSize) {
	std::string text = "";
	glColor3f(r, g, b);
	font->FaceSize(faceSize);
	font->CharMap(ft_encoding_symbol);
	glRasterPos2f(0.65f, y);

	switch (state) {
		case 0:
			text = "1. Freeze Trap";
			break;
		case 1:
			text = "2. Trampoline Trap";
			break;
		case 2:
			text = "3. Slow Trap";
			break;
		case 3:
			text = "4. Push Trap";
			break;
		case 4:
			text = "5. Lightning Trap";
			break;
		case 5:
			text = "6. Portal Trap";
			break;
		case 6:
			text = "7. Flash Trap";
		default:
			break;
	}
	/*for (int i = 0; i<text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}*/
	font->Render(text.c_str());
	glRasterPos2f(0.90f, y);
	int cost = 0;
	switch (state) {
	case 0:
		ConfigSettings::getConfig()->getValue("CostFreezeTrap", cost);
		text = to_string(cost);
		break;
	case 1:
		ConfigSettings::getConfig()->getValue("CostTrampolineTrap", cost);
		text = to_string(cost);
		break;
	case 2:
		ConfigSettings::getConfig()->getValue("CostSlowTrap", cost);
		text = to_string(cost);
		break;
	case 3:
		ConfigSettings::getConfig()->getValue("CostPushTrap", cost);
		text = to_string(cost);
		break;
	case 4:
		ConfigSettings::getConfig()->getValue("CostLightningTrap", cost);
		text = to_string(cost);
		break;
	case 5:
		ConfigSettings::getConfig()->getValue("CostPortalTrap", cost);
		text = to_string(cost);
		break;
	case 6:
		ConfigSettings::getConfig()->getValue("CostFlashTrap", cost);
		text = to_string(cost);
	default:
		break;
	}
	font->Render(text.c_str());
}

void TrapMenu::setInfoState(int state) {
	infoState = state;
}

int TrapMenu::getInfoState() {
	return infoState;
}
