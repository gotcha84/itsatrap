#include "World.h"

int World::m_heightMapXShift = UNIT_SIZE * MAP_X_LENGTH / 2 -1;
int World::m_heightMapZShift = UNIT_SIZE * MAP_Z_LENGTH / 2 -1;
float World::m_heightMap[UNIT_SIZE * MAP_X_LENGTH][UNIT_SIZE * MAP_Z_LENGTH];
vector<AABB> World::m_boundingBoxes;

void World::initializeHeightMap() {
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			m_heightMap[x][z] = FLOOR_HEIGHT;
		}
	}
}

void World::updateHeightMap(AABB boundingBox) {
	//float temp = 0;
	for (int x = boundingBox.minX; x <= boundingBox.maxX; ++x) {
		for (int z = boundingBox.minZ; z <= boundingBox.maxZ; ++z) {
			//temp = m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift];
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < boundingBox.maxY) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = boundingBox.maxY;
				//temp = m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift];
			}
		}
	}
}

void World::printHeightMapToFile(string file) {
	ofstream outputfile = ofstream(file);
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			outputfile << "[" << x << "] [" << z << "]:" << m_heightMap[x][z] << endl;
		}
	}
}

void World::readInHeightMapFromFile(string file) {
	string line = "";
	ifstream inputfile;
	inputfile.open(file, std::ifstream::in);

	if (inputfile.is_open()) {
		while (getline(inputfile, line)) {
			string x = line.substr(1, line.find_first_of(']') - 1);
			string z = line.substr(line.find_last_of('[') + 1, line.find_last_of(']') - line.find_last_of('[') - 1);
			string value = line.substr(line.find_first_of(':') + 1);
			m_heightMap[stoi(x)][stoi(z)] = stof(value);
		}
		inputfile.close();
	} else {
		cout << "[ERR]: World.cpp - cannot find heightMap.txt!" << endl;
	}
}

void World::addBoundingBox(AABB boundingBox) {
	m_boundingBoxes.push_back(boundingBox);
}