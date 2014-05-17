#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneGraph.h"

#define UNIT_SIZE		27
#define BLOCKS			"../Models/Blocks/"
#define POLYNOIDS		"../Models/Polynoids/"
#define CLIMBBLOCK		"../Models/Blocks/ClimbBlock.obj"
#define CROSSBAR		"../Models/Blocks/CrossBar.obj"
#define LARGERAMP		"../Models/Blocks/LargeRamp.obj"
#define RESOURCETOWER	"../Models/Blocks/ResourceTower.obj"
#define SMALLBLOCK		"../Models/Blocks/SmallBlock.obj"
#define SMALLRAMP		"../Models/Blocks/SmallRamp.obj"
#define POLYNOID		"../Models/Polynoids/Polynoid.obj"


class Level {
	public:
		sg::MatrixTransform *root;
		sg::MatrixTransform *ground;

		Level();
		~Level();

		void initLevel();
		void destroyLevel();

		sg::MatrixTransform* getRoot();
		void setRoot(sg::MatrixTransform *newRoot);
};

#endif