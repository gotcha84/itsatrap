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
bool *Window::specialKeyState = new bool[256];

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
	client->root->getPlayer()->getCamera()->m_cameraCenter.y += 4.0f;
	client->root->getPlayer()->getCamera()->m_cameraLookAt.y += 4.0f;

	// updates player view
	client->root->getPlayer()->getCamera()->updateCameraMatrix();
	client->root->getPlayer()->updateModelViewMatrix(); // andre added this line
	client->root->draw();

	client->root->getPlayer()->getCamera()->m_cameraCenter.y -= 4.0f;
	client->root->getPlayer()->getCamera()->m_cameraLookAt.y -= 4.0f;

	//cout << glm::to_string(client->root->getPlayer()->getPhysics()->m_velocity) << endl;

	glFlush();  
	glutSwapBuffers();
}

void Window::keyDown(unsigned char key, int x, int y)
{
	keyState[key] = true;
	//cout << key << " down" << endl;
}

void Window::keyUp(unsigned char key, int x, int y) {
	keyState[key] = false;
	//cout << key << " up" << endl;
}

void Window::specialKeyDown(int key, int x, int y) {
	specialKeyState[key] = true;
	//cout << "special " << key << " down" << endl;
}

void Window::specialKeyUp(int key, int x, int y) {
	specialKeyState[key] = false;
	//cout << "special " << key << " up" << endl;
}

void Window::processKeys() {
	// esc to quit
	if (keyState[27]) {
		exit(0);
	}

	// forward + backward
	if (keyState['w']) {
		client->root->getPlayer()->handleMovement('w');
	}
	else if (keyState['s']) {
		client->root->getPlayer()->handleMovement('s');
	}

	// left + right
	if (keyState['a']) {
		client->root->getPlayer()->handleMovement('a');
	}
	else if (keyState['d']) {
		client->root->getPlayer()->handleMovement('d');
	}

	// jump
	if (keyState[' ']) {
		client->root->getPlayer()->handleJump();
	}

	// trap
	if (keyState['t']) {
		sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition());
		//client->root->addChild(trap);
		Client::sendSpawnTrapEvent(trap->getTrapObjectForNetworking());
		//delete trap;
		//trap = nullptr;
	}

	// reload config file
	if (keyState['r']) {
		ConfigSettings::getConfig()->reloadSettingsFile();
		int testVal = 0;
		ConfigSettings::getConfig()->getValue("ScreenWidth", testVal);
		cout << "Reloaded config: ScreenWidth: " << testVal << endl;
	}

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
							bool hit = client->root->knifeHitWith(client->players[i]);

							if (hit)
							{
								Client::sendKnifeHitEvent(i);

								cout << "Player " << client->root->getPlayerID() << " hit Player " << i << "!" << endl;
								cout << "Player " << i << " has " << client->players[i]->getHealth() << endl;
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
