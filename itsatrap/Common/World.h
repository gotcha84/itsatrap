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

#include "Exports.h"
#include "AABB.h"

// REFERENCE FROM Level.h
// TODO (ktngo): Figure how to link the header file without crashing everything
#define FLOOR_HEIGHT	0.01f
#define MAP_X_LENGTH	40
#define MAP_Z_LENGTH	40
#define UNIT_SIZE		27

#define MAP_X_LENGTH_MIKE 1500
#define MAP_Z_LENGTH_MIKE 2500

using namespace std;

class COMMON_API World {
	public:
		static int m_heightMapXShift;
		static int m_heightMapZShift;

		static int m_structuresMapXShift;
		static int m_structuresMapZShift;

		static float m_heightMap[UNIT_SIZE * MAP_X_LENGTH + 1][UNIT_SIZE * MAP_Z_LENGTH + 1];
		static vector<int> m_structuresMap[MAP_X_LENGTH_MIKE][MAP_Z_LENGTH_MIKE];
		static vector<AABB> m_boundingBoxes;

		static void updateStructuresMap(AABB boundingBox, int id);

		static void initializeHeightMap();
		static void superHeightMapInit(vector<AABB> buildings, vector<AABB> ramps);
		static void updateHeightMap(AABB boundingBox);
		static float updateHeightMapRamp(AABB boundingBox, int rotation);
		static float updateHeightMapRamp0(AABB boundingBox);
		static float updateHeightMapRamp90(AABB boundingBox);
		static float updateHeightMapRamp180(AABB boundingBox);
		static float updateHeightMapRamp270(AABB boundingBox);
		static void printHeightMapToFile(string file);
		static void readInHeightMapFromFile(string file);

		static AABB* fixBoundingBox(AABB boundingBox);
};

#endif