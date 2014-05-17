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
	groundXForm->setMatrix(glm::scale(glm::vec3(40 * UNIT_SIZE, 0.01f, 40 * UNIT_SIZE)));
	ground->addChild(groundXForm);

	sg::Cube *groundCube = new sg::Cube();
	groundCube->setName("ground cube");
	groundCube->setColor(glm::vec4(0,1,0,1));
	groundXForm->addChild(groundCube);

	// Add obstacles + buildings to level
	// Building: Main Arena w/ Resource Tower
	sg::MatrixTransform *blockResource1_XForm = new sg::MatrixTransform();
	blockResource1_XForm->setMatrix(glm::translate(glm::vec3(0, 12*UNIT_SIZE/2, 0)) * glm::scale(glm::vec3(16*UNIT_SIZE, 12*UNIT_SIZE, 8*UNIT_SIZE)));
	root->addChild(blockResource1_XForm);

	sg::Cube *buildingResource1 = new sg::Cube();
	buildingResource1->setName("Building: Resource 1");
	buildingResource1->setColor(glm::vec4(1, 0, 0, 1));
	blockResource1_XForm->addChild(buildingResource1);

	// Resource Tower #1
	sg::MatrixTransform *resourceTower1_XForm = new sg::MatrixTransform();
	resourceTower1_XForm->setMatrix(glm::translate(glm::vec3(0, 12 * UNIT_SIZE, 0)));
	root->addChild(resourceTower1_XForm);

	sg::ObjNode *resourceTower1 = new sg::ObjNode(RESOURCETOWER, BLOCKS);
	resourceTower1->setName("Resource Tower 1");
	resourceTower1->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	resourceTower1_XForm->addChild(resourceTower1);

	// Building 
	sg::MatrixTransform *building1_XForm = new sg::MatrixTransform();
	building1_XForm->setMatrix(glm::translate(glm::vec3(13 * UNIT_SIZE, 8 * UNIT_SIZE / 2, -1 * UNIT_SIZE)) * glm::scale(glm::vec3(6*UNIT_SIZE, 8*UNIT_SIZE, 6*UNIT_SIZE)));
	root->addChild(building1_XForm);

	sg::Cube *building1 = new sg::Cube();
	building1->setName("Building: 1");
	building1->setColor(glm::vec4(1, 0, 0, 1));
	building1_XForm->addChild(building1);
}

void Level::destroyLevel() {

}

sg::MatrixTransform* Level::getRoot() {
	return root;
}

void Level::setRoot(sg::MatrixTransform *newRoot) {
	root = newRoot;
}