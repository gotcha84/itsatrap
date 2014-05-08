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
#include "ConfigSettings.h"

// networking
#include "Client.h"
#include "enrico.h"

ClientInstance *client;
Window *window;

int main(int argc, char *argv[]) {

	Sleep(1000);

	ConfigSettings::getConfig()->loadSettingsFile();
	int testVal = 0;
	ConfigSettings::getConfig()->getValue("ScreenWidth", testVal);
	cout << "Test config file: ScreenWidth:" << testVal << endl;

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
	//glutFullScreen();

	glEnable(GL_DEPTH_TEST);                    // enable depth buffering
	glClear(GL_DEPTH_BUFFER_BIT);               // clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);           // set clear color to black
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
	glDisable(GL_CULL_FACE);					// disable backface culling to render both sides of polygons
	glShadeModel(GL_SMOOTH);                    // set shading to smooth
	
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

	// player 1
	//sg::Player *p1 = client->root;
	//client->root->setColor(glm::vec3(1,0,0));
	//client->root->moveTo(glm::vec3(80.0f, 0.0f, 100.0f));
	//client->root->lookIn(glm::vec3(0.0f, 0.0f, -1.0f));

	//// player 2
	//sg::Player p2 = sg::Player();
	//p2.setPlayerID(2);
	//p2.setColor(glm::vec3(0,1,0));
	//p2.moveTo(glm::vec3(90.0f, 4.0f, 0.0f));
	//p2.lookIn(glm::vec3(0.0f, 0.0f, 1.0f));

	//// player 3
	//sg::Player p3 = sg::Player();
	//p3.setPlayerID(3);
	//p3.setColor(glm::vec3(0,0,1));
	//p3.moveTo(glm::vec3(100.0f, 4.0f, 50.0f));
	//p3.lookIn(glm::vec3(0.0f, 0.0f, 1.0f));

	// add players	
	//client->addPlayer(&p2);
	//client->addPlayer(&p3);

	// ground nodes
	sg::MatrixTransform ground = sg::MatrixTransform();
	ground.setName("ground");
	//client->root->addChild(&ground);
	sg::Cube groundShape = sg::Cube();
	groundShape.setName("ground");
	ground.addChild(&groundShape);
	ground.setMatrix(glm::translate(glm::vec3(0,-10,0)) * glm::scale(glm::vec3(100,0.1,100)));
	groundShape.m_color = glm::vec4(0,1,0,1);

	// cube nodes
	sg::MatrixTransform obj1 = sg::MatrixTransform();
	obj1.setName("cube");
	//client->root->addChild(&obj1);
	sg::Cube obj1Shape = sg::Cube();
	obj1Shape.setName("cube");
	obj1.addChild(&obj1Shape);
	obj1.setMatrix(glm::translate(glm::vec3(0,-5,0)) * glm::scale(glm::vec3(10,10,10)));

	sg::City city = sg::City();
	city.loadData("city.obj");
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

