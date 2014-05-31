#pragma once
#ifndef CITY_H
#define CITY_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <algorithm>
#include <math.h>

#include "Group.h"
#include "Utilities.h"
#include "tiny_obj_loader.h"
#include "../Common/World.h"
#include "Building.h"

using namespace std;

namespace sg {
	class City : public Group {
		public: 
			int m_heightMapXShift;
			int m_heightMapZShift;
			int m_numBuildings; // technically = to m_child.size() for now
			int m_maxArraySize;
			int m_maxNumArrays;

			float m_cityScale;
			float m_canScale;
			glm::vec3 m_defaultScale;

			int* m_nVertices;
			float** m_vertices;
			float** m_normals;
			float** m_texcoords;
			int** m_indices;
			int* m_nIndices;
			
			//vector<vector<int>> m_heightMap;

			City();

			// BUGGY 
			//City(string filename);
			~City();
			void initArrays();
			void draw(glm::mat4 parent, glm::mat4 camera);

			void updateHeightMap();
			void loadData(string inputfile);
			bool loadDataAtPlace(string inputfile, glm::vec3 position);

			void print();
			
	};
}

#endif