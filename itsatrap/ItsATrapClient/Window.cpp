#include "Window.h"
#include "ClientInstance.h"
#include "Client.h"
#include "ConfigSettings.h"

extern ClientInstance *client;

int Window::m_width  = 512; // set window width in pixels here
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
ISound *walk;
ISound *freezeTrapSound;
ISound *pushSound;
ISound *tramSound;
ISound *slowSound;
ISound *lightningSound;
bool jump;


Window::Window() {
	for (int i=0; i<256; i++) {
		keyState[i] = false;
		specialKeyState[i] = false;
		keyEventTriggered[i] = false;
	}
	engine = createIrrKlangDevice(); //declare loop, pause, and track
	jumpSound = createIrrKlangDevice();
	knifeSound = createIrrKlangDevice();
	walk = engine->play2D("../SoundFiles/footstep.wav", true, true, true);
	freezeTrapSound = engine->play2D("../SoundFiles/trap.wav", true, true, true);
	pushSound = engine->play2D("../SoundFiles/push.wav", true, true, true);
	tramSound = engine->play2D("../SoundFiles/tram.wav", true, true, true);
	slowSound = engine->play2D("../SoundFiles/slow.wav", true, true, true);
	lightningSound = engine->play2D("../SoundFiles/lightning.wav", true, true, true);
	jump = true;
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
	glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
	//glTranslatef(0, 0, -20);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{	
	float oldBuildingId = client->root->getPlayer()->m_onTopOfBuildingId;
	float oldXRotated = client->root->getCamera()->getXRotated();
	float oldYRotated = client->root->getCamera()->getYRotated();
	
	bool sendUpdate = false, lookChanged = false;

	glm::vec3 oldPos = client->root->getPlayer()->getPosition();

	//cout << "curr_state: " << curr_state << endl;

	//cout << "position: " << glm::to_string(client->root->getPlayer()->getPosition()) << endl;
	processKeys();

	//PhysicsStates curr_state = client->root->getPlayer()->getPhysics()->m_currentState;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	
	// TODO: lock movement if looking up?
	if (client->root->m_xAngleChange != 0.0f) {
		client->root->handleXRotation(client->root->m_xAngleChange);
		client->root->m_xAngleChange = 0.0f;
		//Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
		lookChanged = true;
	}
	
	if (client->root->m_yAngleChange != 0.0f) {
		client->root->handleYRotation(client->root->m_yAngleChange);
		client->root->m_yAngleChange = 0.0f;
		//Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());

		lookChanged = true;
	}

	if (oldXRotated != client->root->getCamera()->getXRotated() || oldYRotated != client->root->getCamera()->getYRotated()) {
		sendUpdate = true;
		lookChanged = true;
		/*if (oldYRotated != client->root->getCamera()->getYRotated()) {
			cout << "yrotated: " << client->root->getCamera()->getYRotated() << endl;
		}*/
	}
	
	//client->root->getPlayer()->applyCamAdjustments();
	
	int buildingId = -2; //client->root->getPlayer()->getPhysics()->applyGravity(client->root->getPlayer()->getAABB());

	/*if (client->root->getPlayer()->m_timeUntilRespawn <= 0) {
		if (buildingId != -2 && oldBuildingId != buildingId) {
			sendUpdate = true;
			client->root->getPlayer()->m_onTopOfBuildingId = buildingId;
			cout << "old building id: " << oldBuildingId << endl;
			cout << "new building id:" << client->root->getPlayer()->m_onTopOfBuildingId << endl;
		}

	}*/

	

	/*if (client->root->getPlayer()->getPhysics()->m_velocity.x != client->root->getPlayer()->getPhysics()->m_velocityDiff.x || client->root->getPlayer()->getPhysics()->m_velocity.z != client->root->getPlayer()->getPhysics()->m_velocityDiff.z) {

		cout << "velo: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocity) << endl;
		cout << "velocityDiff: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocityDiff) << endl;

	}*/
	
	//client->root->getPlayer()->getPhysics()->m_velocity = glm::vec3(0.0f, client->root->getPlayer()->getPhysics()->m_velocity.y, 0.0f);
	
	if (oldPos != client->root->getPlayer()->getPosition()) {		
		//client->root->getPlayer()->getPhysics()->m_lastMoved = client->root->getPlayer()->getPosition() - oldPos;
		client->root->getPlayer()->setModelMatrix(glm::translate(client->root->getPlayer()->getPosition()/* + client->root->getPlayer()->getPhysics()->m_velocity*/));
		client->root->getPlayer()->updateBoundingBox();
		sendUpdate = true;
	}

	//if (sendUpdate) {
	//	Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	//}
	if (lookChanged)
		Client::sendLookEvent(client->root->getCameraObjectForNetworking());

	/*if (client->root->getPlayer()->getCamera()->m_cameraCenter != oldPos) {
		cout << "oldpos: " << glm::to_string(oldPos) << endl;
		cout << "cam center: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraCenter) << endl;
	}*/

	//cout << "m_position: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_position) << endl;
	//glm::vec3 moved = client->root->getPlayer()->getPosition() - /*oldPos */client->root->getPlayer()->getCamera()->m_cameraCenter;
	
	//client->root->getPlayer()->getCamera()->m_cameraCenter += moved;
	//client->root->getPlayer()->getCamera()->m_cameraLookAt += moved;
	
	/*if (client->root->getPlayer()->getPhysics()->m_currentState == PhysicsStates::Sliding) {
		client->root->getPlayer()->getCamera()->m_cameraCenter += client->root->getPlayer()->getCamera()->m_slidingHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt += client->root->getPlayer()->getCamera()->m_slidingHeight + client->root->getPlayer()->getCamera()->m_camZSliding;
	}
	else {*/
	//uncomment lower two for nonmidgets
		//client->root->getPlayer()->getCamera()->m_cameraCenter += client->root->getPlayer()->getCamera()->m_playerHeight;
		//client->root->getPlayer()->getCamera()->m_cameraLookAt += client->root->getPlayer()->getCamera()->m_playerHeight;
	//}


	// updates player view
	client->root->getPlayer()->getCamera()->updateCameraMatrix();
	client->root->getPlayer()->updateModelViewMatrix();
	client->root->draw();

	/*if (client->root->getPlayer()->getPhysics()->m_currentState == PhysicsStates::Sliding) {
		client->root->getPlayer()->getCamera()->m_cameraCenter -= client->root->getPlayer()->getCamera()->m_slidingHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt -= (client->root->getPlayer()->getCamera()->m_slidingHeight + client->root->getPlayer()->getCamera()->m_camZSliding);
	}
	else {*/
	// uncomment lower two for nonmidgets
		//client->root->getPlayer()->getCamera()->m_cameraCenter -= client->root->getPlayer()->getCamera()->m_playerHeight;
		//client->root->getPlayer()->getCamera()->m_cameraLookAt -= client->root->getPlayer()->getCamera()->m_playerHeight;
	//}

	//client->root->getPlayer()->getCamera()->updateCameraMatrix();
	//client->root->getPlayer()->updateModelViewMatrix();

	m_fpsCounter+=1;
				
	if (clock()-m_timer > 1000) {
		//cout << "FPS: " <<  m_fpsCounter/((clock() - m_timer)/1000.0) << '\n';
		m_timer = clock();
		m_fpsCounter = 0;
	}

	glFlush();  
	glutSwapBuffers();
}

void Window::keyDown(unsigned char key, int x, int y)
{
	keyState[key] = true;
	if (key >= '1' && key <= '9') {
		switch (key)
		{
		case '1':
			freezeTrapSound->setIsPaused(false);
			break;
		case '2':
			tramSound->setIsPaused(false);
			break;
		case '3':
			slowSound->setIsPaused(false);
			break;
		case '4':
			pushSound->setIsPaused(false);
			break;
		case '5':
			lightningSound->setIsPaused(false);
			break;
		case '6':
			freezeTrapSound->setIsPaused(false);
			break;
		default:
			freezeTrapSound->setIsPaused(false);
			break;
		}
	}
}

void Window::keyUp(unsigned char key, int x, int y) {
	keyState[key] = false;
	keyEventTriggered[key] = false;
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
			freezeTrapSound->setIsPaused(true);
			break;
		case '2':
			type = TYPE_TRAMPOLINE_TRAP;
			filename = TRAMPOLINE_TRAP_OBJ;
			tramSound->setIsPaused(true);
			break;
		case '3':
			type = TYPE_SLOW_TRAP;
			filename = SLOW_TRAP_OBJ;
			slowSound->setIsPaused(true);
			break;
		case '4':
			type = TYPE_PUSH_TRAP;
			filename = PUSH_TRAP_OBJ;
			pushSound->setIsPaused(true);
			break;
		case '5':
			type = TYPE_LIGHTNING_TRAP;
			filename = DEATH_TRAP_OBJ;
			lightningSound->setIsPaused(true);
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
			freezeTrapSound->setIsPaused(true);
			break;
		}
		sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->m_xRotated, TRAP_DIR + filename);
		struct trapObject t = trap->getTrapObjectForNetworking();
		t.type = type;
		Client::sendSpawnTrapEvent(t);
		delete trap;
		trap = nullptr;
	}
	else if (key == 'r') 
	{
		ConfigSettings::getConfig()->reloadSettingsFile();
		Client::sendReloadConfigFile();
	}
	else if (key == 'e')
	{
		// Resource Tower Hits
		for (int i = 0; i < client->level.resources.size(); ++i) {
			Client::sendChannelAttemptEvent(client->level.resources[i]->getResourceId());
		}
	}
}

void Window::specialKeyDown(int key, int x, int y) {
	modifierKey = glutGetModifiers();
	
	specialKeyState[key] = true;
}

void Window::specialKeyUp(int key, int x, int y) {
	modifierKey = glutGetModifiers();

	specialKeyState[key] = false;
	specialKeyEventTriggered[key] = false;
}

void Window::processKeys() {
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

			if (!jumpSound->isCurrentlyPlaying("../SoundFiles/jump.wav")) {
				jumpSound->play2D("../SoundFiles/jump.wav", false, false, true);
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

	// teleport
	/*if (modifierKey == GLUT_ACTIVE_ALT) {
		client->root->getPlayer()->handleTeleport();
	}*/

	// forward + backward
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
	
	client->tabPressed = keyState[9];
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

					if (!knifeSound->isCurrentlyPlaying("../SoundFiles/knife.wav")) {
						knifeSound->play2D("../SoundFiles/knife.wav", false, false, true);
					}

					// Player Hits
					Client::sendKnifeHitEvent();
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
	}

	client->m_xMouse = x;
	client->m_yMouse = y;
}
