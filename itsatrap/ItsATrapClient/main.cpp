#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "SceneGraph.h"
#include "ClientInstance.h"
#include "Window.h"
#include "World.h"
#include "Client.h"

ClientInstance client = ClientInstance();

int Window::m_width  = 512;   // set window width in pixels here
int Window::m_height = 512;   // set window height in pixels here

float Window::spin_angle = 0.001f;

GLfloat Window::red = 0.0;
GLfloat Window::green = 2.0;
GLfloat Window::blue = 0.0;

int main(int argc, char *argv[])
{

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
	
	//glMatrixMode(GL_PROJECTION);
	//gluPerspective(90, float(Window::m_width)/float(Window::m_height), 0.1, 10000);

	// Initialize networking for client
	Client::initializeClient();

	/*
	Vector3 pos = Vector3(
	cube.getMatrix().m[3][0], 
	cube.getMatrix().m[3][1], 
	cube.getMatrix().m[3][2]
	);
	pos.print();
	*/
	
	// ground nodes
	//World world = World();
	//world.loadWorldData();
	//world.initializeHeightMap();

	// player 1
	sg::Player p1 = sg::Player();
	p1.moveTo(glm::vec3(0,0,100.0f));
	p1.lookIn(glm::vec3(0,0,-1.0f));


	// 2nd player
	sg::Player p2 = sg::Player();
	p2.moveTo(glm::vec3(0,0,0.0f));
	p2.lookIn(glm::vec3(0,0,-1.0f));

	// set root node
	client.root = &p1;
	client.root->addChild(&p2);

	sg::MatrixTransform ground = sg::MatrixTransform();
	client.root->addChild(&ground);
	sg::Cube groundShape = sg::Cube();
	ground.addChild(&groundShape);

	ground.setMatrix(glm::translate(glm::vec3(0,-10,0)) * glm::scale(glm::vec3(100,0.1,100)));
	groundShape.color = glm::vec3(0,1,0);

	// cube nodes
	sg::MatrixTransform obj1 = sg::MatrixTransform();
	//client.root->addChild(&obj1);
	sg::Cube obj1Shape = sg::Cube();
	obj1.addChild(&obj1Shape);

	obj1.setMatrix(glm::translate(glm::vec3(0,-5,0)) * glm::scale(glm::vec3(10,10,10)));

	cout << "center: " << glm::to_string(client.root->getCamera()->m_cameraCenter) << endl;

	glutMainLoop();

	return 0;
}
