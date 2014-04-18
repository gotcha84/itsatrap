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

World::World() {
	
	//initializeHeightMap();
	//m_heightMap = vector<vector<int>>(10, vector<int>(10, 0));
}


// TODO: implement
void World::initializeHeightMap() {
	
	int x_shift = 278;
	int z_shift = 463;

	float minx;
	float maxx;
	float minz;
	float maxz;
	float miny;
	float maxy;
	float tmpminx;
	float tmpminy;
	float tmpminz;
	float tmpmaxx;
	float tmpmaxy;
	float tmpmaxz;

	//int arr[1][2] = {{0, 1}};
	int arr[4][2] = {{0, 1}, {0, 3}, {1, 3}, {2, 3}};  
	//int arr[5][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}};   
	for (int i = 0; i < city_nVerts*3; i++) {
		city_vertices[i]/=10.0;
	}

	// TODO: fix min/max (only takes 2 args)
	for (int i = 0; i < city_nVerts*3; i+=12) { // else 84 or  126
		tmpminx = min(min(city_vertices[i], city_vertices[i+3]), min(city_vertices[i+6], city_vertices[i+9])); 
		tmpmaxx = max(max(city_vertices[i], city_vertices[i+3]), max(city_vertices[i+6], city_vertices[i+9])); 
		tmpminy = min(min(city_vertices[i+1], city_vertices[i+4]), min(city_vertices[i+7], city_vertices[i+10])); 
		tmpmaxy = max(max(city_vertices[i+1], city_vertices[i+4]), max(city_vertices[i+7], city_vertices[i+10])); 
		tmpminz = min(min(city_vertices[i+2], city_vertices[i+5]), min(city_vertices[i+8], city_vertices[i+11])); 
		tmpmaxz = max(max(city_vertices[i+2], city_vertices[i+5]), max(city_vertices[i+8], city_vertices[i+11]));

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
				if (maxy > m_heightMap[j+x_shift][k+z_shift]) {
					m_heightMap[j+x_shift][k+z_shift] = maxy;
				}
			}
		}
	}

	
	// player is 4 feet "tall"
	for (int i = 0; i < 1019; i++) {
		for (int j = 0; j < 787; j++) {
				m_heightMap[i][j]+=4;
		}
	}

}

void World::loadWorldData() {
	ObjReader::readObj("city.obj", city_nVerts, &city_vertices, &city_normals, &city_texcoords, city_nIndices, &city_indices);
}