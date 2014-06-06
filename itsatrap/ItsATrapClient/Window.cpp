#include <GL/glew.h>
#include "Window.h"
#include "ClientInstance.h"
#include "Client.h"
#include "ConfigSettings.h"
#include "Shader.h"

extern ClientInstance *client;
extern Texture *textures;
extern sg::Cube *test;
extern FTGLPixmapFont *font;


//int Window::m_width = 1600; // set window width in pixels here
//int Window::m_height = 1200; // set window height in pixels here

int Window::m_width = 512; // set window width in pixels here
int Window::m_height = 512; // set window height in pixels here


int Window::m_heightMapXShift = 278;
int Window::m_heightMapZShift = 463;

int Window::m_fpsCounter = 0;
clock_t Window::m_timer = clock();

bool *Window::keyState = new bool[256];
bool *Window::keyEventTriggered = new bool[256];
bool *Window::specialKeyState = new bool[256];
bool *Window::specialKeyEventTriggered = new bool[256];
int Window::modifierKey = 0;

ISoundEngine *engine;
ISoundEngine *jumpSound;
ISoundEngine *knifeSound;
ISoundEngine *createTrapSound;
ISound *walk;
bool jump;

//#define fbOWIDTHT  1600       // width of fbo
//#define fbOHEIGHT  1200       // hight of fbo
#define fbOWIDTHT  512       // width of fbo
#define fbOHEIGHT  512       // hight of fbo

GLuint Window::fb = 0;           // fbo
GLuint Window::cb = 0;           // texture for color buffer
GLuint Window::rb = 0;           // render buffer for depth buffer
GLuint Window::ab = 0;
GLuint Window::sb = 0;
GLuint Window::pass1 = 0;
GLuint Window::pass2 = 0;
GLuint Window::diffuse = 0;
GLuint Window::specular = 0;
GLuint Window::ambient = 0;
GLuint tex2;

static const GLenum bufs[] = {
	GL_COLOR_ATTACHMENT0_EXT, 
	GL_COLOR_ATTACHMENT1_EXT,
	/*GL_COLOR_ATTACHMENT2_EXT,*/ 
};

Window::Window() {
	for (int i = 0; i < 256; i++) {
		keyState[i] = false;
		specialKeyState[i] = false;
		keyEventTriggered[i] = false;
	}
	engine = createIrrKlangDevice(); //declare loop, pause, and track
	jumpSound = createIrrKlangDevice();
	knifeSound = createIrrKlangDevice();
	createTrapSound = createIrrKlangDevice();
	walk = engine->play2D("../Sound/footstep.wav", true, true, true);
	jump = true;
	client->screen->setColor(glm::vec4(1, 1, 1, 1));
	client->screen->setTexture(textures->m_texID[Textures::Background]);
}

Window::~Window() {
	delete[] keyState;
	keyState = nullptr;

	delete[] specialKeyState;
	specialKeyState = nullptr;
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
	displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, w, h);  // set new viewport size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
	gluPerspective(45.0f, ((float)w) / ((float)h), 1.0f, 1000.0f);
	//glTranslatef(0, 0, -20);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{
	GameStates currState = client->gameState.getState();

	if (currState == WELCOME) {
		//cout << "WELCOME STATE" << endl;

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, 0, 2);
		glScalef(2, 2, 2);
		client->screen->drawCube();
		glPopMatrix();

		

		glPopMatrix();
		glPopMatrix();

	}

	if (currState == GAMEREADY) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();

				glPushMatrix();
					glLoadIdentity();
					glTranslatef(0, 0, 2);
					glScalef(2, 2, 2);
					client->screen->drawCube();
				glPopMatrix();

			glPopMatrix();
		glPopMatrix();
	}

	if (currState == GAMEPLAY || currState == GAMEOVER) {
		float oldBuildingId = client->root->getPlayer()->m_onTopOfBuildingId;
		float oldXRotated = client->root->getCamera()->getXRotated();
		float oldYRotated = client->root->getCamera()->getYRotated();

		bool sendUpdate = false, lookChanged = false;

		glm::vec3 oldPos = client->root->getPlayer()->getPosition();

		processKeys();

		glViewport(0, 0, fbOWIDTHT, fbOHEIGHT);

		glEnable(GL_DEPTH_TEST);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers

		// TODO: lock movement if looking up?
		if (client->root->m_xAngleChange != 0.0f) {
			client->root->getCamera()->m_xRotationAngle = client->root->m_xAngleChange;
			lookChanged = true;
		}
		else {
			client->root->getCamera()->m_xRotationAngle = 0.0f;
		}

		if (client->root->m_yAngleChange != 0.0f) {
			client->root->getCamera()->m_yRotationAngle = client->root->m_yAngleChange;

			lookChanged = true;
		}
		else {
			client->root->getCamera()->m_yRotationAngle = 0.0f;
		}

		int buildingId = -2;

		if (lookChanged) {
			Client::sendLookEvent(client->root->getCameraObjectForNetworking());
		}

		// updates player view
		client->root->getPlayer()->getCamera()->updateCameraMatrix();
		client->root->getPlayer()->updateModelViewMatrix();
		client->root->draw();
		m_fpsCounter += 1;

		if (clock() - m_timer > 1000) {
			m_timer = clock();
			m_fpsCounter = 0;
		}
		glFlush();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glDisable(GL_DEPTH_TEST);

		
		glUseProgram(pass1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cb);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glViewport(0, 0, m_width, m_height);

		glEnable(GL_TEXTURE_2D);

		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-1.0, -1.0);
		glTexCoord2d(1.0, 0.0);
		glVertex2d(1.0, -1.0);
		glTexCoord2d(1.0, 1.0);
		glVertex2d(1.0, 1.0);
		glTexCoord2d(0.0, 1.0);
		glVertex2d(-1.0, 1.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	

	glFlush();
	glutSwapBuffers();
}

void Window::keyDown(unsigned char key, int x, int y)
{
	if (client->gameState.getState() == GAMEPLAY) {
		keyState[key] = true;
		if (key >= '1' && key <= '9') {
			switch (key) {
			default:
				if (!createTrapSound->isCurrentlyPlaying("../Sound/hammer.wav"))
					createTrapSound->play2D("../Sound/hammer.wav", false, false, true);
				break;
			}
		}
	}
}

void Window::keyUp(unsigned char key, int x, int y) {
	keyState[key] = false;
	keyEventTriggered[key] = false;
	GameStates curr = client->gameState.getState();

	if (curr == GAMEREADY) {
		// Enter Key
		if (key == 13) {
			cout << "ENTER PRESSED - READY TO PLAY" << endl;
			Client::sendGameReadyState();
		}
	}

	if (curr == GAMEOVER) {
		// Enter Key
		if (key == 13) {
			cout << "ENTER PRESSED - BACK TO READY" << endl;
			Client::sendGameRestartState();
			client->gameState.setGameReady();
			client->screen->setTexture(textures->m_texID[Textures::BackgroundComplete]);
		}
	}

	if (curr == GAMEPLAY) {
		jump = true;
		walk->setIsPaused(true);
		if (key >= '1' && key <= '9') {
			string filename = TRAMPOLINE_TRAP_OBJ;
			int type = 0;

			switch (key)
			{
			case '1':
				type = TYPE_FREEZE_TRAP;
				filename = FREEZE_TRAP_OBJ;
				//freezeTrapSound->setIsPaused(true);
				break;
			case '2':
				type = TYPE_TRAMPOLINE_TRAP;
				filename = TRAMPOLINE_TRAP_OBJ;
				//tramSound->setIsPaused(true);
				break;
			case '3':
				type = TYPE_SLOW_TRAP;
				filename = SLOW_TRAP_OBJ;
				//slowSound->setIsPaused(true);
				break;
			case '4':
				type = TYPE_PUSH_TRAP;
				filename = PUSH_TRAP_OBJ;
				//pushSound->setIsPaused(true);
				break;
			case '5':
				type = TYPE_LIGHTNING_TRAP;
				filename = DEATH_TRAP_OBJ;
				//lightningSound->setIsPaused(true);
				break;
			case '6':
				type = TYPE_PORTAL_TRAP;
				filename = PORTAL_TRAP_OBJ;
				break;
			case '7':
				type = TYPE_FLASH_TRAP;
				filename = FLASH_TRAP_OBJ;
				break;
			default:
				type = TYPE_FREEZE_TRAP;
				filename = TRAMPOLINE_TRAP_OBJ;
				//freezeTrapSound->setIsPaused(true);
				break;
			}
			sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->m_xRotated, TRAP_DIR + filename);
			struct trapObject t = trap->getTrapObjectForNetworking();
			t.type = type;
			Client::sendSpawnTrapEvent(t);
			delete trap;
			trap = nullptr;
		}
		else if (key == 13)
		{
			cout << "ENTER IS PRESSED" << endl;
			//		client->enterPressed = true;
		}
		else if (key == 'r')
		{
			//ConfigSettings::getConfig()->reloadSettingsFile();
			//Client::sendReloadConfigFile();
		}
		else if (key == 'b')
		{
			Client::sendRecallEvent();
		}
		else if (key == 'e')
		{
			// Resource Tower Hits
			for (int i = 0; i < client->level.resources.size(); ++i) {
				Client::sendChannelAttemptEvent(client->level.resources[i]->getResourceId());
			}
		}
		else if (key == 't')
		{
			cout << "PRESSED T" << endl;
			cout << "getInfoState() " << client->root->trapMenu->getInfoState() << endl;
			string filename = TRAMPOLINE_TRAP_OBJ;
			int type = 0;
			switch (client->root->trapMenu->getInfoState())
			{
			case 0:
				type = TYPE_FREEZE_TRAP;
				filename = FREEZE_TRAP_OBJ;
				break;
			case 1:
				type = TYPE_TRAMPOLINE_TRAP;
				filename = TRAMPOLINE_TRAP_OBJ;
				break;
			case 2:
				type = TYPE_SLOW_TRAP;
				filename = SLOW_TRAP_OBJ;
				break;
			case 3:
				type = TYPE_PUSH_TRAP;
				filename = PUSH_TRAP_OBJ;
				break;
			case 4:
				type = TYPE_LIGHTNING_TRAP;
				filename = DEATH_TRAP_OBJ;
				break;
			case 5:
				type = TYPE_PORTAL_TRAP;
				filename = PORTAL_TRAP_OBJ;
				break;
			case 6:
				type = TYPE_FLASH_TRAP;
				filename = FLASH_TRAP_OBJ;
				break;
			default:
				type = TYPE_FREEZE_TRAP;
				filename = TRAMPOLINE_TRAP_OBJ;
				break;
			}
			sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->m_xRotated, TRAP_DIR + filename);
			struct trapObject t = trap->getTrapObjectForNetworking();
			t.type = type;
			Client::sendSpawnTrapEvent(t);
			delete trap;
			trap = nullptr;
		}
	}
}

void Window::specialKeyDown(int key, int x, int y) {
	if (client->gameState.getState() == GAMEPLAY) {
		modifierKey = glutGetModifiers();

		specialKeyState[key] = true;
	}
}

void Window::specialKeyUp(int key, int x, int y) {
	if (client->gameState.getState() == GAMEPLAY) {
		modifierKey = glutGetModifiers();

		specialKeyState[key] = false;
		specialKeyEventTriggered[key] = false;
	}
}

void Window::processKeys() {
	if (client->gameState.getState() == GAMEPLAY) {
		//client->root->m_player->getPhysics()->m_triedToRun = false;
		//client->root->m_player->getPhysics()->m_triedForward = false;

		int count = 0;
		//PhysicsStates curr_state = client->root->m_player->getPhysics()->m_currentState;
		if (client->root->m_player->m_stunDuration > 0)
		{
			printf("[CLIENT]: Sorry, you are STUNNED! Remaining: %d\n", client->root->m_player->m_stunDuration);
			return;
		}

		// jump
		if (keyState[' '] && jump) {
			Client::sendJumpEvent();
			if (!keyEventTriggered[' ']) {

				keyEventTriggered[' '] = true;

				if (!jumpSound->isCurrentlyPlaying("../Sound/jump.wav")) {
					jumpSound->play2D("../Sound/jump.wav", false, false, true);
				}
			}
			jump = false;
		}

		// modifierKey = 
		// GLUT_ACTIVE_SHIFT
		// GLUT_ACTIVE_CTRL
		// GLUT_ACTIVE_ALT

		// esc to quit
		if (keyState[27]) {
			exit(0);
		}
	}

	// dash
	if (modifierKey == GLUT_ACTIVE_SHIFT) {
		Client::sendMoveEvent(DASH);
	}
	else {
		if (keyState['w']) {
			Client::sendMoveEvent(FORWARD);
			if (!keyEventTriggered['w']) {

				keyEventTriggered['w'] = true;
			}
		}
		else if (keyState['s']) {
			Client::sendMoveEvent(BACKWARD);
			if (!keyEventTriggered['s']) {

				keyEventTriggered['s'] = true;
			}
		}

		// left + right
		if (keyState['a']) {
			Client::sendMoveEvent(LEFT);
			if (!keyEventTriggered['a']) {

				keyEventTriggered['a'] = true;
			}
		}
		else if (keyState['d']) {
			Client::sendMoveEvent(RIGHT);
			if (!keyEventTriggered['d']) {

				keyEventTriggered['d'] = true;
			}
		}

		if (keyState['w'] ||
			keyState['s'] ||
			keyState['a'] ||
			keyState['d']) {
			walk->setIsPaused(false);
		}

		//if (keyState['u']) {
		//	client->root->getPlayer()->Unstuck();
		//}

		// trap
		/*
		if (keyState['1']) {
		sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition());
		struct trapObject t = trap->getTrapObjectForNetworking();
		t.type = TYPE_FREEZE_TRAP;
		Client::sendSpawnTrapEvent(t);
		delete trap;
		}
		*/

		//case 9: // TAB
		//client->toggleCurrentPlayer();
		//client->printSceneGraph();
		//break;

		//*(client->tabPressed) = keyState[9];
		client->tabPressed = keyState[9];
	}
}
void Window::processMouseKeys(int button, int state, int x, int y)
{
	if (client->gameState.getState() == GAMEPLAY) {
		switch (state)
		{
		case GLUT_DOWN:
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
			{
									 // Needs to send a query to the server and check all of the players to see if client has hit anyone
									 printf("[Client]: Knife Swung!\n");

									 if (!knifeSound->isCurrentlyPlaying("../Sound/knife.wav")) {
										 knifeSound->play2D("../Sound/knife.wav", false, false, true);
									 }

									 // Player Hits
									 Client::sendKnifeHitEvent();
									 break;
			}
			case GLUT_RIGHT_BUTTON:
			{
									  cout << "RIGHT PRESSED" << endl;
									  string filename = TRAMPOLINE_TRAP_OBJ;
									  int type = 0;
									  switch (client->root->trapMenu->getInfoState())
									  {
									  case 0:
										  type = TYPE_FREEZE_TRAP;
										  filename = FREEZE_TRAP_OBJ;
										  break;
									  case 1:
										  type = TYPE_TRAMPOLINE_TRAP;
										  filename = TRAMPOLINE_TRAP_OBJ;
										  break;
									  case 2:
										  type = TYPE_SLOW_TRAP;
										  filename = SLOW_TRAP_OBJ;
										  break;
									  case 3:
										  type = TYPE_PUSH_TRAP;
										  filename = PUSH_TRAP_OBJ;
										  break;
									  case 4:
										  type = TYPE_LIGHTNING_TRAP;
										  filename = DEATH_TRAP_OBJ;
										  break;
									  case 5:
										  type = TYPE_PORTAL_TRAP;
										  filename = PORTAL_TRAP_OBJ;
										  break;
									  case 6:
										  type = TYPE_FLASH_TRAP;
										  filename = FLASH_TRAP_OBJ;
										  break;
									  default:
										  type = TYPE_FREEZE_TRAP;
										  filename = TRAMPOLINE_TRAP_OBJ;
										  break;
									  }
									  sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->m_xRotated, TRAP_DIR + filename);
									  struct trapObject t = trap->getTrapObjectForNetworking();
									  t.type = type;
									  Client::sendSpawnTrapEvent(t);
									  delete trap;
									  trap = nullptr;
			}
			default:
				break;
			}
			break;
		default:
			break;
		}
		if (button == 3) {
			//*client->scrollDown = true;
			client->scrollDown = true;
		}
		else if (button == 4) {
			//*client->scrollUp = true;
			client->scrollUp = true;
		}
	}
}

void Window::processMouseMove(int x, int y) {
	if (client->gameState.getState() == GAMEPLAY) {
		//if (*client->m_xMouse != x) {
		//	client->root->m_xAngleChange = float((float)x - *client->m_xMouse) / client->root->m_xAngleChangeFactor;
		if (client->m_xMouse != x) {
			client->root->m_xAngleChange = float((float)x - client->m_xMouse) / client->root->m_xAngleChangeFactor;
		}

		//if (*client->m_yMouse != y) {
		//	client->root->m_yAngleChange = float((float)y - *client->m_yMouse) / client->root->m_yAngleChangeFactor;
		if (client->m_yMouse != y) {
			client->root->m_yAngleChange = float((float)y - client->m_yMouse) / client->root->m_yAngleChangeFactor;
		}

		// keeps mouse centered
		if (x != m_width / 2 || y != m_height / 2) {
			glutWarpPointer(m_width / 2, m_height / 2);
		}

		//*client->m_xMouse = x;
		//*client->m_yMouse = y;
		client->m_xMouse = x;
		client->m_yMouse = y;
	}
}
