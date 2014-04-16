#include "Window.h"

static double spin_angle = 0.001;
static GLfloat red = 0.0, green = 2.0, blue = 0.0;

Window::Window() {
	m_width  = 512;   // set window width in pixels here
	m_height = 512;   // set window height in pixels here

	m_xMouse = 0;
	m_yMouse = 0;

	m_xAngleChange = 0.0f;
	m_yAngleChange = 0.0f;

	m_xAngleChangeFactor = 0.0f;
	m_yAngleChangeFactor = 0.0f;

	x = 0;
	inc = 0.0;

	trans = new MatrixTransform();
}

Window::~Window() {
	delete trans;
	trans = nullptr;
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
	//cube.spin(spin_angle); // rotate cube; if it spins too fast try 0.001
	
	if (x > 10 || x < -10) {
		inc = -1*inc;
	}
	x+=inc;
	trans->setMatrix(glm::translate(glm::vec3(0,x,0)));
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
void Window::displayCallback(void)
{
	m_myPlayer.updateModelViewMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(m_myPlayer.m_modelviewMatrix));

	// Draw sides of cube in object coordinate system:
	glBegin(GL_QUADS);
	glColor3f(red, green, blue);

	// Draw front face:
	glNormal3f(0.0, 0.0, 1.0);   
	glVertex3f(-5.0,  5.0,  5.0);
	glVertex3f( 5.0,  5.0,  5.0);
	glVertex3f( 5.0, -5.0,  5.0);
	glVertex3f(-5.0, -5.0,  5.0);
	
	// Draw left side:
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-5.0,  5.0,  5.0);
	glVertex3f(-5.0,  5.0, -5.0);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f(-5.0, -5.0,  5.0);
	
	// Draw right side:
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f( 5.0,  5.0,  5.0);
	glVertex3f( 5.0,  5.0, -5.0);
	glVertex3f( 5.0, -5.0, -5.0);
	glVertex3f( 5.0, -5.0,  5.0);
	
	// Draw back face:
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-5.0,  5.0, -5.0);
	glVertex3f( 5.0,  5.0, -5.0);
	glVertex3f( 5.0, -5.0, -5.0);
	glVertex3f(-5.0, -5.0, -5.0);
	
	// Draw top side:
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-5.0,  5.0,  5.0);
	glVertex3f( 5.0,  5.0,  5.0);
	glVertex3f( 5.0,  5.0, -5.0);
	glVertex3f(-5.0,  5.0, -5.0);
	
	// Draw bottom side:
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f( 5.0, -5.0, -5.0);
	glVertex3f( 5.0, -5.0,  5.0);
	glVertex3f(-5.0, -5.0,  5.0);
	glEnd();
	
	
	if (Window::m_xAngleChange != 0.0f) {
		if (Window::m_xAngleChange < 0) {
			m_myPlayer.m_cam.handleXRotation('l');
		}
		else {
			m_myPlayer.m_cam.handleXRotation('r');
		}
		Window::m_xAngleChange = 0.0f;
	}
	// TODO test
	if (Window::m_yAngleChange != 0.0f) {
		if (Window::m_yAngleChange < 0) {
			m_myPlayer.m_cam.handleYRotation('d');
		}
		else {
			m_myPlayer.m_cam.handleYRotation('u');
		}
		Window::m_yAngleChange = 0.0f;
	}
	

	glFlush();  
	glutSwapBuffers();
}

void Window::displaySceneGraph(void)
{
	// updates stuff
	m_myPlayer.updateModelViewMatrix(); // andre added this line
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(m_myPlayer.m_modelviewMatrix)); // andre added this line

	root->draw(m_myPlayer.m_modelviewMatrix);

	// andre added below if sattements
	if (Window::m_xAngleChange != 0.0f) {
		if (Window::m_xAngleChange < 0) {
			m_myPlayer.m_cam.handleXRotation('l');
		}
		else {
			m_myPlayer.m_cam.handleXRotation('r');
		}
		Window::m_xAngleChange = 0.0f;
	}
	// TODO test
	if (Window::m_yAngleChange != 0.0f) {
		if (Window::m_yAngleChange < 0) {
			m_myPlayer.m_cam.handleYRotation('d');
		}
		else {
			m_myPlayer.m_cam.handleYRotation('u');
		}
		Window::m_yAngleChange = 0.0f;
	}


	glFlush();  
	glutSwapBuffers();
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
		m_myPlayer.handleMovement(key);
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
	
	if (m_xMouse != x) {
	m_xAngleChange = float(m_xMouse-x)/m_xAngleChangeFactor;
	}

	if (m_yMouse != y) {
	m_yAngleChange = float(m_yMouse-y)/m_yAngleChangeFactor;
	}
	

	// keeps mouse centered
	/*if (x != m_width/2 || y != m_height/2) {
	glutWarpPointer(m_width/2, m_height/2);
	}*/

	m_xMouse = x;
	m_yMouse = y;
	
}
