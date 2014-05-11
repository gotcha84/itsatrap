#pragma once
#ifndef BUILDING_H
#define BUILDING_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <climits>
#include <math.h>

#include "Geode.h"
#include "World.h"
#include "AABB.h"
#include "City.h"

using namespace std;

namespace sg {
	class Building : public Geode {
		public:
			AABB m_boundingBox;
			int m_id;

			Building(int id);
			~Building();

			void draw(glm::mat4 parent, glm::mat4 camera);
			void drawShape();
			void calculateBoundingBox();
			bool isInside(glm::vec3);
	};
}

#endif