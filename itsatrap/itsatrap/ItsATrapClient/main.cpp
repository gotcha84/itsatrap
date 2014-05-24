#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "FTGL\ftgl.h"

// graphics
#include "ClientInstance.h"
#include "Window.h"
#include "SceneGraph.h"
#include "ConfigSettings.h"

// networking
#include "Client.h"
#include "enrico.h"

ClientInstance *client;
Window *window;

int main(int argc, char *argv[]) {

	Sleep(1000);

	ConfigSettings::getConfig()->loadSettingsFile();

	// opengl lighting
	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};  // lightsource position

	// Initialize networking for client
	Client::initializeClient();
	client = new ClientInstance(Client::getPlayerId());
	window = new Window();
	glm::vec3 starting = client->root->getPosition();
	client->root->moveTo(glm::vec3(starting.x, starting.y + 2, starting.z));
	Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());

	glutInit(&argc, argv);                      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(window->m_width, window->m_height);      // set initial window size
	glutCreateWindow("It's a Trap!");           // open window and set window title
	glutFullScreen();

	glEnable(GL_DEPTH_TEST);                    // enable depth buffering
	glClear(GL_DEPTH_BUFFER_BIT);               // clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);           // set clear color to black
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
	glEnable(GL_CULL_FACE);					// disable backface culling to render both sides of polygons
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	glShadeModel(GL_SMOOTH);                    // set shading to smooth

	// backface culling to render front sides of polygons
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	// Generate material properties:
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	// Generate light source:
	glDisable(GL_LIGHTING);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	
	// Install callback functions:
	glutDisplayFunc(window->displayCallback);
	glutReshapeFunc(window->reshapeCallback);
	glutIdleFunc(window->idleCallback);

	// to avoid cube turning white on scaling down
	glEnable(GL_NORMALIZE);

	// enable alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// keyboard input
	glutKeyboardFunc(window->keyDown);
	glutKeyboardUpFunc(window->keyUp);
	glutSpecialFunc(window->specialKeyDown); // Tell GLUT to use the method "keySpecial" for special key presses  
	glutSpecialUpFunc(window->specialKeyUp);

	// mouse input
	glutMouseFunc(window->processMouseKeys);
	glutPassiveMotionFunc(window->processMouseMove);

	// hide mouse cursor
	//glutSetCursor(GLUT_CURSOR_NONE);

	sg::City city = sg::City();
	city.loadData("../Models/city.obj");
	//city.loadData("Can.obj");
	client->root->addChild(&city);

	client->printPlayers();
	// client->printSceneGraph();

	glutMainLoop();

	delete client;
	client = nullptr;

	delete window;
	window = nullptr;

	return 0;
}

