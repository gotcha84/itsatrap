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
	// Building: Main Arena w/ Resource Tower
	sg::MatrixTransform *blockResource1_XForm = new sg::MatrixTransform();
	blockResource1_XForm->setMatrix(glm::translate(glm::vec3(0, UNIT_12/2, 0)) * glm::scale(glm::vec3(UNIT_16, UNIT_12, UNIT_8)));
	root->addChild(blockResource1_XForm);

	sg::Cube *buildingResource1 = new sg::Cube();
	buildingResource1->setName("Building: Resource 1");
	buildingResource1->setColor(glm::vec4(1, 0, 0, 1));
	blockResource1_XForm->addChild(buildingResource1);
	
	world.updateHeightMap(buildingResource1->getBoundingBox());

	//buildingResource1->getBoundingBox().print();

	buildingResource1->calculateBoundingBox();
	buildingResource1->getBoundingBox().print();

	// Building: Arena w/ Resource Tower 2 (-15, 8, 7)
	sg::MatrixTransform *blockResource2_XForm = new sg::MatrixTransform();
	blockResource2_XForm->setMatrix(glm::translate(glm::vec3(-15 * UNIT_SIZE, UNIT_8 / 2, 7 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(blockResource2_XForm);

	sg::Cube *buildingResource2 = new sg::Cube();
	buildingResource2->setName("Building: Resource 2");
	buildingResource2->setColor(glm::vec4(1, 0, 0, 1));
	blockResource2_XForm->addChild(buildingResource2);

	//cout << "BOUNDING BOX: " << endl;

	// Building: Arena w/ Resource Tower 3 (15, 8, -7)
	sg::MatrixTransform *blockResource3_XForm = new sg::MatrixTransform();
	blockResource3_XForm->setMatrix(glm::translate(glm::vec3(15 * UNIT_SIZE, UNIT_8 / 2, -7 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(blockResource3_XForm);

	sg::Cube *buildingResource3 = new sg::Cube();
	buildingResource3->setName("Building: Resource 3");
	buildingResource3->setColor(glm::vec4(1, 0, 0, 1));
	blockResource3_XForm->addChild(buildingResource3);

	//// Resource Tower #1
	sg::MatrixTransform *resourceTower1_XForm = new sg::MatrixTransform();
	resourceTower1_XForm->setMatrix(glm::translate(glm::vec3(0, UNIT_12, 0)));
	root->addChild(resourceTower1_XForm);

	sg::ObjNode *resourceTower1 = new sg::ObjNode(RESOURCETOWER, BLOCKS);
	resourceTower1->setName("Resource Tower 1");
	resourceTower1->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	resourceTower1_XForm->addChild(resourceTower1);

	// Resource Tower #2
	sg::MatrixTransform *resourceTower2_XForm = new sg::MatrixTransform();
	resourceTower2_XForm->setMatrix(glm::translate(glm::vec3(-15 * UNIT_SIZE, UNIT_8, 7 * UNIT_SIZE)));
	root->addChild(resourceTower2_XForm);

	sg::ObjNode *resourceTower2 = new sg::ObjNode(RESOURCETOWER, BLOCKS);
	resourceTower2->setName("Resource Tower 2");
	resourceTower2->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	resourceTower2_XForm->addChild(resourceTower2);

	// Resource Tower #3
	sg::MatrixTransform *resourceTower3_XForm = new sg::MatrixTransform();
	resourceTower3_XForm->setMatrix(glm::translate(glm::vec3(15 * UNIT_SIZE, UNIT_8, -7 * UNIT_SIZE)));
	root->addChild(resourceTower3_XForm);

	sg::ObjNode *resourceTower3 = new sg::ObjNode(RESOURCETOWER, BLOCKS);
	resourceTower3->setName("Resource Tower 3");
	resourceTower3->getModel()->setColor(glm::vec4(0, 0, 1, 1));
	resourceTower3_XForm->addChild(resourceTower3);

	// Building 1: (-13, 8, -1)
	sg::MatrixTransform *building1_XForm = new sg::MatrixTransform();
	building1_XForm->setMatrix(glm::translate(glm::vec3(-1 * UNIT_13, UNIT_8 / 2, -1 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(building1_XForm);

	sg::Cube *building1 = new sg::Cube();
	building1->setName("Building: 1");
	building1->setColor(glm::vec4(1, 0, 0, 1));
	building1_XForm->addChild(building1);

	// Building 2: (-12, 4, -8) 
	sg::MatrixTransform *building2_XForm = new sg::MatrixTransform();
	building2_XForm->setMatrix(glm::translate(glm::vec3(-1 * UNIT_12, UNIT_4 / 2, -8 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_4, UNIT_4)));
	root->addChild(building2_XForm);

	sg::Cube *building2 = new sg::Cube();
	building2->setName("Building: 2");
	building2->setColor(glm::vec4(1, 0, 0, 1));
	building2_XForm->addChild(building2);

	// Building 3: (-18, 8, -11)
	sg::MatrixTransform *building3_XForm = new sg::MatrixTransform();
	building3_XForm->setMatrix(glm::translate(glm::vec3(-18 * UNIT_SIZE, UNIT_8 / 2, -11 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_4)));
	root->addChild(building3_XForm);

	sg::Cube *building3 = new sg::Cube();
	building3->setName("Building: 3");
	building3->setColor(glm::vec4(1, 0, 0, 1));
	building3_XForm->addChild(building3);

	// Building 4: (-17, 20, -17)
	sg::MatrixTransform *building4_XForm = new sg::MatrixTransform();
	building4_XForm->setMatrix(glm::translate(glm::vec3(-17 * UNIT_SIZE, UNIT_20 / 2, -17 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_20, UNIT_5)));
	root->addChild(building4_XForm);

	sg::Cube *building4 = new sg::Cube();
	building4->setName("Building: 4");
	building4->setColor(glm::vec4(1, 0, 0, 1));
	building4_XForm->addChild(building4);

	// Building 5: (-10, 16, -19)
	sg::MatrixTransform *building5_XForm = new sg::MatrixTransform();
	building5_XForm->setMatrix(glm::translate(glm::vec3(-10 * UNIT_SIZE, UNIT_16 / 2, -19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_16, UNIT_2)));
	root->addChild(building5_XForm);

	sg::Cube *building5 = new sg::Cube();
	building5->setName("Building: 5");
	building5->setColor(glm::vec4(1, 0, 0, 1));
	building5_XForm->addChild(building5);

	// Building 6: (-5, 12, -18)
	sg::MatrixTransform *building6_XForm = new sg::MatrixTransform();
	building6_XForm->setMatrix(glm::translate(glm::vec3(-5 * UNIT_SIZE, UNIT_12 / 2, -18 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_12, UNIT_4)));
	root->addChild(building6_XForm);

	sg::Cube *building6 = new sg::Cube();
	building6->setName("Building: 6");
	building6->setColor(glm::vec4(1, 0, 0, 1));
	building6_XForm->addChild(building6);

	// Building 7: (-5, 8, -13)
	sg::MatrixTransform *building7_XForm = new sg::MatrixTransform();
	building7_XForm->setMatrix(glm::translate(glm::vec3(-5 * UNIT_SIZE, UNIT_8 / 2, -13 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_8, UNIT_2)));
	root->addChild(building7_XForm);

	sg::Cube *building7 = new sg::Cube();
	building7->setName("Building: 7");
	building7->setColor(glm::vec4(1, 0, 0, 1));
	building7_XForm->addChild(building7);

	// Building 8: (-4, 4, -9)
	sg::MatrixTransform *building8_XForm = new sg::MatrixTransform();
	building8_XForm->setMatrix(glm::translate(glm::vec3(-4 * UNIT_SIZE, UNIT_4 / 2, -9 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_4, UNIT_2)));
	root->addChild(building8_XForm);

	sg::Cube *building8 = new sg::Cube();
	building8->setName("Building: 8");
	building8->setColor(glm::vec4(1, 0, 0, 1));
	building8_XForm->addChild(building8);

	// Building 9: (-19, 8, -3)
	sg::MatrixTransform *building9_XForm = new sg::MatrixTransform();
	building9_XForm->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, UNIT_8 / 2, -3 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_8, UNIT_12)));
	root->addChild(building9_XForm);

	sg::Cube *building9 = new sg::Cube();
	building9->setName("Building: 9");
	building9->setColor(glm::vec4(1, 0, 0, 1));
	building9_XForm->addChild(building9);

	// Building 10: (6, 6, -15)
	sg::MatrixTransform *building10_XForm = new sg::MatrixTransform();
	building10_XForm->setMatrix(glm::translate(glm::vec3(6 * UNIT_SIZE, UNIT_6 / 2, -15 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_6, UNIT_2)));
	root->addChild(building10_XForm);

	sg::Cube *building10 = new sg::Cube();
	building10->setName("Building: 9");
	building10->setColor(glm::vec4(1, 0, 0, 1));
	building10_XForm->addChild(building10);

	// Building 11: (9, 8, -19)
	sg::MatrixTransform *building11_XForm = new sg::MatrixTransform();
	building11_XForm->setMatrix(glm::translate(glm::vec3(9 * UNIT_SIZE, UNIT_8 / 2, -19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_2)));
	root->addChild(building11_XForm);

	sg::Cube *building11 = new sg::Cube();
	building11->setName("Building: 11");
	building11->setColor(glm::vec4(1, 0, 0, 1));
	building11_XForm->addChild(building11);

	// Building 12: (13, 12, -14)
	sg::MatrixTransform *building12_XForm = new sg::MatrixTransform();
	building12_XForm->setMatrix(glm::translate(glm::vec3(13 * UNIT_SIZE, UNIT_12 / 2, -14 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_12, UNIT_4)));
	root->addChild(building12_XForm);

	sg::Cube *building12 = new sg::Cube();
	building12->setName("Building: 12");
	building12->setColor(glm::vec4(1, 0, 0, 1));
	building12_XForm->addChild(building12);

	// Building 13: (16, 16, -19)
	sg::MatrixTransform *building13_XForm = new sg::MatrixTransform();
	building13_XForm->setMatrix(glm::translate(glm::vec3(16 * UNIT_SIZE, UNIT_16 / 2, -19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_16, UNIT_2)));
	root->addChild(building13_XForm);

	sg::Cube *building13 = new sg::Cube();
	building13->setName("Building: 13");
	building13->setColor(glm::vec4(1, 0, 0, 1));
	building13_XForm->addChild(building13);

	// Building 14: (19, 16, 2)
	sg::MatrixTransform *building14_XForm = new sg::MatrixTransform();
	building14_XForm->setMatrix(glm::translate(glm::vec3(19 * UNIT_SIZE, UNIT_16 / 2, 0)) * glm::scale(glm::vec3(UNIT_2, UNIT_16, UNIT_40)));
	root->addChild(building14_XForm);

	sg::Cube *building14 = new sg::Cube();
	building14->setName("Building: 14");
	building14->setColor(glm::vec4(1, 0, 0, 1));
	building14_XForm->addChild(building14);

	// Building 15: (17, 8, 1)
	sg::MatrixTransform *building15_XForm = new sg::MatrixTransform();
	building15_XForm->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, UNIT_8 / 2, UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_8, UNIT_6)));
	root->addChild(building15_XForm);

	sg::Cube *building15 = new sg::Cube();
	building15->setName("Building: 15");
	building15->setColor(glm::vec4(1, 0, 0, 1));
	building15_XForm->addChild(building15);

	// Building 16: (12, 16, 4)
	sg::MatrixTransform *building16_XForm = new sg::MatrixTransform();
	building16_XForm->setMatrix(glm::translate(glm::vec3(12 * UNIT_SIZE, UNIT_16 / 2, 4 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_16, UNIT_8)));
	root->addChild(building16_XForm);

	sg::Cube *building16 = new sg::Cube();
	building16->setName("Building: 16");
	building16->setColor(glm::vec4(1, 0, 0, 1));
	building16_XForm->addChild(building16);

	// Building 17: (12, 4, 13)
	sg::MatrixTransform *building17_XForm = new sg::MatrixTransform();
	building17_XForm->setMatrix(glm::translate(glm::vec3(12 * UNIT_SIZE, UNIT_4 / 2, 13 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_4, UNIT_6)));
	root->addChild(building17_XForm);

	sg::Cube *building17 = new sg::Cube();
	building17->setName("Building: 17");
	building17->setColor(glm::vec4(1, 0, 0, 1));
	building17_XForm->addChild(building17);

	// Building 18: (17, 24, 16)
	sg::MatrixTransform *building18_XForm = new sg::MatrixTransform();
	building18_XForm->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, UNIT_24 / 2, 16 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_24, UNIT_4)));
	root->addChild(building18_XForm);

	sg::Cube *building18 = new sg::Cube();
	building18->setName("Building: 18");
	building18->setColor(glm::vec4(1, 0, 0, 1));
	building18_XForm->addChild(building18);

	// Building 19: (14, 16, 19)
	sg::MatrixTransform *building19_XForm = new sg::MatrixTransform();
	building19_XForm->setMatrix(glm::translate(glm::vec3(14 * UNIT_SIZE, UNIT_16 / 2, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_8, UNIT_16, UNIT_2)));
	root->addChild(building19_XForm);

	sg::Cube *building19 = new sg::Cube();
	building19->setName("Building: 19");
	building19->setColor(glm::vec4(1, 0, 0, 1));
	building19_XForm->addChild(building19);

	// Building 20: (6, 8, 9)
	sg::MatrixTransform *building20_XForm = new sg::MatrixTransform();
	building20_XForm->setMatrix(glm::translate(glm::vec3(6 * UNIT_SIZE, UNIT_8 / 2, 9 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_6)));
	root->addChild(building20_XForm);

	sg::Cube *building20 = new sg::Cube();
	building20->setName("Building: 20");
	building20->setColor(glm::vec4(1, 0, 0, 1));
	building20_XForm->addChild(building20);

	// Building 21: (-6, 6, 17)
	sg::MatrixTransform *building21_XForm = new sg::MatrixTransform();
	building21_XForm->setMatrix(glm::translate(glm::vec3(-6 * UNIT_SIZE, UNIT_6 / 2, 17 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_6, UNIT_2)));
	root->addChild(building21_XForm);

	sg::Cube *building21 = new sg::Cube();
	building21->setName("Building: 21");
	building21->setColor(glm::vec4(1, 0, 0, 1));
	building21_XForm->addChild(building21);

	// Building 22: (-8, 8, 13)
	sg::MatrixTransform *building22_XForm = new sg::MatrixTransform();
	building22_XForm->setMatrix(glm::translate(glm::vec3(-8 * UNIT_SIZE, UNIT_8 / 2, 13 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_2)));
	root->addChild(building22_XForm);

	sg::Cube *building22 = new sg::Cube();
	building22->setName("Building: 22");
	building22->setColor(glm::vec4(1, 0, 0, 1));
	building22_XForm->addChild(building22);

	// Building 23: (-13, 16, 19)
	sg::MatrixTransform *building23_XForm = new sg::MatrixTransform();
	building23_XForm->setMatrix(glm::translate(glm::vec3(-13 * UNIT_SIZE, UNIT_16 / 2, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_10, UNIT_16, UNIT_2)));
	root->addChild(building23_XForm);

	sg::Cube *building23 = new sg::Cube();
	building23->setName("Building: 23");
	building23->setColor(glm::vec4(1, 0, 0, 1));
	building23_XForm->addChild(building23);

	// Building 24: (-12, 8, 15)
	sg::MatrixTransform *building24_XForm = new sg::MatrixTransform();
	building24_XForm->setMatrix(glm::translate(glm::vec3(-12 * UNIT_SIZE, UNIT_8 / 2, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_4, UNIT_8, UNIT_2)));
	root->addChild(building24_XForm);

	sg::Cube *building24 = new sg::Cube();
	building24->setName("Building: 24");
	building24->setColor(glm::vec4(1, 0, 0, 1));
	building24_XForm->addChild(building24);

	// Building 25: (17, 12, 15)
	sg::MatrixTransform *building25_XForm = new sg::MatrixTransform();
	building25_XForm->setMatrix(glm::translate(glm::vec3(-17 * UNIT_SIZE, UNIT_12 / 2, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_12, UNIT_6)));
	root->addChild(building25_XForm);

	sg::Cube *building25 = new sg::Cube();
	building25->setName("Building: 25");
	building25->setColor(glm::vec4(1, 0, 0, 1));
	building25_XForm->addChild(building25);

	// Building 26: (19, 16, 12)
	sg::MatrixTransform *building26_XForm = new sg::MatrixTransform();
	building26_XForm->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, UNIT_16 / 2, 12 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_16, UNIT_16)));
	root->addChild(building26_XForm);

	sg::Cube *building26 = new sg::Cube();
	building26->setName("Building: 26");
	building26->setColor(glm::vec4(1, 0, 0, 1));
	building26_XForm->addChild(building26);

	// Building 27: (-3, 24, 8)
	sg::MatrixTransform *building27_XForm = new sg::MatrixTransform();
	building27_XForm->setMatrix(glm::translate(glm::vec3(-3 * UNIT_SIZE, UNIT_24 / 2, 8 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_24, UNIT_4)));
	root->addChild(building27_XForm);

	sg::Cube *building27 = new sg::Cube();
	building27->setName("Building: 27");
	building27->setColor(glm::vec4(1, 0, 0, 1));
	building27_XForm->addChild(building27);

	// Building 28: (3, 24, -8)
	sg::MatrixTransform *building28_XForm = new sg::MatrixTransform();
	building28_XForm->setMatrix(glm::translate(glm::vec3(3 * UNIT_SIZE, UNIT_24 / 2, -8 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_2, UNIT_24, UNIT_4)));
	root->addChild(building28_XForm);

	sg::Cube *building28 = new sg::Cube();
	building28->setName("Building: 28");
	building28->setColor(glm::vec4(1, 0, 0, 1));
	building28_XForm->addChild(building28);

	// Note: Ramp needs to be reduced by 45% in the y-axis to match unit_size. 55% for other axis
	// Ramp 1: (7, 0, -7)
	sg::MatrixTransform *ramp1_XForm = new sg::MatrixTransform();
	ramp1_XForm->setMatrix(glm::translate(glm::vec3(7 * UNIT_SIZE, 0, -7 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 24, .55 * 6)));
	root->addChild(ramp1_XForm);

	sg::ObjNode *ramp1 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp1->setName("Ramp: 1");
	ramp1->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp1_XForm->addChild(ramp1);

	// Ramp 2: (-7, 0, 7)	
	sg::MatrixTransform *ramp2_XForm = new sg::MatrixTransform();
	ramp2_XForm->setMatrix(glm::translate(glm::vec3(-7 * UNIT_SIZE, 0, 7 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 24, .55 * 6)) * Utilities::rotateY(180));
	root->addChild(ramp2_XForm);

	sg::ObjNode *ramp2 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp2->setName("Ramp: 2");
	ramp2->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp2_XForm->addChild(ramp2);

	ramp2->calculateBoundingBox();
	ramp2->getBoundingBox().print();

	// Ramp 3: (-12, 0, -14)
	sg::MatrixTransform *ramp3_XForm = new sg::MatrixTransform();
	ramp3_XForm->setMatrix(glm::translate(glm::vec3(-12 * UNIT_SIZE, 0, -14 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 4, 0.45 * 8, .55 * 4)));
	root->addChild(ramp3_XForm);

	sg::ObjNode *ramp3 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp3->setName("Ramp: 3");
	ramp3->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp3_XForm->addChild(ramp3);

	// Ramp 4: (-19, 8, 0)
	sg::MatrixTransform *ramp4_XForm = new sg::MatrixTransform();
	ramp4_XForm->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, 8 * UNIT_SIZE, 0)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 16, .55 * 6)));
	root->addChild(ramp4_XForm);

	sg::ObjNode *ramp4 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp4->setName("Ramp: 4");
	ramp4->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp4_XForm->addChild(ramp4);

	// Ramp 5: (5, 0, 15)
	sg::MatrixTransform *ramp5_XForm = new sg::MatrixTransform();
	ramp5_XForm->setMatrix(glm::translate(glm::vec3(5 * UNIT_SIZE, 0, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 6, 0.45 * 8, .55 * 2)) * Utilities::rotateY(90));
	root->addChild(ramp5_XForm);

	sg::ObjNode *ramp5 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp5->setName("Ramp: 5");
	ramp5->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp5_XForm->addChild(ramp5);

	// Ramp 6: (5, 0, 19)
	sg::MatrixTransform *ramp6_XForm = new sg::MatrixTransform();
	ramp6_XForm->setMatrix(glm::translate(glm::vec3(5 * UNIT_SIZE, 0, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 10, 0.45 * 32, .55 * 2)) * Utilities::rotateY(90));
	root->addChild(ramp6_XForm);

	sg::ObjNode *ramp6 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp6->setName("Ramp: 6");
	ramp6->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp6_XForm->addChild(ramp6);

	// Ramp 7: (17, 0, 8)
	sg::MatrixTransform *ramp7_XForm = new sg::MatrixTransform();
	ramp7_XForm->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, 0, 7.95 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 16, .55 * 8)) * Utilities::rotateY(180));
	root->addChild(ramp7_XForm);

	sg::ObjNode *ramp7 = new sg::ObjNode(SMALLRAMP, BLOCKS);
	ramp7->setName("Ramp: 7");
	ramp7->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	ramp7_XForm->addChild(ramp7);

	ramp7->getBoundingBox().print();
}

void Level::destroyLevel() {

}

sg::MatrixTransform* Level::getRoot() {
	return root;
}

void Level::setRoot(sg::MatrixTransform *newRoot) {
	root = newRoot;
}