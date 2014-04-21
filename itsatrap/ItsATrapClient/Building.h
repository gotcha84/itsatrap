#pragma once
#ifndef BUILDING_H
#define BUILDING_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <algorithm>
#include <math.h>

#include "Geode.h"
#include "Utilities.h"
#include "tiny_obj_loader.h"
#include "World.h"

using namespace std;

namespace sg {
	class Building : public Geode {
		public: 
			int m_heightMapXShift;
			int m_heightMapZShift;

			int* m_nVertices;
			float** m_vertices;
			float** m_normals;
			float** m_texcoords;
			int** m_indices;
			int* m_nIndices;

			//vector<vector<int>> m_heightMap;

			Building();

			// BUGGY 
			//Building(string filename);
			~Building();
			void initArrays();
			void draw(glm::mat4 parent, glm::mat4 camera);

			// TODO: figure out what it takes 
			void updateHeightMap();
			void loadData(string inputfile);
			void drawShape();
	};
}

#endif