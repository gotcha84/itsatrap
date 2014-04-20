#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include <climits>  
#include <algorithm>
#include "World.h"

using namespace std;

//int World::m_worldNVertices = 0;
//vector<float> World::m_worldVertices;
//vector<float> World::m_worldNormals;
//vector<float> World::m_worldTexcoords;
//vector <int> World::m_worldIndices;
//int World::m_worldNIndices = 0;



//int World::m_worldNVertices = 0;
//float* World::m_worldVertices;
//float* World::m_worldNormals;
//float* World::m_worldTexcoords;
//int* World::m_worldIndices;
//int World::m_worldNIndices = 0;




//int World::m_worldNVertices [100];
//float World::m_worldVertices [100][1000];
//float World::m_worldNormals [100][1000];
//float World::m_worldTexcoords [100][1000];
//int World::m_worldIndices [100][1000];
//int World::m_worldNIndices [100];
//int World::m_worldPartitions [100];

int	World::m_heightMapXShift = 278;
int	World::m_heightMapZShift = 463;

int* World::m_worldNVertices = new int[100];
float** World::m_worldVertices = new float*[100];
float** World::m_worldNormals = new float*[100];
float** World::m_worldTexcoords = new float*[100];
int** World::m_worldIndices = new int*[100];
int* World::m_worldNIndices = new int[100];

float World::m_heightMap[1019][787];

World::World() {
	cout << "hihi\n";
	//initializeHeightMap();

	//m_heightMap = vector<vector<int>>(10, vector<int>(10, 0));
}

void World::initializeHeightMap() {

	
	float minx;
	float maxx;
	float minz;
	float maxz;
	float miny;
	float maxy;
	float tmpminx = FLT_MAX;
	float tmpminy = FLT_MAX;
	float tmpminz = FLT_MAX;
	float tmpmaxx = FLT_MIN;
	float tmpmaxy = FLT_MIN;
	float tmpmaxz = FLT_MIN;

	//int arr[1][2] = {{0, 1}};
	int arr[4][2] = {{0, 1}, {0, 3}, {1, 3}, {2, 3}};  
	//int arr[5][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}};   
	//if (!init_heightMap) {
	for (int k = 0; k < 46; k++) {
		for (int i = 0; i < m_worldNVertices[k]; i++) {
			m_worldVertices[k][i]/=10.0f;
		}
	}
	//}

	// TODO: fix min/max (only takes 2 args)
	for (int l = 0; l < 46; l++) {
		//for (int i = 0; i < m_worldNVertices[l]-m_worldNVertices[l]%9; i+=9) { 
		for (int i = 0; i < m_worldNVertices[l]-m_worldNVertices[l]%12; i+=12) { 

			
			/*if (m_worldVertices[l][i] < tmpminx) {
				tmpminx = m_worldVertices[l][i];
			}
			if (m_worldVertices[l][i+1] < tmpminy) {
				tmpminy = m_worldVertices[l][i+1];
			}
			if (m_worldVertices[l][i+2] < tmpminz) {
				tmpminz = m_worldVertices[l][i+2];
			}
			if (m_worldVertices[l][i] > tmpmaxx) {
				tmpmaxx = m_worldVertices[l][i];
			}
			if (m_worldVertices[l][i+1] > tmpmaxy) {
				tmpmaxy = m_worldVertices[l][i+1];
			}
			if (m_worldVertices[l][i+2] > tmpmaxz) {
				tmpmaxz = m_worldVertices[l][i+2];
			}*/
		

			/*tmpminx = min(min(m_worldVertices[l][i], m_worldVertices[l][i+3]),  m_worldVertices[l][i+6]);
			tmpmaxx = max(max(m_worldVertices[l][i], m_worldVertices[l][i+3]),  m_worldVertices[l][i+6]);
			tmpminy = min(min(m_worldVertices[l][i+1], m_worldVertices[l][i+4]),  m_worldVertices[l][i+7]);
			tmpmaxy = max(max(m_worldVertices[l][i+1], m_worldVertices[l][i+4]),  m_worldVertices[l][i+7]);
			tmpminz = min(min(m_worldVertices[l][i+2], m_worldVertices[l][i+5]),  m_worldVertices[l][i+8]);
			tmpmaxz = max(max(m_worldVertices[l][i+2], m_worldVertices[l][i+5]),  m_worldVertices[l][i+8]);*/

			tmpminx = min(min(m_worldVertices[l][i], m_worldVertices[l][i+3]),  min(m_worldVertices[l][i+6], m_worldVertices[l][i+9]));
			tmpmaxx = max(max(m_worldVertices[l][i], m_worldVertices[l][i+3]),  max(m_worldVertices[l][i+6], m_worldVertices[l][i+9]));
			tmpminy = min(min(m_worldVertices[l][i+1], m_worldVertices[l][i+4]),  min(m_worldVertices[l][i+7], m_worldVertices[l][i+10]));
			tmpmaxy = max(max(m_worldVertices[l][i+1], m_worldVertices[l][i+4]),  max(m_worldVertices[l][i+7], m_worldVertices[l][i+10]));
			tmpminz = min(min(m_worldVertices[l][i+2], m_worldVertices[l][i+5]),  min(m_worldVertices[l][i+8], m_worldVertices[l][i+11]));
			tmpmaxz = max(max(m_worldVertices[l][i+2], m_worldVertices[l][i+5]),  max(m_worldVertices[l][i+8], m_worldVertices[l][i+11]));


			if (tmpminx < 0) {
				minx = -1*floor((-1.0*tmpminx)+0.5);
			}
			else {
				minx = floor(tmpminx+0.5);
			}

			if (tmpminz < 0) {
				minz = -1*floor((-1.0*tmpminz)+0.5);
			}
			else {
				minz = floor(tmpminz+0.5);
			}

			if (tmpmaxx < 0) {
				maxx = -1*floor((-1.0*tmpmaxx)+0.5);
			}
			else {
				maxx = floor(tmpmaxx+0.5);
			}

			if (tmpmaxz < 0) {
				maxz = -1*floor((-1.0*tmpmaxz)+0.5);
			}
			else {
				maxz = floor(tmpmaxz+0.5);
			}

			if (tmpminy < 0) {
				miny = -1*floor((-1.0*tmpminy)+0.5);
			}
			else {
				miny = floor(tmpminy+0.5);
			}

			if (tmpmaxy < 0) {
				maxy = -1*floor((-1.0*tmpmaxy)+0.5);
			}
			else {
				maxy = floor(tmpmaxy+0.5);
			}

			for (int j = minx; j < maxx; j++) {
				for (int k = minz; k < maxz; k++) {
					if (maxy > World::m_heightMap[j+World::m_heightMapXShift][k+World::m_heightMapZShift]) {
						World::m_heightMap[j+World::m_heightMapXShift][k+World::m_heightMapZShift] = maxy;
					}
				}
			}
		}
	}
	

	
	// player is 4 feet "tall"
	for (int i = 0; i < 1019; i++) {
		for (int j = 0; j < 787; j++) {
				World::m_heightMap[i][j]+=4;
				//cout << World::m_heightMap[i][j] << endl;
		}
	}

	
}


// TODO: implement initializeHeightMap
/*
void World::loadWorldData() {
	std::string inputfile = "city.obj";
    std::vector<tinyobj::shape_t> shapes;
  
    std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  
	int indicesCount = 0;
	int verticesCount = 0;
	int texturesCount = 0;
	int normalsCount = 0;
	int max_ele = -1;

	int tmpIndicesCount = 0;
	int tmpVerticesCount = 0;
	int tmpTexturesCount = 0;
	int tmpNormalsCount = 0;
	//cout << shapes.size() << endl;
	for (int j = 0; j < shapes.size(); j++) {
		tmpIndicesCount = 0;
		tmpVerticesCount = 0;
		tmpTexturesCount = 0;
		tmpNormalsCount = 0;
		for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
			World::m_worldIndices[i+indicesCount] = shapes[j].mesh.indices[i]+indicesCount;
			//cout << World::m_worldIndices[i+indicesCount] << endl;
			tmpIndicesCount++;
		}
		cout << tmpIndicesCount << endl;
		indicesCount += tmpIndicesCount;
		
		for (int i = 0; i < shapes[j].mesh.positions.size(); i++) {
			World::m_worldVertices[i+verticesCount] = shapes[j].mesh.positions[i];
			tmpVerticesCount++;
		}
		
		verticesCount += tmpVerticesCount;

		for (int i = 0; i < shapes[j].mesh.normals.size(); i++) {
			World::m_worldNormals[i+normalsCount] = shapes[j].mesh.normals[i];
			tmpNormalsCount++;
		}
		normalsCount += tmpNormalsCount;

		for (int i = 0; i < shapes[j].mesh.texcoords.size(); i++) {
			World::m_worldTexcoords[i+texturesCount] = shapes[j].mesh.texcoords[i];
			tmpTexturesCount++;
		}
		texturesCount +=tmpTexturesCount;
	
	}
	World::m_worldNIndices = indicesCount;
	World::m_worldNVertices = verticesCount;

	Utilities::writeIntArrayToFile(World::m_worldIndices, World::m_worldNIndices, "new_indices.txt");
	Utilities::writeFloatArrayToFile(World::m_worldVertices, World::m_worldNVertices, "new_vertices.txt");
	Utilities::writeFloatArrayToFile(World::m_worldNormals, normalsCount, "new_normals.txt");
	Utilities::writeFloatArrayToFile(World::m_worldTexcoords, texturesCount, "new_textures.txt");

}
*/


void World::loadWorldData() {

	for (int i = 0; i < 100; i++) {
		World::m_worldVertices[i] = new float[1000];
	}

	for (int i = 0; i < 100; i++) {
		World::m_worldNormals[i] = new float[1000];
	}

	for (int i = 0; i < 100; i++) {
		World::m_worldTexcoords[i] = new float[1000];
	}

	for (int i = 0; i < 100; i++) {
		World::m_worldIndices[i] = new int[1000];
	}

	std::string inputfile = "city.obj";
    std::vector<tinyobj::shape_t> shapes;
  
    std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  
	int indicesCount = 0;
	int verticesCount = 0;
	int texturesCount = 0;
	int normalsCount = 0;
	int max_ele = -1;

	int tmpIndicesCount = 0;
	int tmpVerticesCount = 0;
	int tmpTexturesCount = 0;
	int tmpNormalsCount = 0;
	//cout << shapes.size() << endl;
	for (int j = 0; j < 46; j++) {
		tmpIndicesCount = 0;
		tmpVerticesCount = 0;
		tmpTexturesCount = 0;
		tmpNormalsCount = 0;
		for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
			World::m_worldIndices[j][i] = shapes[j].mesh.indices[i];
			//cout << World::m_worldIndices[i+indicesCount] << endl;
			
			tmpIndicesCount++;
		}

		indicesCount += tmpIndicesCount;
		
		for (int i = 0; i < shapes[j].mesh.positions.size(); i++) {
			World::m_worldVertices[j][i] = shapes[j].mesh.positions[i];
			tmpVerticesCount++;
		}
		
		verticesCount += tmpVerticesCount;

		for (int i = 0; i < shapes[j].mesh.normals.size(); i++) {
			World::m_worldNormals[j][i] = shapes[j].mesh.normals[i];
			tmpNormalsCount++;
		}
		normalsCount += tmpNormalsCount;

		// TODO textures
		/*for (int i = 0; i < shapes[j].mesh.texcoords.size(); i++) {
			World::m_worldTexcoords[j][i+texturesCount] = shapes[j].mesh.texcoords[i];
			tmpTexturesCount++;
		}
		texturesCount +=tmpTexturesCount;*/
		World::m_worldNIndices[j] = tmpIndicesCount;
		World::m_worldNVertices[j] = tmpVerticesCount;
	
		
		

	}
	
	Utilities::writeIntArrayToFile(World::m_worldNVertices, 100, "nverts.txt");
	Utilities::writeFloatArrayToFile(World::m_worldVertices[0], 1000, "verts0.txt");

	//Utilities::writeIntArrayToFile(World::m_worldIndices, World::m_worldNIndices, "new_indices.txt");
	//Utilities::writeFloatArrayToFile(World::m_worldVertices, World::m_worldNVertices, "new_vertices.txt");
	//Utilities::writeFloatArrayToFile(World::m_worldNormals, normalsCount, "new_normals.txt");
	//Utilities::writeFloatArrayToFile(World::m_worldTexcoords, texturesCount, "new_textures.txt");

}
