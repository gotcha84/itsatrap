#ifndef WORLD_H
#define WORLD_H

#include "SceneGraph.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/ext.hpp>
#include "objreader.h"

using namespace std;

class World {
	
	public: 
		

		int city_nVerts;
		float *city_vertices;
		float *city_normals;
		float *city_texcoords;
		int city_nIndices;
		int *city_indices;
		float city_colors[45][3];

		//vector<vector<int>> m_heightMap;
		float m_heightMap[1019][787];
		//float m_heightMap[][];

		World();

		// TODO: figure out what it takes in
		void initializeHeightMap();
		void loadWorldData();
};

#endif