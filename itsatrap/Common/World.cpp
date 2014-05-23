#include "World.h"

int World::m_heightMapXShift = UNIT_SIZE * MAP_X_LENGTH / 2;
int World::m_heightMapZShift = UNIT_SIZE * MAP_Z_LENGTH / 2;
float World::m_heightMap[UNIT_SIZE * MAP_X_LENGTH + 1][UNIT_SIZE * MAP_Z_LENGTH + 1];
vector<AABB> World::m_boundingBoxes;

void World::initializeHeightMap() {
	for (int x = 0; x < UNIT_SIZE * MAP_X_LENGTH; ++x) {
		for (int z = 0; z < UNIT_SIZE * MAP_Z_LENGTH; ++z) {
			m_heightMap[x][z] = FLOOR_HEIGHT;
		}
	}
}

void World::updateHeightMap(AABB boundingBox) {
	for (int x = boundingBox.minX; x < boundingBox.maxX; ++x) {
		for (int z = boundingBox.minZ; z < boundingBox.maxZ; ++z) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < boundingBox.maxY) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = boundingBox.maxY;
			}
		}
	}
}

void World::updateHeightMapRamp(AABB boundingBox, int rotation) {
	switch (rotation)
	{
	case 0:
		updateHeightMapRamp0(boundingBox);
		break;
	case 90:
		updateHeightMapRamp90(boundingBox);
		break;
	case 180:
		updateHeightMapRamp180(boundingBox);
		break;
	case 270:
		updateHeightMapRamp270(boundingBox);
		break;
	default:
		cout << "[ERR]: World.cpp - Ramp Rotation not found!" << endl;
		break;
	}
}

// Ramp inclines or declines along the z-axis (z, y)
void World::updateHeightMapRamp0(AABB boundingBox) {
	float slope = (boundingBox.maxY - boundingBox.minY) / (boundingBox.maxZ - boundingBox.minZ);
	float intercept = boundingBox.minY - (slope * boundingBox.minZ);

	for (int z = boundingBox.minZ; z <= boundingBox.maxZ; ++z) {
		float height = (slope * z) + intercept;
		//cout << "z: " << z << " - height: " << height << endl;
		for (int x = boundingBox.minX; x <= boundingBox.maxX; ++x) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < height) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = height;
			}
		}
	}
}

void World::updateHeightMapRamp180(AABB boundingBox) {
	float slope = (boundingBox.maxY - boundingBox.minY) / (boundingBox.minZ - boundingBox.maxZ);
	float intercept = boundingBox.minY - (slope * boundingBox.maxZ);

	for (int z = boundingBox.minZ; z <= boundingBox.maxZ; ++z) {
		float height = (slope * z) + intercept;
		//cout << "z: " << z << " - height: " << height << endl;
		for (int x = boundingBox.minX; x <= boundingBox.maxX; ++x) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < height) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = height;
			}
			//cout << m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] << endl;
		}
	}
}

// Ramp inclines or declines along the x-axis (x, y)
void World::updateHeightMapRamp90(AABB boundingBox) {
	float slope = (boundingBox.maxY - boundingBox.minY) / (boundingBox.maxX - boundingBox.minX);
	float intercept = boundingBox.minY - (slope * boundingBox.minX);

	for (int x = boundingBox.minX; x <= boundingBox.maxX; ++x) {
		float height = (slope * x) + intercept;
		//cout << "x: " << x << " - height: " << height << endl;
		for (int z = boundingBox.minZ; z <= boundingBox.maxZ; ++z) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < height) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = height;
			}
		}
	}
}

void World::updateHeightMapRamp270(AABB boundingBox) {
	float slope = (boundingBox.maxY - boundingBox.minY) / (boundingBox.minX - boundingBox.maxX);
	float intercept = boundingBox.minY - (slope * boundingBox.maxX);

	for (int x = boundingBox.maxX; x <= boundingBox.minX; ++x) {
		float height = (slope * x) + intercept;
		//cout << "x: " << x << " - height: " << height << endl;
		for (int z = boundingBox.maxZ; z <= boundingBox.minZ; ++z) {
			if (m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] < height) {
				m_heightMap[x + m_heightMapXShift][z + m_heightMapZShift] = height;
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

AABB* World::fixBoundingBox(AABB boundingBox) {
	float minX = boundingBox.minX;
	float minY = boundingBox.minY;
	float minZ = boundingBox.minZ;
	float maxX = boundingBox.maxX;
	float maxY = boundingBox.maxY;
	float maxZ = boundingBox.maxZ;
	float temp = 0.0f;

	if (minX > maxX) {
		temp = minX;
		minX = maxX;
		maxX = temp;
	}

	if (minY > maxY) {
		temp = minY;
		minY = maxY;
		maxY = temp;
	}

	if (minZ > maxZ) {
		temp = minZ;
		minZ = maxZ;
		maxZ = temp;
	}

	return new AABB(minX, minY, minZ, maxX, maxY, maxZ);
}