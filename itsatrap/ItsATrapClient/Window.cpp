#include "Window.h"
#include "ClientInstance.h"
#include "Client.h"
#include "ConfigSettings.h"

extern ClientInstance *client;

int Window::m_width  = 512; // set window width in pixels here
int Window::m_height = 512; // set window height in pixels here

int Window::m_heightMapXShift = 278;
int Window::m_heightMapZShift = 463;

bool *Window::keyState = new bool[256];
bool *Window::keyEventTriggered = new bool[256];
char Window::trapKey = 0;
bool *Window::specialKeyState = new bool[256];
bool *Window::specialKeyEventTriggered = new bool[256];
int Window::modifierKey = 0;

ISoundEngine *engine = createIrrKlangDevice();
ISound *walk;
ISound *jumpSound;
ISound *knifeSound;
ISound *freezeTrapSound;
ISound *pushSound;
ISound *tramSound;
ISound *slowSound;
ISound *lightningSound;

/*Sound *walk		  = new Sound("footstep.wav");
Sound *jumpSound  = new Sound("jump.wav");
Sound *knifeSound = new Sound("knife.wav");
Sound *freezeTrapSound  = new Sound("trap.wav");
Sound *pushSound  = new Sound("push.wav");
Sound *tramSound  = new Sound("tram.wav");
Sound *slowSound  = new Sound("slow.wav");
Sound *lightningSound = new Sound("lightning.wav");
*/

Window::Window() {
	for (int i=0; i<256; i++) {
		keyState[i] = false;
		keyEventTriggered[i] = false;
		specialKeyState[i] = false;
		specialKeyEventTriggered[i] = false;
	}	
	jumpSound = engine->play2D("jump.wav", true, true, true);
}

Window::~Window() {
	delete[] keyState;
	keyState = nullptr;

	delete[] specialKeyState;
	specialKeyState = nullptr;

	delete walk;
	walk = nullptr;
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
	glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
	glTranslatef(0, 0, -20);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void) 
{
	processKeys();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	
	// andre added below if sattements
	if (client->root->m_xAngleChange != 0.0f) {
		client->root->handleXRotation(client->root->m_xAngleChange);
		client->root->m_xAngleChange = 0.0f;
		Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}
	
	if (client->root->m_yAngleChange != 0.0f) {
		client->root->handleYRotation(client->root->m_yAngleChange);
		client->root->m_yAngleChange = 0.0f;
		Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}
	
	glm::vec3 oldPos = client->root->getPlayer()->getPosition();
	//cout << "curr state: " << client->root->getPlayer()->getPhysics()->m_currentState << endl;
	if (client->root->getPlayer()->getPhysics()->m_currentState == PhysicsStates::Sliding) {
		client->root->getPlayer()->handleSliding();
	}

	if (client->root->getPlayer()->getPhysics()->m_currentState == PhysicsStates::Climbing) {
		client->root->getPlayer()->applyWallJump();
	}

	client->root->getPlayer()->getPhysics()->applyGravity();

	client->root->getPlayer()->getPhysics()->m_position += client->root->getPlayer()->getPhysics()->m_velocity;
	client->root->getPlayer()->getPhysics()->m_velocity = glm::vec3(0.0f, client->root->getPlayer()->getPhysics()->m_velocity.y, 0.0f);
	
	//client->root->getPlayer()->getPhysics()->m_velocity -= client->root->getPlayer()->getPhysics()->m_velocityDiff;
	client->root->getPlayer()->getPhysics()->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	// TODO: move to player class?
	/*cout << "oldpos: " << glm::to_string(client->root->getPlayer()->getPosition()) << endl;
	cout << "velo: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocity) << endl;
	cout << "newpos: " << glm::to_string(client->root->getPlayer()->getPosition()) << endl << endl;*/
	client->root->getPlayer()->getPhysics()->m_lastMoved = glm::vec3(0.0f, 0.0f, 0.0f);

	if (oldPos != client->root->getPlayer()->getPosition()) {		
		client->root->getPlayer()->getPhysics()->m_lastMoved = client->root->getPlayer()->getPosition() - oldPos;
		//cout << "change is good " << endl;
		client->root->getPlayer()->setModelMatrix(glm::translate(client->root->getPlayer()->getPhysics()->m_position + client->root->getPlayer()->getPhysics()->m_velocity));
		client->root->getPlayer()->updateBoundingBox();
		Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}

	glm::vec3 moved = client->root->getPlayer()->getPhysics()->m_position - client->root->getPlayer()->getCamera()->m_cameraCenter;
	//moved.y += 4.0f;
	
	client->root->getPlayer()->getCamera()->m_cameraCenter += moved;
	client->root->getPlayer()->getCamera()->m_cameraLookAt += moved;
	
	if (client->root->getPlayer()->getPhysics()->m_currentState == PhysicsStates::Sliding) {
		client->root->getPlayer()->getCamera()->m_cameraCenter += client->root->getPlayer()->getCamera()->m_slidingHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt += client->root->getPlayer()->getCamera()->m_slidingHeight + client->root->getPlayer()->getCamera()->m_camZSliding;
	}
	else {
		client->root->getPlayer()->getCamera()->m_cameraCenter += client->root->getPlayer()->getCamera()->m_playerHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt += client->root->getPlayer()->getCamera()->m_playerHeight;
	}


	// updates player view
	client->root->getPlayer()->getCamera()->updateCameraMatrix();
	client->root->getPlayer()->updateModelViewMatrix(); // andre added this line
	client->root->draw();

	if (client->root->getPlayer()->getPhysics()->m_currentState == PhysicsStates::Sliding) {
		client->root->getPlayer()->getCamera()->m_cameraCenter -= client->root->getPlayer()->getCamera()->m_slidingHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt -= (client->root->getPlayer()->getCamera()->m_slidingHeight + client->root->getPlayer()->getCamera()->m_camZSliding);
	}
	else {
		client->root->getPlayer()->getCamera()->m_cameraCenter -= client->root->getPlayer()->getCamera()->m_playerHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt -= client->root->getPlayer()->getCamera()->m_playerHeight;
	}

	//cout << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocity) << endl;

	glFlush();  
	glutSwapBuffers();
}

void Window::keyDown(unsigned char key, int x, int y)
{
	keyState[key] = true;

	if (key >= '1' && key <= '9') {
		trapKey = key;
	}
	//cout << key << " down" << endl;
	cout << key << " : " << keyEventTriggered[key] << endl;
}

void Window::keyUp(unsigned char key, int x, int y) {
	keyState[key] = false;
	keyEventTriggered[key] = false;
	trapKey = 0;
	//cout << key << " up" << endl;
	jumpSound->setIsPaused(true);
	cout << key << " : " << keyEventTriggered[key] << endl;
}

void Window::specialKeyDown(int key, int x, int y) {
	modifierKey = glutGetModifiers();
	
	specialKeyState[key] = true;
	//cout << "special " << key << " down" << endl;
}

void Window::specialKeyUp(int key, int x, int y) {
	modifierKey = glutGetModifiers();

	specialKeyState[key] = false;
	specialKeyEventTriggered[key] = false;
	//cout << "special " << key << " up" << endl;
}

void Window::processKeys() {

	if (client->root->m_player->m_stunDuration > 0)
	{
		printf("[CLIENT]: Sorry, you are STUNNED! Remaining: %d\n", client->root->m_player->m_stunDuration);
		return;
	}

	// modifierKey = 
	// GLUT_ACTIVE_SHIFT
	// GLUT_ACTIVE_CTRL
	// GLUT_ACTIVE_ALT

	// esc to quit
	if (keyState[27]) {
		exit(0);
	}

	// teleport
	if (modifierKey == GLUT_ACTIVE_ALT) {
		client->root->getPlayer()->handleTeleport();
	}

	// forward + backward
	if (keyState['w']) {
		if (modifierKey == GLUT_ACTIVE_SHIFT) {
			client->root->getPlayer()->handleSliding();
		}
		else {
			client->root->getPlayer()->handleMovement('w');
			if (!keyEventTriggered['w']) {
				walk = engine->play2D("footstep.wav", false, false, true);
				keyEventTriggered['w'] = true;
			}
		}
	}
	else if (keyState['s']) {
		client->root->getPlayer()->handleMovement('s');
		if (!keyEventTriggered['s']) {
			walk = engine->play2D("footstep.wav", false, false, true);
			keyEventTriggered['s'] = true;
		}
	}

	// left + right
	if (keyState['a']) {
		client->root->getPlayer()->handleMovement('a');
		if (!keyEventTriggered['a']) {
			walk = engine->play2D("footstep.wav", false, false, true);
			keyEventTriggered['a'] = true;
		}
	}
	else if (keyState['d']) {
		client->root->getPlayer()->handleMovement('d');
		if (!keyEventTriggered['d']) {
			walk = engine->play2D("footstep.wav", false, false, true);
			keyEventTriggered['d'] = true;
		}
	}

	// jump
	if (keyState[' ']) {
		client->root->getPlayer()->handleJump();
		if (!keyEventTriggered[' ']) {
			jumpSound->setIsPaused(false);
			keyEventTriggered[' '] = true;
		}
	}

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

	// reload config file
	if (keyState['r']) {
		ConfigSettings::getConfig()->reloadSettingsFile();
	}

	if (trapKey >= '1' && trapKey <= '9' && !keyEventTriggered[trapKey]) {
		sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->m_xRotated);
		struct trapObject t = trap->getTrapObjectForNetworking();
		
		switch (trapKey) {
			case '1':
				t.type = TYPE_FREEZE_TRAP;
				freezeTrapSound = engine->play2D("trap.wav", false, false, true);
				break;
			case '2':
				t.type = TYPE_TRAMPOLINE_TRAP;
				tramSound = engine->play2D("tram.wav", false, false, true);
				break;
			case '3':
				t.type = TYPE_SLOW_TRAP;
				slowSound = engine->play2D("slow.wav", false, false, true);
				break;
			case '4':
				t.type = TYPE_PUSH_TRAP;
				pushSound = engine->play2D("push.wav", false, false, true);
				break;
			case '5':
				t.type = TYPE_LIGHTNING_TRAP;
				lightningSound = engine->play2D("lightning.wav", false, false, true);
				break;
			default:
				t.type = TYPE_FREEZE_TRAP;
				freezeTrapSound = engine->play2D("trap.wav", false, false, true);
				break;
		}

		keyEventTriggered[trapKey] = true;
		trapKey = 0;
		
		Client::sendSpawnTrapEvent(t);
		delete trap;
		trap = nullptr;
	}
}

void Window::processMouseKeys(int button, int state, int x, int y)
{
	switch (state)
	{
		case GLUT_DOWN:
			switch (button)
			{
				case GLUT_LEFT_BUTTON:
				{
					// Needs to send a query to the server and check all of the players to see if client has hit anyone
					printf("[Client]: Knife Swung!\n");
					knifeSound = engine->play2D("knife.wav", false, false, true);
					int numPlayers = client->players.size();
					for (int i = 0; i < numPlayers; ++i)
					{
						if (i != client->root->getPlayerID())
						{
							bool hit = client->root->knifeHitWith(client->players[i]);

							if (hit)
							{
								Client::sendKnifeHitEvent(i);
							}
						}
					}
					break;
				}
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Window::processMouseMove(int x, int y) {
	
	/*cout << "client factor: " << client->root->m_xAngleChangeFactor << endl;
	cout << "client factor: " << client->root->m_yAngleChangeFactor << endl;
	cout << "clientX: " << client->m_xMouse << endl;
	cout << "clientY: " << client->m_yMouse << endl;*/
	if (client->m_xMouse != x) {
	client->root->m_xAngleChange = float((float)x-client->m_xMouse)/client->root->m_xAngleChangeFactor;
	}

	if (client->m_yMouse != y) {
	client->root->m_yAngleChange = float((float)y-client->m_yMouse)/client->root->m_yAngleChangeFactor;
	}
	
	// keeps mouse centered
	if (x != m_width/2 || y != m_height/2) {
		glutWarpPointer(m_width/2, m_height/2);
		
		// TODO: actually fix this
		/*cout << "xchange: " << client->root->m_xAngleChange << endl;
		cout << "ychange: " << client->root->m_yAngleChange << endl;
		client->root->m_xAngleChange = -1.0f * client->root->m_xAngleChange;
		client->root->m_yAngleChange = -1.0f * client->root->m_yAngleChange;
		cout << "xchange: " << client->root->m_xAngleChange << endl;
		cout << "ychange: " << client->root->m_yAngleChange << endl;*/
	}

	client->m_xMouse = x;
	client->m_yMouse = y;
	
}
