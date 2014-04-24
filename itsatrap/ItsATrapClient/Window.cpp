#include "Window.h"
#include "ClientInstance.h"

extern ClientInstance *client;

int Window::m_width  = 512; // set window width in pixels here
int Window::m_height = 512; // set window height in pixels here

int Window::m_heightMapXShift = 278;
int Window::m_heightMapZShift = 463;

Window::Window() {

}

Window::~Window() {

}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
	displaySceneGraph(); // call display routine to re-draw cube
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
void Window::displaySceneGraph(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	
	// andre added below if sattements
	if (client->m_xAngleChange != 0.0f) {
		if (client->m_xAngleChange < 0) {
			client->root->getCamera()->handleXRotation('l');
		}
		else {
			client->root->getCamera()->handleXRotation('r');
		}
		client->m_xAngleChange = 0.0f;
	}

	// TODO test
	/*
	if (client->m_yAngleChange != 0.0f) {
		if (client->m_yAngleChange < 0) {
			client->root->getPlayer()->getCamera()->handleYRotation('u');
		}
		else {
			client->root->getPlayer()->getCamera()->handleYRotation('d');
		}
		client->m_yAngleChange = 0.0f;
	}*/
	/*
	cout << "lookat: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraLookAt) << endl;
	cout << "center: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraCenter) << endl;
	cout << "lookup: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraUp) << endl << endl;
	*/
	// TODO: move to player class?
	client->root->getPlayer()->getPhysics()->applyGravity();
	glm::vec3 moved = client->root->getPlayer()->getPhysics()->m_position - client->root->getPlayer()->getCamera()->m_cameraCenter;
	//moved.y += 4.0f;
	client->root->getPlayer()->getCamera()->m_cameraLookAt += moved;
	client->root->getPlayer()->getCamera()->m_cameraCenter = client->root->getPlayer()->getPhysics()->m_position;
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

	glFlush();  
	glutSwapBuffers();
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	// TODO: maybe more states
	if (/*(client->m_myPlayer.m_physics.m_currentState != PhysicsStates::Falling) && */ (key == 'w' || key == 'a' || key == 's' || key == 'd')) {
		client->root->getPlayer()->handleMovement(key);
	}

	switch (key) {
		case 9: // TAB
			client->toggleCurrentPlayer();
			client->printSceneGraph();
			break;
	}
	
	// TODO: running

	/*
	Vector3 pos = Vector3(
	cube.getMatrix().m[3][0], 
	cube.getMatrix().m[3][1], 
	cube.getMatrix().m[3][2]
	);

	pos.print();
	*/
}

void Window::processMouseMove(int x, int y) {
	
	/*cout << "client factor: " << client->m_xAngleChangeFactor << endl;
	cout << "client factor: " << client->m_yAngleChangeFactor << endl;
	cout << "clientX: " << client->m_xMouse << endl;
	cout << "clientY: " << client->m_yMouse << endl;*/
	if (client->m_xMouse != x) {
	client->m_xAngleChange = float(client->m_xMouse-x)/client->m_xAngleChangeFactor;
	}

	if (client->m_yMouse != y) {
	client->m_yAngleChange = float(client->m_yMouse-y)/client->m_yAngleChangeFactor;
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
