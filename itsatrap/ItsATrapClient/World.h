#ifndef WORLD_H
#define WORLD_H

#include "SceneGraph.h"
#include <vector>
#include <algorithm>
#include "Utilities.h"
#include <math.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/ext.hpp>
#include "tiny_obj_loader.h"
#include <fstream>
#include <climits>

using namespace std;

class World {
	
	public: 

		static int m_heightMapXShift;
		static int m_heightMapZShift;

		/*static int m_worldNVertices;
		static vector <float> m_worldVertices;
		static vector <float> m_worldNormals;
		static vector <float> m_worldTexcoords;
		static vector <int> m_worldIndices;
		static int m_worldNIndices;*/

		/*static int m_worldNVertices;
		static vector <float> m_worldVertices;
		static vector <float> m_worldNormals;
		static vector <float> m_worldTexcoords;
		static vector <int> m_worldIndices;
		static int m_worldNIndices;*/

		static int* m_worldNVertices;
		static float** m_worldVertices;
		static float** m_worldNormals;
		static float** m_worldTexcoords;
		static int** m_worldIndices;
		static int* m_worldNIndices;

		/*static int m_worldNVertices [];
		static float m_worldVertices [][10000];
		static float m_worldNormals [][10000];
		static float m_worldTexcoords [][10000];
		static int m_worldIndices [][10000];
		static int m_worldNIndices [];
		static int m_worldPartitions [];*/


		//vector<vector<int>> m_heightMap;
		static float m_heightMap[1019][787];
		//float m_heightMap[][];

		World();


		// TODO: figure out what it takes 
		static void initializeHeightMap();
		void loadWorldData();
		
		
};

#endif