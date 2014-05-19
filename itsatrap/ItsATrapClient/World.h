#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include "AABB.h"

// REFERENCE FROM Level.h
// TODO (ktngo): Figure how to link the header file without crashing everything
#define FLOOR_HEIGHT	0.01f
#define MAP_X_LENGTH	40
#define MAP_Z_LENGTH	40
#define UNIT_SIZE		27

using namespace std;

class World{
	public:
		static int m_heightMapXShift;
		static int m_heightMapZShift;

		static float m_heightMap[UNIT_SIZE * MAP_X_LENGTH][UNIT_SIZE * MAP_Z_LENGTH];
		//static float m_heightMap[1019][787];

		void initializeHeightMap();
		void updateHeightMap(AABB boundingBox);
		void printHeightMapToFile(string file);
};

#endif