#include "World.h"

//int World::m_heightMapXShift = 278;
//int World::m_heightMapZShift = 463;
//float World::m_heightMap[1019][787];
int World::m_heightMapXShift = UNIT_SIZE * MAP_X_LENGTH / 2;
int World::m_heightMapZShift = UNIT_SIZE * MAP_Z_LENGTH / 2;
float World::m_heightMap[UNIT_SIZE * MAP_X_LENGTH][UNIT_SIZE * MAP_Z_LENGTH];

void World::initializeHeightMap() {
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			m_heightMap[x][z] = FLOOR_HEIGHT;
		}
	}
}

void World::updateHeightMap(AABB boundingBox) {
	for (int x = boundingBox.m_minX; x <= boundingBox.m_maxX; ++x) {
		for (int z = boundingBox.m_minZ; z <= boundingBox.m_maxZ; ++z) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < boundingBox.m_maxY) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = boundingBox.m_maxY;
			}
		}
	}
}

void World::printHeightMapToFile(string file) {
	ofstream outputfile = ofstream(file);
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			outputfile << "[" << x << "] [" << z << "]: " << m_heightMap[x][z] << endl;
		}
	}
}