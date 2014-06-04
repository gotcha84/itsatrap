#define ENABLE_PARTICLES 1
#define NUMPARTICLES 1000

#include "Level.h"

Level::Level() {
	root = new sg::MatrixTransform();
	root->setName("LEVEL ROOT");

	ground = new sg::MatrixTransform();
	ground->setName("LEVEL GROUND");
	root->addChild(ground);

	this->initLevel0();
}

Level::~Level() {
	this->destroyLevel();
	delete ground;
	ground = nullptr;
	delete root;
	root = nullptr;
}

void Level::initLevel0() {
	DIR *dir;
	struct dirent *ent;

	int counter = 0;
	float alpha = 0.9f;
	int resourceCounter = 0;

	sg::MatrixTransform *xForm = new sg::MatrixTransform();
	ground->addChild(xForm);

	//if ((dir = opendir(LEVEL_DIR))!= NULL) {
	//if ((dir = opendir(OBELISK_DIR))!= NULL) {
	if ((dir = opendir(OBELISK2_DIR)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			string fileName(ent->d_name);
			if (fileName.size() > 3) {
				string part(fileName.substr(0, fileName.find_first_of('_')));
				string extension(fileName.substr(fileName.find_first_of('.')));

				if (extension == ".obj") {
					if (part == "ResourceTower") {
						sg::ResourceNode *rs; // temp var to reference resource nodes

						rs = new sg::ResourceNode(resourceCounter, NUMPARTICLES);
						//rs->loadModel(LEVEL + fileName, LEVEL);
						//rs->loadModel(OBELISK + fileName, OBELISK);
						rs->loadModel(OBELISK2 + fileName, OBELISK2);
						rs->getParticleSystem()->setColor(glm::vec4(1, 0, 0, 1));
						rs->m_particles2->setColor(glm::vec4(0, 1, 0, 1));
						rs->m_particles2->reverse();
						if (!ENABLE_PARTICLES) {
							rs->getParticleSystem()->disable();
							rs->m_particles2->disable();
						}
						resources.push_back(rs);
						resources.back()->setName("Resource Tower " + resourceCounter);
						resources.back()->getModel()->setColor(glm::vec4(1, 1, 1, 1));
						xForm->addChild(resources.back());

						resources.back()->calculateBoundingBox();

						float x = (resources.back()->getBoundingBox().minX + resources.back()->getBoundingBox().maxX) / 2.0f;
						float z = (resources.back()->getBoundingBox().minZ + resources.back()->getBoundingBox().maxZ) / 2.0f;
						float y = resources.back()->getBoundingBox().minY;

						resources.back()->setParticleSystemOrigin(glm::vec3(x, y, z + 25.0f));

						++resourceCounter;
					} else if (part == "BRail" || part == "ERail" || part == "Rail" || part == "Caltrop") {
						levelNodes.push_back({ new sg::ObjNode(OBELISK2 + fileName, OBELISK2), true });
						levelNodes.back().first->setName("ObjNode: " + fileName);

						if (counter % 1000 == 0) {
							++counter;
						}

						levelNodes.back().first->getModel()->setColor(glm::vec4(0.1f*(float)(counter / 100), 0.1f*(float)((counter % 100) / 10), 0.5f + 0.05*(float)(counter % 10), alpha));
						xForm->addChild(levelNodes.back().first);
					} else if (part != "Ramp" && part != "UD") {
						//levelNodes.push_back(new sg::ObjNode(LEVEL + fileName, LEVEL));
						//levelNodes.push_back(new sg::ObjNode(OBELISK + fileName, OBELISK));
						levelNodes.push_back({ new sg::ObjNode(OBELISK2 + fileName, OBELISK2), false });
						levelNodes.back().first->setName("ObjNode: " + fileName);

						if (counter % 1000 == 0) {
							++counter;
						}

						//string color = Utilities::intToBaseThree(counter % 27);
						levelNodes.back().first->getModel()->setColor(glm::vec4(0.1f*(float)(counter/100), 0.1f*(float)((counter%100)/10), 0.5f+0.05*(float)(counter%10), alpha));
						//levelNodes.back()->loadTexture("../Models/Polynoid_Updated/animus.ppm");
						xForm->addChild(levelNodes.back().first);
					} 
				}
				++counter;
			}
		}
		closedir(dir);
	}
	else {
		cout << "[ERROR]: Level.cpp - Could not read in Level obj files!" << endl;
	}
	cout << "size: " << levelNodes.size() << endl;
	for (int i = 0; i < levelNodes.size(); ++i) {
		levelNodes[i].first->calculateBoundingBox();

		// ANDRE
		/*if (levelNodes[i]->m_boundingBox.minX < 200 && levelNodes[i]->m_boundingBox.maxX > 200 &&
			levelNodes[i]->m_boundingBox.minZ < -300 && levelNodes[i]->m_boundingBox.maxZ > -300) {
			cout << "COLLIDED!!!! i: " << i << ", name: " << levelNodes[i]->getName() << endl;
		/*}
		/*
		if (i < 200) {
			cout << "i: " << i;
			levelNodes[i]->getBoundingBox().print();
		}*/
		//levelNodes[i]->enableDrawBB();
	}

	disableAllResourceNodes();
}

void Level::initLevel() {
	activeResourceNode = 0;
	sg::ResourceNode *rs; // temp var to reference resource nodes

	// Initialize height map
	World::initializeHeightMap();

	// add basic ground
	sg::MatrixTransform *groundXForm = new sg::MatrixTransform();
	groundXForm->setMatrix(glm::scale(glm::vec3(MAP_X_LENGTH * UNIT_SIZE, FLOOR_HEIGHT, MAP_Z_LENGTH * UNIT_SIZE)));

	ground->addChild(groundXForm);

	groundCube = new sg::Cube();
	groundCube->setName("ground cube");
	groundCube->setColor(glm::vec4(0, 1, 0, 1));
	groundXForm->addChild(groundCube);

	// Add obstacles + buildings to level
	// Resource Tower #1
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(0, UNIT_12, 0)) * glm::scale(glm::vec3(5, 10, 5)));
	root->addChild(xForms.back());

	rs = new sg::ResourceNode(0, NUMPARTICLES);
	rs->loadModel(RESOURCETOWER, BLOCKS);
	rs->getParticleSystem()->setColor(glm::vec4(1, 0, 0, 1));
	rs->m_particles2->setColor(glm::vec4(1, 0, 0, 1));
	rs->m_particles2->reverse();
	if (!ENABLE_PARTICLES) {
		rs->getParticleSystem()->disable();
		rs->m_particles2->disable();
	}
	resources.push_back(rs);
	resources.back()->setName("Resource Tower 0: (0, 0, 0)");
	resources.back()->getModel()->setColor(glm::vec4(1, 1, 1, 1));
	xForms.back()->addChild(resources.back());

	// Resource Tower #2
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-15 * UNIT_SIZE, UNIT_8, 7 * UNIT_SIZE)) * glm::scale(glm::vec3(5, 10, 5)));
	root->addChild(xForms.back());

	rs = new sg::ResourceNode(1, NUMPARTICLES);
	rs->loadModel(RESOURCETOWER, BLOCKS);
	rs->getParticleSystem()->setColor(glm::vec4(0, 1, 0, 1));
	rs->m_particles2->setColor(glm::vec4(0, 1, 0, 1));
	rs->m_particles2->reverse();
	if (!ENABLE_PARTICLES) {
		rs->getParticleSystem()->disable();
		rs->m_particles2->disable();
	}
	resources.push_back(rs);
	resources.back()->setName("Resource Tower 1: (-15, 8, 7)");
	resources.back()->getModel()->setColor(glm::vec4(1, 1, 1, 1));
	xForms.back()->addChild(resources.back());

	// Resource Tower #3
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(15 * UNIT_SIZE, UNIT_8, -7 * UNIT_SIZE)) * glm::scale(glm::vec3(5, 10, 5)));
	root->addChild(xForms.back());

	rs = new sg::ResourceNode(2, NUMPARTICLES);
	rs->loadModel(RESOURCETOWER, BLOCKS);
	rs->getParticleSystem()->setColor(glm::vec4(0, 0, 1, 1));
	rs->m_particles2->setColor(glm::vec4(0, 0, 1, 1));
	rs->m_particles2->reverse();
	if (!ENABLE_PARTICLES) {
		rs->getParticleSystem()->disable();
		rs->m_particles2->disable();
	}
	resources.push_back(rs);
	resources.back()->setName("Resource Tower 2: (-15, 8, 7)");
	resources.back()->getModel()->setColor(glm::vec4(1, 1, 1, 1));
	xForms.back()->addChild(resources.back());

	// Building 0: (0, 0, 0)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(0, UNIT_12 / 2, 0)) * glm::scale(glm::vec3(UNIT_16, UNIT_12, UNIT_8)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 0: (0, 0, 0)");
	buildings.back()->setColor(glm::vec4(0.5, 0.3, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 1: (-15, 8, 7)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-15 * UNIT_SIZE, UNIT_8 / 2, 7 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 1: (-15, 8, 7)");
	buildings.back()->setColor(glm::vec4(0.5, 0.3, 0.5, 1));
	xForms.back()->addChild(buildings.back());

	// Building 2: (15, 8, -7)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(15 * UNIT_SIZE, UNIT_8 / 2, -7 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_6, UNIT_8, UNIT_6)));
	root->addChild(xForms.back());

	buildings.push_back(new sg::Cube());
	buildings.back()->setName("Building 2: (15, 8, -7)");
	buildings.back()->setColor(glm::vec4(0.5, 0.3, 0.5, 1));
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

	// Wall 0: (-21, 48, 0)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-21 * UNIT_SIZE, UNIT_48 / 2, 0)) * glm::scale(glm::vec3(UNIT_2, UNIT_48 * 4, UNIT_40)));
	root->addChild(xForms.back());

	walls.push_back(new sg::Cube());
	walls.back()->setName("Wall 0: (-21, 48, 0)");
	walls.back()->setColor(glm::vec4(0.7, 0.3, 0.6, 0.5));
	xForms.back()->addChild(walls.back());

	// Wall 1: (21, 48, 0)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(21 * UNIT_SIZE, UNIT_48 / 2, 0)) * glm::scale(glm::vec3(UNIT_2, UNIT_48 * 4, UNIT_40)));
	root->addChild(xForms.back());

	walls.push_back(new sg::Cube());
	walls.back()->setName("Wall 1: (21, 48, 0)");
	walls.back()->setColor(glm::vec4(0.7, 0.3, 0.6, 0.5));
	xForms.back()->addChild(walls.back());

	// Wall 2: (0, 48, -21)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(0, UNIT_48 / 2, -21 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_40, UNIT_48 * 4, UNIT_2)));
	root->addChild(xForms.back());

	walls.push_back(new sg::Cube());
	walls.back()->setName("Wall 2: (21, 48, 0)");
	walls.back()->setColor(glm::vec4(0.7, 0.3, 0.6, 0.5));
	xForms.back()->addChild(walls.back());

	// Wall 3: (0, 48, 21)
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(0, UNIT_48 / 2, 21 * UNIT_SIZE)) * glm::scale(glm::vec3(UNIT_40, UNIT_48, UNIT_2)));
	root->addChild(xForms.back());

	walls.push_back(new sg::Cube());
	walls.back()->setName("Wall 3: (21, 48, 0)");
	walls.back()->setColor(glm::vec4(0.7, 0.3, 0.6, 0.5));
	xForms.back()->addChild(walls.back());

	// Note: Ramp needs to be reduced by 45% in the y-axis to match unit_size. 55% for other axis
	// Note: COMMENT THESE FOLLOWING RAMPS TO GET RID OF THE RAMPS
	// Ramp 0: (7, 0, -8) +z
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(7 * UNIT_SIZE, 0, -8 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 24, .55 * 6)));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 0: (7, 0, -7)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 1: (-7, 0, 8)	-z
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-7 * UNIT_SIZE, 0, 8 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 24, .55 * 6)) * Utilities::rotateY(180));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 1: (-7, 0, 7)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 2: (-12, 0, -14) +z
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-12 * UNIT_SIZE, 0, -14 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 4, 0.45 * 8, .55 * 4)));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 2: (-12, 0, -14)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 3: (-19, 8, 0) +z
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(-19 * UNIT_SIZE, 8 * UNIT_SIZE, 0)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 16, .55 * 6)));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 3: (-19, 8, 0)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 4: (5, 0, 15) +x
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(5 * UNIT_SIZE, 0, 15 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 6, 0.45 * 8, .55 * 2)) * Utilities::rotateY(90));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 4: (5, 0, 15)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 5: (4, 0, 18) +x
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(4 * UNIT_SIZE, 0, 19 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 10, 0.45 * 32, .55 * 2)) * Utilities::rotateY(90));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 5: (5, 0, 19)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Ramp 6: (17, 0, 9) -z
	xForms.push_back(new sg::MatrixTransform());
	xForms.back()->setMatrix(glm::translate(glm::vec3(17 * UNIT_SIZE, 0, 9 * UNIT_SIZE)) * glm::scale(glm::vec3(.55 * 2, 0.45 * 16, .55 * 8)) * Utilities::rotateY(180));
	root->addChild(xForms.back());

	ramps.push_back(new sg::ObjNode(SMALLRAMP, BLOCKS));
	ramps.back()->setName("Ramp 6: (17, 0, 8)");
	ramps.back()->getModel()->setColor(glm::vec4(0, 0.5, 0.5, 1));
	xForms.back()->addChild(ramps.back());

	// Update Bounding Boxes and Height Map
	groundCube->calculateBoundingBox();

	// Building Height Map
	for (int i = 0; i < buildings.size(); ++i) {
		buildings[i]->calculateBoundingBox();
		World::updateHeightMap(buildings[i]->getBoundingBox());
	}

	for (int i = 0; i < walls.size(); ++i) {
		walls[i]->calculateBoundingBox();
	}

	// Resource Nodes
	for (int i = 0; i < resources.size(); ++i) {
		resources[i]->calculateBoundingBox();
	}

	float xLength;
	float zLength;
	// HARDCODING STUFF GET OUT OF MY WAY BITCHES
	for (int i = 0; i < resources.size(); ++i) {
		xLength = resources[i]->m_boundingBox.maxX - resources[i]->m_boundingBox.minX;
		zLength = resources[i]->m_boundingBox.maxZ - resources[i]->m_boundingBox.minZ;
		resources[i]->m_boundingBox.minX += xLength / 4.0f;
		resources[i]->m_boundingBox.minZ += zLength / 4.0f;
		resources[i]->m_boundingBox.maxX -= xLength / 4.0f;
		resources[i]->m_boundingBox.maxZ -= zLength / 4.0f;
	}

	// Ramp Height Map
	// NOTE: COMMENT THIS OUT TO GET RID OF RAMPS
	for (int i = 0; i < ramps.size(); ++i) {
		int num = stoi(ramps[i]->getName().substr(5, 1));
		float slope = 0.0f;
		ramps[i]->calculateBoundingBox();
		ramps[i]->setBoundingBox(*World::fixBoundingBox(ramps[i]->getBoundingBox()));

		switch (num)
		{
		case 0:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 0);
			break;
		case 1:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 180);
			break;
		case 2:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 0);
			break;
		case 3:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 0);
			break;
		case 4:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 90);
			break;
		case 5:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 90);
			break;
		case 6:
			slope = World::updateHeightMapRamp(ramps[i]->getBoundingBox(), 180);
			break;
		default:
			cout << "[ERR] Level.cpp - Unexpected Ramp Found!" << endl;
			break;
		}

		rampSlopes.push_back(slope);
	}

	//World::printHeightMapToFile("heightMap.txt");
	disableAllResourceNodes();
}

void Level::destroyLevel() {

}

sg::MatrixTransform* Level::getRoot() {
	return root;
}

void Level::setRoot(sg::MatrixTransform *newRoot) {
	root = newRoot;
}

void Level::disableAllResourceNodes() {
	for (int i = 0; i < resources.size(); ++i) {
		resources[i]->disableParticles();
		resources[i]->resetOwnerColor();
	}
	activeResourceNode = -1;
}

void Level::disableCurrentResourceNode() {
	for (int i = 0; i < resources.size(); ++i) {
		if (resources[i]->getResourceId() == activeResourceNode) {
			resources[i]->disableParticles();
			resources[i]->resetOwnerColor();
			activeResourceNode = -1;
		}
	}
}

void Level::activateResourceNode(int id) {
	for (int i = 0; i < resources.size(); ++i) {
		if (resources[i]->getResourceId() == id) {
			activeResourceNode = id;
			resources[i]->enableParticles();
		}
	}
}

void Level::setOwnerToResourceNode(int resourceId, int playerId) {
	for (int i = 0; i < resources.size(); ++i) {
		if (resources[i]->getResourceId() == resourceId) {
			resources[i]->setOwnerColor(playerId);
		}
	}
}
