#include "Window.h"
#include "ClientInstance.h"
#include "Client.h"

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

	// cout << "lookat: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraLookAt) << endl;
	// cout << "center: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraCenter) << endl;
	// cout << "lookup: " << glm::to_string(client->root->getPlayer()->getCamera()->m_cameraUp) << endl << endl;

	// TODO: move to player class?
	client->root->getPlayer()->getPhysics()->applyGravity();
	glm::vec3 moved = client->root->getPlayer()->getPhysics()->m_position - client->root->getPlayer()->getCamera()->m_cameraCenter;
	
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
	
	glFlush();  
	glutSwapBuffers();
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	// TODO: maybe more states
	if (/*(client->m_myPlayer.m_physics.m_currentState != PhysicsStates::Falling) && */ (key == 'w' || key == 'a' || key == 's' || key == 'd')) {
		client->root->handleMovement(key);
		Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	}

	switch (key) {
		case 9: // TAB
			client->toggleCurrentPlayer();
			client->printSceneGraph();
			Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
			break;
		case 't':
			sg::Trap *trap = new sg::Trap(Client::getPlayerId(), client->root->getPosition());
			Client::requestToSpawnTrap(trap->getTrapObjectForNetworking());
			delete trap;
			trap = nullptr;
			break;
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
					int numPlayers = client->players.size();
					for (int i = 0; i < client->players.size(); ++i)
					{
						if (i != client->root->getPlayerID())
						{
							bool hit = client->root->knifeHitWith(client->players[i]);

							if (hit)
							{
								Client::sendKnifeHitEvent(i);
								cout << "Player " << client->root->getPlayerID() << " hit Player " << i << "!" << endl;
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
