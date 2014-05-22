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


	// Initialize height map
	world.initializeHeightMap();

	// add basic ground
	sg::MatrixTransform *groundXForm = new sg::MatrixTransform();
	groundXForm->setMatrix(glm::scale(glm::vec3(MAP_X_LENGTH * UNIT_SIZE, FLOOR_HEIGHT, MAP_Z_LENGTH * UNIT_SIZE)));

	ground->addChild(groundXForm);

	sg::Cube *groundCube = new sg::Cube();
	groundCube->setName("ground cube");
	groundCube->setColor(glm::vec4(0,1,0,1));
	groundXForm->addChild(groundCube);

	// Add obstacles + buildings to level
	// Resource Tower #1
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(0, UNIT_12, 0)));
	root->addChild(xForms.back());

	resources.push_back(new sg::ObjNode(RESOURCETOWER, BLOCKS));
	resources.back()->setName("Resource Tower 0: (0, 0, 0)");
	resources.back()->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	xForms.back()->addChild(resources.back());

	// Resource Tower #2
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-15 * UNIT_SIZE, UNIT_8, 7 * UNIT_SIZE)));
	root->addChild(xForms.back());

	resources.push_back(new sg::ObjNode(RESOURCETOWER, BLOCKS));
	resources.back()->setName("Resource Tower 1: (-15, 8, 7)");
	resources.back()->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	xForms.back()->addChild(resources.back());

	// Resource Tower #3
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(15 * UNIT_SIZE, UNIT_8, -7 * UNIT_SIZE)));
	root->addChild(xForms.back());

	resources.push_back(new sg::ObjNode(RESOURCETOWER, BLOCKS));
	resources.back()->setName("Resource Tower 2: (-15, 8, 7)");
	resources.back()->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	xForms.back()->addChild(resources.back());

	// Building 0: (0, 0, 0)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(0, UNIT_12/2, 0)) * glm::scale(glm::vec3(UNIT_16, UNIT_12, UNIT_8)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 0: (0, 0, 0)");
	buildings.back()->setColor(glm::vec4(1, 0, 0, 1));
	xForms.back()->addChild(buildings.back());
	
	// Building 1: (-15, 8, 7)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-15 * UNIT_SIZE, UNIT_8 / 2, 7 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 1: (-15, 8, 7)");
	buildings.back()->setColor(glm::vec4(1, 0, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 2: (15, 8, -7)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(15 * UNIT_SIZE, UNIT_8 / 2, -7 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 2: (15, 8, -7)");
	buildings.back()->setColor(glm::vec4(1, 0, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 3: (-13, 8, -1)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-1 * UNIT_13, UNIT_8 / 2, -1 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 3: (-13, 8, -1)");
	buildings.back()->setColor(glm::vec4(0.5, 0.1, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 4: (-12, 4, -8) 
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-1 * UNIT_12, UNIT_4 / 2, -8 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_4, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 4: (-12, 4, -8)");
	buildings.back()->setColor(glm::vec4(0.5, 0.2, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 5: (-18, 8, -11)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-18 * UNIT_SIZE, UNIT_8 / 2, -11 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 5: (-18, 8, -11)");
	buildings.back()->setColor(glm::vec4(0.5, 0.3, 0, 1));
	xForms.back()->addChild(buildings.back());;

	// Building 6: (-17, 20, -17)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-17 * UNIT_SIZE, UNIT_20 / 2, -17 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_20, UNIT_5)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 6: (-17, 20, -17)");
	buildings.back()->setColor(glm::vec4(0.5, 0.4, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 7: (-10, 16, -19)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-10 * UNIT_SIZE, UNIT_16 / 2, -19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_16, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 7: (-10, 16, -19)");
	buildings.back()->setColor(glm::vec4(0.5, 0.5, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 8: (-5, 12, -18)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-5 * UNIT_SIZE, UNIT_12 / 2, -18 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_12, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 8: (-5, 12, -18)");
	buildings.back()->setColor(glm::vec4(0.6, 0.5, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 9: (-5, 8, -13)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-5 * UNIT_SIZE, UNIT_8 / 2, -13 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_8, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 9: (-5, 8, -13)");
	buildings.back()->setColor(glm::vec4(0.7, 0.5, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 10: (-4, 4, -9)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-4 * UNIT_SIZE, UNIT_4 / 2, -9 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_4, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 10: (-4, 4, -9)");
	buildings.back()->setColor(glm::vec4(0.8, 0.5, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 11: (-19, 8, -3)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, UNIT_8 / 2, -3 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_8, UNIT_12)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 11: (-19, 8, -3)");
	buildings.back()->setColor(glm::vec4(0.9, 0.5, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 12: (6, 6, -15)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(6 * UNIT_SIZE, UNIT_6 / 2, -15 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_6, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 12: (6, 6, -15)");
	buildings.back()->setColor(glm::vec4(1, 0.5, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 13: (9, 8, -19)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(9 * UNIT_SIZE, UNIT_8 / 2, -19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 13: (9, 8, -19)");
	buildings.back()->setColor(glm::vec4(1, 0.6, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 14: (13, 12, -14)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(13 * UNIT_SIZE, UNIT_12 / 2, -14 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_12, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 14: (13, 12, -14)");
	buildings.back()->setColor(glm::vec4(1, 0.7, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 15: (16, 16, -19)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(16 * UNIT_SIZE, UNIT_16 / 2, -19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_16, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 15: (16, 16, -19)");
	buildings.back()->setColor(glm::vec4(1, 0.8, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 16: (19, 16, 2)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(19 * UNIT_SIZE, UNIT_16 / 2, 0)) * glm::scale(glm::vec3(UNIT_2, UNIT_16, UNIT_40)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 16: (19, 16, 2)");
	buildings.back()->setColor(glm::vec4(1, 0.9, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 17: (17, 8, 1)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, UNIT_8 / 2, UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 17: (17, 8, 1)");
	buildings.back()->setColor(glm::vec4(1, 1, 0, 1));
	xForms.back()->addChild(buildings.back());

	// Building 18: (12, 16, 4)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(12 * UNIT_SIZE, UNIT_16 / 2, 4 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_16, UNIT_8)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 18: (12, 16, 4)");
	buildings.back()->setColor(glm::vec4(0.5, 0, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 19: (12, 4, 13)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(12 * UNIT_SIZE, UNIT_4 / 2, 13 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_4, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 19: (12, 4, 13)");
	buildings.back()->setColor(glm::vec4(0.6, 0, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 20: (17, 24, 16)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, UNIT_24 / 2, 16 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_24, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 20: (17, 24, 16)");
	buildings.back()->setColor(glm::vec4(0.7, 0, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 21: (14, 16, 19)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(14 * UNIT_SIZE, UNIT_16 / 2, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_8, UNIT_16, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 21: (14, 16, 19)");
	buildings.back()->setColor(glm::vec4(0.8, 0, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 22: (6, 8, 9)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(6 * UNIT_SIZE, UNIT_8 / 2, 9 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 22: (6, 8, 9)");
	buildings.back()->setColor(glm::vec4(0.9, 0, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 23: (-6, 6, 17)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-6 * UNIT_SIZE, UNIT_6 / 2, 17 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_6, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 23: (-6, 6, 17)");
	buildings.back()->setColor(glm::vec4(1, 0, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 24: (-8, 8, 13)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-8 * UNIT_SIZE, UNIT_8 / 2, 13 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 24: (-8, 8, 13)");
	buildings.back()->setColor(glm::vec4(1, 0, 0.6, 1));
	xForms.back()->addChild(buildings.back());

	// Building 25: (-13, 16, 19)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-13 * UNIT_SIZE, UNIT_16 / 2, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_10, UNIT_16, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 25: (-13, 16, 19)");
	buildings.back()->setColor(glm::vec4(1, 0, 0.7, 1));
	xForms.back()->addChild(buildings.back());

	// Building 26: (-12, 8, 15)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-12 * UNIT_SIZE, UNIT_8 / 2, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_2)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 26: (-12, 8, 15)");
	buildings.back()->setColor(glm::vec4(1, 0, 0.8, 1));
	xForms.back()->addChild(buildings.back());

	// Building 27: (17, 12, 15)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-17 * UNIT_SIZE, UNIT_12 / 2, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_12, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 27: (17, 12, 15)");
	buildings.back()->setColor(glm::vec4(1, 0, 0.9, 1));
	xForms.back()->addChild(buildings.back());

	// Building 28: (19, 16, 12)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, UNIT_16 / 2, 12 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_16, UNIT_16)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 28: (19, 16, 12)");
	buildings.back()->setColor(glm::vec4(1, 0, 1, 1));
	xForms.back()->addChild(buildings.back());

	// Building 29: (-3, 24, 8)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-3 * UNIT_SIZE, UNIT_24 / 2, 8 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_24, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 29: (-3, 24, 8)");
	buildings.back()->setColor(glm::vec4(0.5, 0.5, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 30: (3, 24, -8)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(3 * UNIT_SIZE, UNIT_24 / 2, -8 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_24, UNIT_4)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 30: (3, 24, -8)");
	buildings.back()->setColor(glm::vec4(0.6, 0.5, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Note: Ramp needs to be reduced by 45% in the y-axis to match unit_size. 55% for other axis
	// Ramp 0: (7, 0, -7)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(7 * UNIT_SIZE, 0, -7 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 24, .55 * 6)));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 0: (7, 0, -7)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 1: (-7, 0, 7)	
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-7 * UNIT_SIZE, 0, 7 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 24, .55 * 6)) * Utilities::rotateY(180));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 1: (-7, 0, 7)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());;

	// Ramp 2: (-12, 0, -14)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-12 * UNIT_SIZE, 0, -14 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 4, 0.45 * 8, .55 * 4)));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 2: (-12, 0, -14)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 3: (-19, 8, 0)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, 8 * UNIT_SIZE, 0)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 16, .55 * 6)));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 3: (-19, 8, 0)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 4: (5, 0, 15)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(5 * UNIT_SIZE, 0, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 6, 0.45 * 8, .55 * 2)) * Utilities::rotateY(90));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 4: (5, 0, 15)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 5: (5, 0, 19)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(5 * UNIT_SIZE, 0, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 10, 0.45 * 32, .55 * 2)) * Utilities::rotateY(90));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 5: (5, 0, 19)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 6: (17, 0, 8)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, 0, 7.95 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 16, .55 * 8)) * Utilities::rotateY(180));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 6: (17, 0, 8)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Update Bounding Boxes and Height Map
	// Building Height Map
	for (int i = 0; i < buildings.size(); ++i) {
		buildings[i]->calculateBoundingBox();
		world.updateHeightMap(buildings[i]->getBoundingBox());
	}

	// Ramp Height Map
	// TODO (ktngo): Figure out how to access the true max and mins of the ramp
	for (int i = 0; i < ramps.size(); ++i) {
		ramps[i]->calculateBoundingBox();
		world.updateHeightMap(ramps[i]->getBoundingBox());
	}

	//world.printHeightMapToFile("heightMap.txt");

}

void Level::destroyLevel() {

}

sg::MatrixTransform* Level::getRoot() {
	return root;
}

void Level::setRoot(sg::MatrixTransform *newRoot) {
	root = newRoot;
}