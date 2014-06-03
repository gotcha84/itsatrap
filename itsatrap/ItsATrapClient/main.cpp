#define FULLSCREEN 0
#define FBOWIDTH  512       // width of fbo
#define FBOHEIGHT 512       // hight of fbo

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "FTGL\ftgl.h"

#include "ConfigSettings.h"

// graphics
#include "ClientInstance.h"
#include "Window.h"
#include "SceneGraph.h"
#include "Level.h"
#include "Shader.h"
// networking
#include "Client.h"
#include "enrico.h"

//sound
#include "Sound.h"

ClientInstance *client;
Window *window;
Sound *sound;
Sound *otherPlayerSound;
Shader *shader1 = new Shader();

void initLevel() {

}

void initFBO() {
	window->pass1 = shader1->lightShader("../Shaders/pass1.frag", "../Shaders/pass1.vert");
	window->pass2 = shader1->lightShader("../Shaders/pass2blur.frag", "../Shaders/pass2blur.vert");

	// pass2 シェーダの uniform 変数の場所を得る
	window->diffuse = glGetUniformLocation(window->pass2, "diffuse");
	window->specular = glGetUniformLocation(window->pass2, "specular");
	window->ambient = glGetUniformLocation(window->pass2, "ambient");


	// prepare texture for color buffer
	glGenTextures(1, &window->cb);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, window->cb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FBOWIDTH, FBOHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// prepare render bufer for depth buffer
	glGenRenderbuffersEXT(1, &window->rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, window->rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create FBO
	glGenFramebuffersEXT(1, &window->fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, window->fb);
	// merge color buffer as texture into FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, window->cb, 0);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, window->sb, 0);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, window->ab, 0);
	// merge frame buffer as depth buffer into FBO
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, window->rb);
	// un-merge FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// 背景色
	//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	// 光源
	//glEnable(GL_LIGHT0);
}

void sendAABBInfo()
{
	// Player AABB
	MyPlayer *p = new MyPlayer();
	AABB *playerAABB = new AABB(p->getAABB());
	Client::sendAABBInfo(TYPE_PLAYER, playerAABB);
	delete p;
	delete playerAABB;

	string dir = TRAP_DIR;

	// Freeze trap
	sg::Trap *t = new sg::Trap(0, glm::vec3(0,0,0), 0, dir + FREEZE_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_FREEZE_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;

	// Trampoline trap
	t = new sg::Trap(0, glm::vec3(0, 0, 0), 0, dir + TRAMPOLINE_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_TRAMPOLINE_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;

	// Slow trap
	t = new sg::Trap(0, glm::vec3(0, 0, 0), 0, dir + SLOW_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_SLOW_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;

	// Push trap
	t = new sg::Trap(0, glm::vec3(0, 0, 0), 0, dir + PUSH_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_PUSH_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;

	// Lightning trap
	t = new sg::Trap(0, glm::vec3(0, 0, 0), 0, dir + DEATH_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_LIGHTNING_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;

	// Portal trap
	t = new sg::Trap(0, glm::vec3(0, 0, 0), 0, dir + PORTAL_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_PORTAL_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;

	// Flash trap
	t = new sg::Trap(0, glm::vec3(0, 0, 0), 0, dir + FLASH_TRAP_OBJ);
	Client::sendAABBInfo(TYPE_FLASH_TRAP, t->getTrapObjectForNetworking().aabb);
	delete t;
}

int main(int argc, char *argv[]) {

	Sleep(1000);

	ConfigSettings::getConfig()->loadSettingsFile();

	// opengl lighting
	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};  // lightsource position
	//GLfloat position[]  = {0.0, 1000.0, 0.0, 0.0};  // lightsource position
	//GLfloat ambientColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	// Initialize networking for client
	Client::initializeClient();
	client = new ClientInstance(Client::getPlayerId());
	window = new Window();
	glm::vec3 starting = client->root->getPosition();
	starting = starting + glm::vec3(200, 200, -300);
	client->root->moveTo(starting);
	Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	sendAABBInfo();

	glutInit(&argc, argv);                      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(window->m_width, window->m_height);      // set initial window size
	glutCreateWindow("It's a Trap!");           // open window and set window title
	if (FULLSCREEN)	 {
		glutFullScreen();
	}

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}

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
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	// Generate light source:
	glDisable(GL_LIGHTING);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
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


	//sg::City city = sg::City();
	//city.loadData("../Models/city.obj");
	//client->root->addChild(&city);

	client->root->addChild(client->level.getRoot());
//<HEAD
	for (int i = 0; i < client->level.levelNodes.size(); ++i) {
		Client::sendStaticObject(client->level.levelNodes[i]->getBoundingBox());
//=======
//
//	Client::sendStaticObject(client->level.groundCube->getBoundingBox().minX,
//		client->level.groundCube->getBoundingBox().minY,
//		client->level.groundCube->getBoundingBox().minZ,
//		client->level.groundCube->getBoundingBox().maxX,
//		client->level.groundCube->getBoundingBox().maxY,
//		client->level.groundCube->getBoundingBox().maxZ);
//
//	for (int i = 0; i < client->level.buildings.size(); ++i) {
//		Client::sendStaticObject(client->level.buildings[i]->getBoundingBox().minX, client->level.buildings[i]->getBoundingBox().minY,
//			client->level.buildings[i]->getBoundingBox().minZ, client->level.buildings[i]->getBoundingBox().maxX,
//			client->level.buildings[i]->getBoundingBox().maxY, client->level.buildings[i]->getBoundingBox().maxZ);
//> origin/enrico-temp
	}

	//for (int i = 0; i < client->level.buildings.size(); ++i) {
	//	Client::sendStaticObject(client->level.buildings[i]->getBoundingBox().minX, client->level.buildings[i]->getBoundingBox().minY,
	//		client->level.buildings[i]->getBoundingBox().minZ, client->level.buildings[i]->getBoundingBox().maxX,
	//		client->level.buildings[i]->getBoundingBox().maxY, client->level.buildings[i]->getBoundingBox().maxZ);
	//}

	//for (int i = 0; i < client->level.ramps.size(); ++i) {
	//	//Client::sendStaticRampObject(client->level.ramps[i]->getBoundingBox(), client->level.rampSlopes[i]);
	//}

	//for (int i = 0; i < client->level.resources.size(); ++i) {
	//	Client::sendStaticResourceObject(client->level.resources[i]->getBoundingBox(), client->level.resources[i]->getResourceId());
	//}

	//for (int i = 0; i < client->level.walls.size(); ++i) {
	//	Client::sendStaticWallObject(client->level.walls[i]->getBoundingBox());
	//}

	//client->printPlayers();
	//client->printSceneGraph();
	// setup background music and other player's footstep
	otherPlayerSound = new Sound("footstep.wav");
	sound = new Sound();
	otherPlayerSound->playMusic(false, false, true);
	//sound->playMusic();
	otherPlayerSound->setCenterPosition();
	// hardcode the distance value for now, it will be the input from the server
	otherPlayerSound->changePosition(-1.0f);

	initFBO();

	glutMainLoop();

	delete client;
	client = nullptr;

	delete window;
	window = nullptr;

	return 0;
}
