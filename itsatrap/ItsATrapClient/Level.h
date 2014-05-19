#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneGraph.h"

#define FLOOR_HEIGHT	0.01f
#define MAP_X_LENGTH	40
#define MAP_Z_LENGTH	40
#define UNIT_SIZE		27
#define UNIT_2			UNIT_SIZE * 2
#define UNIT_3			UNIT_SIZE * 3
#define UNIT_4			UNIT_SIZE * 4
#define UNIT_5			UNIT_SIZE * 5
#define UNIT_6			UNIT_SIZE * 6
#define UNIT_8			UNIT_SIZE * 8
#define UNIT_10			UNIT_SIZE * 10
#define UNIT_12			UNIT_SIZE * 12
#define UNIT_13			UNIT_SIZE * 13
#define UNIT_16			UNIT_SIZE * 16
#define UNIT_20			UNIT_SIZE * 20
#define UNIT_24			UNIT_SIZE * 24
#define UNIT_40			UNIT_SIZE * 40

#define BLOCKS			"../Models/Blocks/"
#define POLYNOIDS		"../Models/Polynoid/"
#define CLIMBBLOCK		"../Models/Blocks/ClimbBlock.obj"
#define CROSSBAR		"../Models/Blocks/CrossBar.obj"
#define LARGERAMP		"../Models/Blocks/LargeRamp.obj"
#define RESOURCETOWER	"../Models/Blocks/ResourceTower.obj"
#define SMALLBLOCK		"../Models/Blocks/SmallBlock.obj"
#define SMALLRAMP		"../Models/Blocks/SmallRamp.obj"
#define POLYNOID		"../Models/Polynoid/Polynoid.obj"


class Level {
	public:
		sg::MatrixTransform *root;
		sg::MatrixTransform *ground;
		World world;

		Level();
		~Level();

		void initLevel();
		void destroyLevel();

		sg::MatrixTransform* getRoot();
		void setRoot(sg::MatrixTransform *newRoot);
};

#endif