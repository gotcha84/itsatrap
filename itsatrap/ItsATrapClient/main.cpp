#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// graphics
#include "ClientInstance.h"
#include "Window.h"
#include "SceneGraph.h"

// networking
#include "Client.h"

// Test
#include "enrico.h"

ClientInstance client = ClientInstance();
Window window = Window();

int main(int argc, char *argv[]) {
	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};  // lightsource position

	glutInit(&argc, argv);                      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(window.m_width, window.m_height);      // set initial window size
	glutCreateWindow("It's a Trap!");           // open window and set window title

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
	//glutDisplayFunc(window.displayCallback);
	glutDisplayFunc(window.displaySceneGraph);
	glutReshapeFunc(window.reshapeCallback);
	glutIdleFunc(window.idleCallback);

	// to avoid cube turning white on scaling down
	glEnable(GL_NORMALIZE);

	// Process input
	glutKeyboardFunc(window.processNormalKeys);
	glutPassiveMotionFunc(window.processMouseMove);

	// hide mouse cursor
	//glutSetCursor(GLUT_CURSOR_NONE);
	
	// Initialize networking for client
	Client::initializeClient();
	Client::sendStateUpdate(Client::getPlayerId(), 75, 0, 0);

	// player 1
	sg::Player p1 = sg::Player();
	p1.moveTo(glm::vec3(0,0,100.0f));
	p1.lookIn(glm::vec3(0,0,-1.0f));

	// 2nd player
	sg::Player p2 = sg::Player();
	//p2.moveTo(glm::vec3(0.0f,0.0,0.0f));
	p2.moveTo(glm::vec3(75.0f,0.0,0.0f));
	p2.lookIn(glm::vec3(0,0,-1.0f));

	// set root node
	client.root = &p1;
	client.root->addChild(&p2);

	// ground nodes
	sg::MatrixTransform ground = sg::MatrixTransform();
	//client.root->addChild(&ground);
	sg::Cube groundShape = sg::Cube();
	ground.addChild(&groundShape);
	ground.setMatrix(glm::translate(glm::vec3(0,-10,0)) * glm::scale(glm::vec3(100,0.1,100)));
	groundShape.color = glm::vec3(0,1,0);

	testAddCube(7, 90, 30, 0);

	sg::City city = sg::City();
	city.loadData("city.obj");
	client.root->addChild(&city);

	cout << "player center: " << glm::to_string(client.root->getCamera()->m_cameraCenter) << endl;

	glutMainLoop();

	return 0;
}

