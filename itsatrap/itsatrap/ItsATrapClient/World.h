#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

class World{
	public:
		static int m_heightMapXShift;
		static int m_heightMapZShift;

		static float m_heightMap[1019][787];

		void initializeHeightMap();
};

#endif