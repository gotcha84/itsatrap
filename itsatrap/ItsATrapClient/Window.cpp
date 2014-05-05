#include "Window.h"
#include "ClientInstance.h"
#include "Client.h"

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
	if (client->m_xAngleChange != 0.0f) {
		if (client->m_xAngleChange < 0) {
			client->root->getCamera()->handleXRotation(client->m_xAngleChange);
		}
		else {
			client->root->getCamera()->handleXRotation(client->m_xAngleChange);
		}
		client->m_xAngleChange = 0.0f;
	}
	
	// TODO test
	
	if (client->m_yAngleChange != 0.0f) {
		if (client->m_yAngleChange < 0) {
			client->root->getPlayer()->getCamera()->handleYRotation(client->m_yAngleChange);
		}
		else {
			client->root->getPlayer()->getCamera()->handleYRotation(client->m_yAngleChange);
		}
		client->m_yAngleChange = 0.0f;
	}

	glm::vec3 oldPos = client->root->getPlayer()->getPhysics()->m_position;

	// cout << "lookat: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraLookAt) << endl;
	// cout << "center: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraCenter) << endl;
	// cout << "lookup: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraUp) << endl << endl;

	// TODO: move to player class?
	client->root->getPlayer()->getPhysics()->applyGravity();

	if (oldPos != client->root->getPlayer()->getPhysics()->m_position) {
		client->root->getPlayer()->setModelMatrix(glm::translate(client->root->getPlayer()->getPhysics()->m_position));
		client->root->getPlayer()->updateBoundingBox();
		Client::sendStateUpdate(Client::getPlayerId(), client->root->getPlayer()->getPhysics()->m_position.x, client->root->getPlayer()->getPhysics()->m_position.y, client->root->getPlayer()->getPhysics()->m_position.z);
	}

	glm::vec3 moved = client->root->getPlayer()->getPhysics()->m_position - client->root->getPlayer()->getCamera()->m_cameraCenter;
	//moved.y += 4.0f;
	
	//client->root->getPlayer()->getCamera()->m_cameraCenter = client->root->getPlayer()->getPhysics()->m_position;
	client->root->getPlayer()->getCamera()->m_cameraCenter += moved;
	client->root->getPlayer()->getCamera()->m_cameraLookAt += moved;
	client->root->getPlayer()->getCamera()->m_cameraCenter.y += 4.0f;
	client->root->getPlayer()->getCamera()->m_cameraLookAt.y += 4.0f;
	//client->root->getPlayer()->getCamera()->updateCameraMatrix();
	//cout << "cam is: " << glm::to_string(client->root->getPlayer()->getPhysics()->m_position) << endl;

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
	cout << key << " down" << endl;
}

void Window::keyUp(unsigned char key, int x, int y) {
	keyState[key] = false;
	cout << key << " up" << endl;
}

void Window::specialKeyDown(int key, int x, int y) {
	specialKeyState[key] = true;
	cout << "special " << key << " down" << endl;
}

void Window::specialKeyUp(int key, int x, int y) {
	specialKeyState[key] = false;
	cout << "special " << key << " up" << endl;
}

void Window::processKeys() {
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
			sg::Trap *trap = new sg::Trap(client->root->getPosition());
			client->root->addChild(trap);
		}

		//case 9: // TAB
			//client->toggleCurrentPlayer();
			//client->printSceneGraph();
			//break;
}

void Window::processMouseMove(int x, int y) {
	
	/*cout << "client factor: " << client->m_xAngleChangeFactor << endl;
	cout << "client factor: " << client->m_yAngleChangeFactor << endl;
	cout << "clientX: " << client->m_xMouse << endl;
	cout << "clientY: " << client->m_yMouse << endl;*/
	if (client->m_xMouse != x) {
	client->m_xAngleChange = float((float)x-client->m_xMouse)/client->m_xAngleChangeFactor;
	}

	if (client->m_yMouse != y) {
	client->m_yAngleChange = float((float)y-client->m_yMouse)/client->m_yAngleChangeFactor;
	}
	
	// keeps mouse centered
	if (x != m_width/2 || y != m_height/2) {
		glutWarpPointer(m_width/2, m_height/2);
		
		// TODO: actually fix this
		/*cout << "xchange: " << client->m_xAngleChange << endl;
		cout << "ychange: " << client->m_yAngleChange << endl;
		client->m_xAngleChange = -1.0f * client->m_xAngleChange;
		client->m_yAngleChange = -1.0f * client->m_yAngleChange;
		cout << "xchange: " << client->m_xAngleChange << endl;
		cout << "ychange: " << client->m_yAngleChange << endl;*/
	}

	client->m_xMouse = x;
	client->m_yMouse = y;
	
}
