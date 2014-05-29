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
bool *Window::specialKeyState = new bool[256];
int Window::modifierKey = 0;

Window::Window() {
	for (int i=0; i<256; i++) {
		keyState[i] = false;
		specialKeyState[i] = false;
	}
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
	PhysicsStates curr_state = client->root->getPlayer()->getPhysics()->m_currentState;
	bool sendUpdate = false;

	//cout << "curr_state: " << curr_state << endl;

	//cout << "position: " << glm::to_string(client->root->getPlayer()->getPosition()) << endl;
	processKeys();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	
	// TODO: lock movement if looking up?
	if (client->root->m_xAngleChange != 0.0f) {
		client->root->handleXRotation(client->root->m_xAngleChange);
		client->root->m_xAngleChange = 0.0f;
		//Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}
	
	if (client->root->m_yAngleChange != 0.0f) {
		client->root->handleYRotation(client->root->m_yAngleChange);
		client->root->m_yAngleChange = 0.0f;
		//Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}

	if (oldXRotated != client->root->getCamera()->getXRotated() || oldYRotated != client->root->getCamera()->getYRotated()) {
		sendUpdate = true;
		/*if (oldYRotated != client->root->getCamera()->getYRotated()) {
			cout << "yrotated: " << client->root->getCamera()->getYRotated() << endl;
		}*/
	}

	
	
	glm::vec3 oldPos = client->root->getPlayer()->getPosition();
	if (curr_state == PhysicsStates::Sliding) {
		client->root->getPlayer()->handleSliding();
	}

	if (curr_state == PhysicsStates::Climbing) {
		client->root->getPlayer()->applyClimbing();
	}

	if (curr_state == PhysicsStates::PullingUp) {
		client->root->getPlayer()->applyPullingUp();
	}

	if (curr_state == PhysicsStates::HoldingEdge) {
		client->root->getPlayer()->applyHoldingEdge();
	}

	int buildingId = client->root->getPlayer()->getPhysics()->applyGravity(client->root->getPlayer()->getAABB());

	if (!client->root->getPlayer()->m_deathState) {
		if (buildingId != -2) {
			sendUpdate = true;
			client->root->getPlayer()->m_onTopOfBuildingId = buildingId;
		}
	}

	if (oldBuildingId != client->root->getPlayer()->m_onTopOfBuildingId) {
		cout << "old building id: " << oldBuildingId << endl;
		cout << "new building id:" << client->root->getPlayer()->m_onTopOfBuildingId << endl;
	}
	
	client->root->getPlayer()->getPhysics()->m_velocity += client->root->getPlayer()->getPhysics()->m_velocityDiff;

	
	/*if (curr_state == PhysicsStates::PullingUp || curr_state == PhysicsStates::Climbing) {
		cout << "velo: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocity) << endl;
		cout << "velodiff: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocityDiff) << endl;
	}*/

	client->root->getPlayer()->getPhysics()->m_position += client->root->getPlayer()->getPhysics()->m_velocity;

	/*if (client->root->getPlayer()->getPhysics()->m_velocity.x != client->root->getPlayer()->getPhysics()->m_velocityDiff.x || client->root->getPlayer()->getPhysics()->m_velocity.z != client->root->getPlayer()->getPhysics()->m_velocityDiff.z) {

		cout << "velo: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocity) << endl;
		cout << "velodiff: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocityDiff) << endl;

	}*/
	
	//client->root->getPlayer()->getPhysics()->m_velocity = glm::vec3(0.0f, client->root->getPlayer()->getPhysics()->m_velocity.y, 0.0f);
	
	client->root->getPlayer()->getPhysics()->m_velocity -= client->root->getPlayer()->getPhysics()->m_velocityDiff;
	//client->root->getPlayer()->getPhysics()->m_velocity.x -= client->root->getPlayer()->getPhysics()->m_velocityDiff.x;
	//client->root->getPlayer()->getPhysics()->m_velocity.z -= client->root->getPlayer()->getPhysics()->m_velocityDiff.z;

	client->root->getPlayer()->getPhysics()->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);

	client->root->getPlayer()->getPhysics()->m_lastMoved = glm::vec3(0.0f, 0.0f, 0.0f);

	if (oldPos != client->root->getPlayer()->getPosition()) {		
		client->root->getPlayer()->getPhysics()->m_lastMoved = client->root->getPlayer()->getPosition() - oldPos;
		client->root->getPlayer()->setModelMatrix(glm::translate(client->root->getPlayer()->getPhysics()->m_position/* + client->root->getPlayer()->getPhysics()->m_velocity*/));
		client->root->getPlayer()->updateBoundingBox();
		sendUpdate = true;
	}

	if (sendUpdate) {
		Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}

	/*if (client->root->getPlayer()->getCamera()->m_cameraCenter != oldPos) {
		cout << "oldpos: " << glm::to_string(oldPos) << endl;
		cout << "cam center: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraCenter) << endl;
	}*/

	glm::vec3 moved = client->root->getPlayer()->getPhysics()->m_position - /*oldPos */client->root->getPlayer()->getCamera()->m_cameraCenter;
	
	client->root->getPlayer()->getCamera()->m_cameraCenter += moved;
	client->root->getPlayer()->getCamera()->m_cameraLookAt += moved;
	
	if (curr_state == PhysicsStates::Sliding) {
		client->root->getPlayer()->getCamera()->m_cameraCenter += client->root->getPlayer()->getCamera()->m_slidingHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt += client->root->getPlayer()->getCamera()->m_slidingHeight + client->root->getPlayer()->getCamera()->m_camZSliding;
	}
	else {
		client->root->getPlayer()->getCamera()->m_cameraCenter += client->root->getPlayer()->getCamera()->m_playerHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt += client->root->getPlayer()->getCamera()->m_playerHeight;
	}


	// updates player view
	client->root->getPlayer()->getCamera()->updateCameraMatrix();
	client->root->getPlayer()->updateModelViewMatrix();
	client->root->draw();

	if (curr_state == PhysicsStates::Sliding) {
		client->root->getPlayer()->getCamera()->m_cameraCenter -= client->root->getPlayer()->getCamera()->m_slidingHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt -= (client->root->getPlayer()->getCamera()->m_slidingHeight + client->root->getPlayer()->getCamera()->m_camZSliding);
	}
	else {
		client->root->getPlayer()->getCamera()->m_cameraCenter -= client->root->getPlayer()->getCamera()->m_playerHeight;
		client->root->getPlayer()->getCamera()->m_cameraLookAt -= client->root->getPlayer()->getCamera()->m_playerHeight;
	}

	client->root->getPlayer()->getCamera()->updateCameraMatrix();
	client->root->getPlayer()->updateModelViewMatrix();

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
		sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->getXRotated());
		if (key == '2') {
			trap->loadModel("../Models/Can.obj", "../Models/");
		}
		else {
			trap->loadModel("../Models/Polynoid.obj", "../Models/");
		}
		
		struct trapObject t = trap->getTrapObjectForNetworking();
		switch (key) {
			case '1':
				cout << "TRAP 1" << endl;
				t.type = TYPE_FREEZE_TRAP;
				break;
			case '2':
				cout << "TRAP 2" << endl;
				t.type = TYPE_TRAMPOLINE_TRAP;
				break;
			case '3':
				cout << "TRAP 3" << endl;
				t.type = TYPE_SLOW_TRAP;
				break;
			case '4':
				cout << "TRAP 4" << endl;
				t.type = TYPE_PUSH_TRAP;
				break;
			case '5':
				cout << "TRAP 5" << endl;
				t.type = TYPE_LIGHTNING_TRAP;
				break;
			default:
				t.type = TYPE_FREEZE_TRAP;
				break;
		}

		Client::sendSpawnTrapEvent(t);
		delete trap;
		trap = nullptr;
	}
	else if (key == 'r') {
		ConfigSettings::getConfig()->reloadSettingsFile();
		Client::sendReloadConfigFile();
	}
}

void Window::keyUp(unsigned char key, int x, int y) {
	keyState[key] = false;
}

void Window::specialKeyDown(int key, int x, int y) {
	modifierKey = glutGetModifiers();
	
	specialKeyState[key] = true;
}

void Window::specialKeyUp(int key, int x, int y) {
	modifierKey = glutGetModifiers();

	specialKeyState[key] = false;
}

void Window::processKeys() {
	int count = 0;
	PhysicsStates curr_state = client->root->m_player->getPhysics()->m_currentState;
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
			if (curr_state != PhysicsStates::HoldingEdge) {
				client->root->getPlayer()->handleMovement('w');
			}
			else {
				client->root->getPlayer()->handleHoldingEdge('w');
			}
		}
	}
	else if (keyState['s']) {
		if (curr_state != PhysicsStates::HoldingEdge) {
			client->root->getPlayer()->handleMovement('s');
		}
		else {
			client->root->getPlayer()->handleHoldingEdge('s');
		}
	}

	// left + right
	if (keyState['a']) {
		if (curr_state != PhysicsStates::HoldingEdge) {
			client->root->getPlayer()->handleMovement('a');
		}
		else {
			client->root->getPlayer()->handleHoldingEdge('a');
		}
	}
	else if (keyState['d']) {
		if (curr_state != PhysicsStates::HoldingEdge) {
			client->root->getPlayer()->handleMovement('d');
		}
		else {
			client->root->getPlayer()->handleHoldingEdge('d');
		}
	}

	if (keyState['u']) {
		client->root->getPlayer()->Unstuck();
	}

	// jump
	if (keyState[' ']) {
		if (curr_state != PhysicsStates::HoldingEdge) {
			client->root->getPlayer()->handleJump();
		}
		else {
			client->root->getPlayer()->handleHoldingEdge(' ');
		}
	}

	// trap	
	//if (keyState['t']) {
	//	cout << "'t' trap pressed" << endl;
	//	sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition(), client->root->getCamera()->getXRotated());
	//	trap->loadModel("../Models/Polynoid.obj", "../Models/");
	//	struct trapObject t = trap->getTrapObjectForNetworking();
	//	t.type = TYPE_FREEZE_TRAP;
	//	Client::sendSpawnTrapEvent(t);
	//	delete trap;
	//}
	

	//case 9: // TAB
		//client->toggleCurrentPlayer();
		//client->printSceneGraph();
		//break;
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
					int numPlayers = client->players.size();
					for (int i = 0; i < numPlayers; ++i)
					{
						if (i != client->root->getPlayerID())
						{
							//bool hit = client->root->knifeHitWith(client->players[i]);

							//if (hit)
							//{
								Client::sendKnifeHitEvent(i);
							//}
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
	}

	client->m_xMouse = x;
	client->m_yMouse = y;
	
}
