#include "cube.h"

using namespace std;

static Cube cube;
static double spin_angle = 0.001;
static double red = 0.0, green = 2.0, blue = 0.0;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
	cube.spin(spin_angle); // rotate cube; if it spins too fast try 0.001
	displayCallback(); // call display routine to re-draw cube
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
	width = w;
	height = h;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixd(cube.getMatrix().getPointer());

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
	
	/*
	if (Window::angle_x_change != 0.0f) {
	//cam.CameraXMovement();
	Window::angle_x_change = 0.0f;
	}
	if (Window::angle_y_change != 0.0f) {
	//cam.CameraYMovement();
	Window::angle_y_change = 0.0f;
	}
	*/

	glFlush();  
	glutSwapBuffers();
}

Cube::Cube()
{
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

	cube.walk_x_factor = 1.0f;
	cube.walk_z_factor = 1.0f;

	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};  // lightsource position

	glutInit(&argc, argv);                      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(Window::width, Window::height);      // set initial window size
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
	glutDisplayFunc(Window::displayCallback);
	glutReshapeFunc(Window::reshapeCallback);
	glutIdleFunc(Window::idleCallback);

	// to avoid cube turning white on scaling down
	glEnable(GL_NORMALIZE);

	// Process keyboard input
	glutKeyboardFunc(Window::processNormalKeys);

	glutPassiveMotionFunc(Window::processMouseMove);
	
	// load obj files

	// hide mouse cursor
	//glutSetCursor(GLUT_CURSOR_NONE);
	
	// Initialize cube matrix:
	//cube.getMatrix().identity();
	
	cout << "initialized" << endl;
	/*
	Vector3 pos = Vector3(
	cube.getMatrix().m[3][0], 
	cube.getMatrix().m[3][1], 
	cube.getMatrix().m[3][2]
	);
	pos.print();
	*/
	
	glutMainLoop();
	return 0;
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	/*
	Vec3 oldCameraCenter = cameraCenter;
	Vec3 oldCameraLookAt = cameraLookAt;
	Vec3 oldCameraUp = cameraUp;
	Vec3 proposedCameraDiff = cameraLookAt - cameraDiff;
	proposedCameraDiff.normalize();
	// make y rotation matrix
	proposedxCameraDiff = // rotate vector by 90degrees clockwise
	*/

	switch (key) {
	// go forward
	case 'w':
		//Vec3 proposedCameraDiff = cube.walk_z_factor*(proposedCameraDiff);
		//cam.Movement('w', proposedCameraDiff);

		cout << "w" << endl;
	
		/*
		// returns the new position we want
		Vec3 newCameraCenter = physics.collisionDetected(oldCameraCenter, proposedCameraDiff);
		
		Vec3 actualCameraDiff = newCameraCenter - oldCameraCenter;
		Vec3 newCameraLookAt = oldCameraLookAt + actualCameraDiff;

		camera.updateModelViewMatrix(newCameraCenter, newCameraLookAt, oldCameraUp);
		*/
		break;

	// go backwards
	case 's':
		//Vec3 proposedCameraDiff = -1*cube.walk_z_factor*(proposedCameraDiff);
		//cam.Movement('s', proposedCameraDiff);
		cout << "s" << endl;
		break;
	// strafe left
	case 'a':
		//Vec3 proposedCameraDiff = -1*cube.walk_z_factor*(proposedxCameraDiff);
		//cam.Movement('a', proposedCameraDiff);
		cout << "a" << endl;
		break;
	// strafe right
	case 'd':
		//Vec3 proposedCameraDiff = cube.walk_z_factor*(proposedxCameraDiff);
		//cam.Movement('d', proposedCameraDiff);
		cout << "d" << endl;
		break;
		
		// TODO: running  

	}

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
	/*
	if (mouse_X != x) {
	angle_x_change = float(mouse_X-x)/angle_x_change_factor;
	}

	if (mouse_Y != y) {
	angle_y_change = float(mouse_Y-y)/angle_y_change_factor;
	}
		
	if (x != Window::width/2 || y != Window::height/2) {
	glutWarpPointer(Window::width/2, Window::height/2);
	}

	mouse_X = x;
	mouse_Y = y;
	*/
}