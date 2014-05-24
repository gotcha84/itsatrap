#include "Level.h"

Level::Level() {
	root = new sg::MatrixTransform();
	root->setName("LEVEL ROOT");
	
	ground = new sg::MatrixTransform();
	ground->setName("LEVEL GROUND");
	root->addChild(ground);

	this->initLevel();
}

Level::~Level() {
	this->destroyLevel();

	delete ground;
	ground = nullptr;

	delete root;
	root = nullptr;
}

void Level::initLevel() {
	// add basic ground
	sg::MatrixTransform *groundXForm = new sg::MatrixTransform();
	groundXForm->setMatrix(glm::scale(glm::vec3(100,0.01f,100)));
	ground->addChild(groundXForm);

	sg::Cube *groundCube = new sg::Cube();
	groundCube->setName("ground cube");
	groundCube->setColor(glm::vec4(0,1,0,1));
	groundXForm->addChild(groundCube);

	// add obstacles + buildings to level
	sg::MatrixTransform *resourceTowerXForm = new sg::MatrixTransform();
	resourceTowerXForm->setMatrix(glm::translate(glm::vec3(-25,0,-25)));
	root->addChild(resourceTowerXForm);

	sg::ObjNode *resourceTower = new sg::ObjNode("../Models/Blocks/ResourceTower.obj", "../Models/Blocks/");
	resourceTower->setName("resource tower");
	resourceTower->getModel()->setColor(glm::vec4(1,0,0,1));
	resourceTowerXForm->addChild(resourceTower);

	sg::MatrixTransform *crossBarXForm = new sg::MatrixTransform();
	crossBarXForm->setMatrix(glm::translate(glm::vec3(25,0,-25)));
	root->addChild(crossBarXForm);

	sg::ObjNode *crossBar= new sg::ObjNode("../Models/Blocks/CrossBar.obj", "../Models/Blocks/");
	crossBar->setName("crossbar");
	crossBar->getModel()->setColor(glm::vec4(0,0,1,1));
	crossBarXForm->addChild(crossBar);
}

void Level::destroyLevel() {

}

sg::MatrixTransform* Level::getRoot() {
	return root;
}

void Level::setRoot(sg::MatrixTransform *newRoot) {
	root = newRoot;
}