#include "World.h"

int World::m_heightMapXShift = UNIT_SIZE * MAP_X_LENGTH / 2;
int World::m_heightMapZShift = UNIT_SIZE * MAP_Z_LENGTH / 2;
float World::m_heightMap[UNIT_SIZE * MAP_X_LENGTH][UNIT_SIZE * MAP_Z_LENGTH];
vector<AABB> World::m_boundingBoxes;

void World::initializeHeightMap() {
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			m_heightMap[x][z] = FLOOR_HEIGHT;
		}
	}
}

void World::updateHeightMap(AABB boundingBox, float offset) {
	for (int x = boundingBox.minX; x <= boundingBox.maxX; ++x) {
		for (int z = boundingBox.minZ; z <= boundingBox.maxZ; ++z) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < (boundingBox.maxY + offset)) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = (boundingBox.maxY + offset);
			}
		}
	}
}

void World::updateHeightMap(AABB boundingBox) {
	updateHeightMap(boundingBox, 0);
}

void World::printHeightMapToFile(string file) {
	ofstream outputfile = ofstream(file);
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			outputfile << "[" << x << "] [" << z << "]: " << m_heightMap[x][z] << endl;
		}
	}
}

void World::addBoundingBox(AABB boundingBox) {
	m_boundingBoxes.push_back(boundingBox);
}