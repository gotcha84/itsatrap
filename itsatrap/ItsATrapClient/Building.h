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
#include "City.h"

using namespace std;

namespace sg {
	class Building : public Geode {
		public:
			int m_id;

			Building(int id);
			~Building();

			void initArrays();
			void draw(glm::mat4 parent, glm::mat4 cam);
			void drawShape();

			void print();
			
			virtual void calculateBoundingBox();
			virtual bool isInside(glm::vec3 point);
			bool collidesWith(Building* b); // make virtual if child classes
			virtual void setMaterial();
	};
}

#endif