#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneGraph.h"

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