#include "cube.h"

using namespace std;

static Cube cube;
static double spin_angle = 0.001;
static GLfloat red = 0.0, green = 2.0, blue = 0.0;

int Window::m_width  = 512;   // set window width in pixels here
int Window::m_height = 512;   // set window height in pixels here

int Window::m_xMouse = 0;
int Window::m_yMouse = 0;

float Window::m_xAngleChange = 0.0f;
float Window::m_yAngleChange = 0.0f;

float Window::m_xAngleChangeFactor = 0.0f;
float Window::m_yAngleChangeFactor = 0.0f;

//int, m_yMouse;				// mouse position
//
//static float m_xAngleChange, m_yAngleChange;
//
//static float m_xAngleChangeFactor, m_yAngleChangeFactor;

MatrixTransform *root;
MatrixTransform *trans;
float x = 0;
float inc = 0.01;

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
	cube.m_player.updateModelViewMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cube.m_player.m_modelviewMatrix));

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
			cube.m_player.m_cam.handleXRotation('l');
		}
		else {
			cube.m_player.m_cam.handleXRotation('r');
		}
		Window::m_xAngleChange = 0.0f;
	}
	// TODO test
	if (Window::m_yAngleChange != 0.0f) {
		if (Window::m_yAngleChange < 0) {
			cube.m_player.m_cam.handleYRotation('d');
		}
		else {
			cube.m_player.m_cam.handleYRotation('u');
		}
		Window::m_yAngleChange = 0.0f;
	}
	

	glFlush();  
	glutSwapBuffers();
}

void Window::displaySceneGraph(void)
{
	// updates stuff
	cube.m_player.updateModelViewMatrix(); // andre added this line
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cube.m_player.m_modelviewMatrix)); // andre added this line

	root->draw();

	// andre added below if sattements
	if (Window::m_xAngleChange != 0.0f) {
		if (Window::m_xAngleChange < 0) {
			cube.m_player.m_cam.handleXRotation('l');
		}
		else {
			cube.m_player.m_cam.handleXRotation('r');
		}
		Window::m_xAngleChange = 0.0f;
	}
	// TODO test
	if (Window::m_yAngleChange != 0.0f) {
		if (Window::m_yAngleChange < 0) {
			cube.m_player.m_cam.handleYRotation('d');
		}
		else {
			cube.m_player.m_cam.handleYRotation('u');
		}
		Window::m_yAngleChange = 0.0f;
	}


	glFlush();  
	glutSwapBuffers();
}

Cube::Cube()
{
	m_player = Player();
	angle = 0.0;
}

/*
Matrix4& Cube::getMatrix()
{
	return matrix;
}

void Cube::setMatrix(Matrix4 &m)
{
	matrix = Matrix4(m);
}
*/

double Cube::getAngle()
{
	return angle;
}

void Cube::setAngle(double a)
{
	angle = a;
}

void Cube::spin(double deg)
{
	if (cube.angle > 360.0 || cube.angle < -360.0) cube.angle = 0.0;
	//cube.getMatrix().rotateY(deg);
}

int main(int argc, char *argv[])
{

	cube.m_player.m_xWalkFactor = 1.0f;
	cube.m_player.m_zWalkFactor = 1.0f;

	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};  // lightsource position

	glutInit(&argc, argv);                      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(Window::m_width, Window::m_height);      // set initial window size
	glutCreateWindow("OpenGL Cube for CSE167");           // open window and set window title

	glEnable(GL_DEPTH_TEST);                    // enable depth buffering
	glClear(GL_DEPTH_BUFFER_BIT);               // clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);           // set clear color to black
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
	glDisable(GL_CULL_FACE);     // disable backface culling to render both sides of polygons
	glShadeModel(GL_SMOOTH);                    // set shading to smooth
	glMatrixMode(GL_PROJECTION); 
	
	// Generate material properties:
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	// Generate light source:
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	// Install callback functions:
	//glutDisplayFunc(Window::displayCallback);
	glutDisplayFunc(Window::displaySceneGraph);
	glutReshapeFunc(Window::reshapeCallback);
	glutIdleFunc(Window::idleCallback);

	// to avoid cube turning white on scaling down
	glEnable(GL_NORMALIZE);

	// Process input
	glutKeyboardFunc(Window::processNormalKeys);
	glutPassiveMotionFunc(Window::processMouseMove);
	
	// load obj files

	// hide mouse cursor
	//glutSetCursor(GLUT_CURSOR_NONE);
	
	// Initialize cube matrix:
	//cube.getMatrix().identity();
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective(90, float(Window::m_width)/float(Window::m_height), 0.1, 10000);


	cout << "initialized" << endl;
	/*
	Vector3 pos = Vector3(
	cube.getMatrix().m[3][0], 
	cube.getMatrix().m[3][1], 
	cube.getMatrix().m[3][2]
	);
	pos.print();
	*/
	
	root = new MatrixTransform();

	trans = new MatrixTransform();
	root->addChild(trans);

	Sphere *sphere = new Sphere();
	trans->addChild(sphere);

	glutMainLoop();

	return 0;
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
		cube.m_player.handleMovement(key);
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
	if (x != m_width/2 || y != m_height/2) {
	glutWarpPointer(m_width/2, m_height/2);
	}

	m_xMouse = x;
	m_yMouse = y;
	
}
