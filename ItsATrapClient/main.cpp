#define FULLSCREEN 1
//#define FBOWIDTHT 512       // width of fbo
//#define FBOHEIGHT 512     // hight of fbo
#define FBOWIDTHT 1600       // width of fbo
#define FBOHEIGHT 1200     // hight of fbo

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
#include "Texture.h"
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

Texture *textures;
ClientInstance *client;
Window *window;
Sound *sound;
Sound *otherPlayerSound;
Shader *shader1;

void initLevel() {

}

void initFBO() {
	shader1 = new Shader();
	//window->pass1 = shader1->lightShader("../Shaders/pass1.frag", "../Shaders/pass1.vert");
	shader1->loadShader("../Shaders/pass2blur.vert", "../Shaders/pass2blur.frag");
	window->pass1 = shader1->getShader();
	shader1->loadShader("../Shaders/test3.vert", "../Shaders/test3.frag");
	window->pass2 = shader1->getShader();

	// pass2 シェーダの uniform 変数の場所を得る
	window->diffuse = glGetUniformLocation(window->pass2, "diffuse");
	window->specular = glGetUniformLocation(window->pass2, "specular");
	window->ambient = glGetUniformLocation(window->pass2, "ambient");


	// prepare texture for color buffer
	glGenTextures(1, &window->cb);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, window->cb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FBOWIDTHT, FBOHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &window->sb);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, window->cb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FBOWIDTHT, FBOHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// prepare render bufer for depth buffer
	glGenRenderbuffersEXT(1, &window->rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, window->rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTHT, FBOHEIGHT);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create FBO
	glGenFramebuffersEXT(1, &window->fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, window->fb);
	// merge color buffer as texture into FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, window->cb, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, window->sb, 0);
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

	ConfigSettings::getConfig()->loadSettingsFile();

	// opengl lighting
	
	GLfloat shininess[] = {100.0};
	GLfloat spotDirection[] = { 0.0f, -1.0f, 0.0f };

	GLfloat specularLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat position0[] = { 0.0f, -1200.0f, -300.0f, 0.0f };  // lightsource position
	GLfloat ambientLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat diffuseLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	
	GLfloat specularLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position1[]  = {-20.0f, -100.0f, -350.0f, 1.0f};  // lightsource position
	GLfloat ambientLight1[] = { 0.4f, 0.8f, 0.8f, 1.0f };
	GLfloat diffuseLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat specularLight2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat position2[] = { 0.0f, -1200.0f, 300.0f, 0.0f };  // lightsource position
	GLfloat ambientLight2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat diffuseLight2[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	GLfloat specularLight3[] = { 1.0f, 0.2f, 0.2f, 1.0f };
	GLfloat position3[] = { -350.0f, 1000.0f, -350.0f, 1.0f };  // lightsource position
	GLfloat ambientLight3[] = { 0.6f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight3[] = { 1.0f, 0.2f, 0.2f, 1.0f };

	GLfloat specularLight4[] = { 0.2f, 1.0f, 0.2f, 1.0f };
	GLfloat position4[] = { 350.0f, 1000.0f, -350.0f, 1.0f };  // lightsource position
	GLfloat ambientLight4[] = { 0.2f, 0.6f, 0.2f, 1.0f };
	GLfloat diffuseLight4[] = { 0.2f, 1.0f, 0.2f, 1.0f };

	GLfloat specularLight5[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat position5[] = { -300.0f, -1200.0f, 0.0f, 0.0f };  // lightsource position
	GLfloat ambientLight5[] = { 0.4f, 0.4f, 0.8f, 1.0f };
	GLfloat diffuseLight5[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	GLfloat specularLight6[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat position6[] = { -300.0f, -1200.0f, 0.0f, 0.0f };  // lightsource position
	GLfloat ambientLight6[] = { 0.4f, 0.4f, 0.8f, 1.0f };
	GLfloat diffuseLight6[] = { 0.8f, 0.8f, 0.8f, 1.0f };


	// init glut
	glutInit(&argc, argv);                      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(window->m_width, window->m_height);      // set initial window size
	glutCreateWindow("It's a Trap!");           // open window and set window title
	if (FULLSCREEN)	 {
		glutFullScreen();
	}

	// Initialize networking for client
	Client::initializeClient();
	cout << "DONE CLIENT" << endl;
	textures = new Texture();
	cout << "DONE TEXTURE" << endl;
	client = new ClientInstance(Client::getPlayerId());
	cout << "DONE CLIENT INSTANCE" << endl;
	window = new Window();
	cout << "DONE WINDOW" << endl;
	glm::vec3 starting = client->root->getPosition();
	glm::vec3 shift;
	switch (client->root->getPlayerID()) {
	case 0: 
		shift = glm::vec3(-200, 500, 150);
		break;
	case 1:
		shift = glm::vec3(200, 500, 150);
		break;
	case 2:
		shift = glm::vec3(200, 500, -150);
		break;
	case 3:
		shift = glm::vec3(-200, 500, -150);
		break;
	default:
		shift = glm::vec3(200, 500, -300);
		break;
	}
	starting = starting + shift;
	client->root->moveTo(starting);
	Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
	sendAABBInfo();

	client->gameState.setWelcome();

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);                    // enable depth buffering
	glClearDepth(1.0f);							// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);						// The Type Of Depth Testing To Do

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective 
	//glClear(GL_DEPTH_BUFFER_BIT);               // clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.5f);           // set clear color to black
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
	glEnable(GL_CULL_FACE);					// disable backface culling to render both sides of polygons
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	glShadeModel(GL_SMOOTH);                    // set shading to smooth

	// backface culling to render front sides of polygons
	glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	
	// Generate material properties:
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	// Generate light source:
	glDisable(GL_LIGHTING);
	
	// Install callback functions:
	glutDisplayFunc(window->displayCallback);
	glutReshapeFunc(window->reshapeCallback);
	glutIdleFunc(window->idleCallback);

	// to avoid cube turning white on scaling down
	glEnable(GL_NORMALIZE);

	// enable alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//gluPerspective(90.0f, ((float)Window::m_width)/((float)Window::m_height), 1.0f, 1000.0f);

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

	// Sending AABBs to server
	client->root->addChild(client->level.getRoot());

	if (client->root->getPlayerID() == 0) {
		for (int i = 0; i < client->level.levelNodes.size(); ++i) {
			//if (i % 100 == 0) { Sleep(250); }
			Client::sendStaticObject(client->level.levelNodes[i].first->getBoundingBox(), 
				client->level.levelNodes[i].second,
				client->level.levelNodes[i].first->getModel()->getColor());
			if (i == 271) {
				cout << "I: " << i << ", " << client->level.levelNodes[i].first->getName();
				
				//client->level.levelNodes[i].first->getBoundingBox().print();
			}
			Sleep(10);
		}

		for (int i = 0; i < client->level.resources.size(); ++i) {
			Client::sendStaticObject(client->level.resources[i]->getBoundingBox(), 
				true,
				client->level.resources[i]->getModel()->getColor());
			Client::sendStaticResourceObject(client->level.resources[i]->getBoundingBox(),
				client->level.resources[i]->getResourceId());
		}
	}
	
	client->root->m_elapsedGameTime = 0;
	//client->root->m_gameOver = false;

	/*sg::ObjNode node = sg::ObjNode();
	node.m_model->loadModel("../Models/Polynoid_Updated/Polynoid.obj", "../Models/Polynoid_Updated/");
	node.m_model->setTexture(textures->m_texID[Textures::Polynoid]);
	node.m_model->setColor(glm::vec4(1, 1, 1, 1));
	sg::MatrixTransform nodeXForm = sg::MatrixTransform();
	nodeXForm.addChild(&node);
	client->root->addChild(&nodeXForm);*/

	// skybox
	sg::MatrixTransform sbXForm = sg::MatrixTransform();
	glm::mat4 model = glm::mat4();
	model = glm::rotate(model, 90.0f, glm::vec3(0, 0, 1));
	model = glm::rotate(model, 45.0f, glm::vec3(1, 0, 0));
	model = glm::rotate(model, 30.0f, glm::vec3(0, 1, 0));
	//model = glm::rotate(model, 25.0f, glm::vec3(0, 0, 1));
	sbXForm.setMatrix(model);
	client->root->addChild(&sbXForm);

	sg::Skybox skybox = sg::Skybox();
	skybox.loadModel("../Models/Skybox/skybox.obj", "../Models/Skybox/");
	skybox.getModel()->setTexture(textures->m_texID[Textures::Skybox]);
	skybox.getModel()->setColor(glm::vec4(1, 1, 1, 1));
	skybox.getModel()->disableDrawBB();
	sbXForm.addChild(&skybox);

	sg::ParticleSystemNode ps = sg::ParticleSystemNode(500);
	client->root->addChild(&ps);

	//client->printPlayers();
	//client->printSceneGraph();
	// setup background music and other player's footstep
	otherPlayerSound = new Sound("../Sound/footstep.wav");
	sound = new Sound();
	otherPlayerSound->playMusic(false, false, true);
	sound->playMusic();
	otherPlayerSound->setCenterPosition();
	// hardcode the distance value for now, it will be the input from the server
	otherPlayerSound->changePosition(-1.0f);

	initFBO();

	glutMainLoop();

	delete client;
	client = nullptr;

	delete window;
	window = nullptr;

	delete shader1;
	shader1 = nullptr;

	return 0;
}
