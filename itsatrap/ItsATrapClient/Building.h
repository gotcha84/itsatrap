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
#include "Physics.h"

#include "Texture.h"
#include "Shader.h"
#include "glext.h"

using namespace std;

namespace sg {
	class Building : public Geode {
		public:
			int m_id;

			Physics m_physics;
			
			// textures
			Texture *texture;
			GLuint texturePPM;
			char* m_textureFilename;

			// shaders
			Shader *shader;
			GLuint light;

			Building(int id);
			~Building();

			void initArrays();
			void draw(glm::mat4 parent, glm::mat4 cam);
			void drawShape();

			void print();
			
			virtual void calculateBoundingBox();
			virtual bool isInside(glm::vec3 point);
			bool nearTop (glm::vec3 point);
			bool clearedTop(AABB* other);

			virtual glm::vec3 intersects(glm::vec3 from, glm::vec3 goTo);
			virtual float angleIntersection(glm::vec3 from, glm::vec3 goTo);
			virtual int reflectionIntersection(glm::vec3 from, glm::vec3 goTo);
			
			int collidesWith(AABB* b); // make virtual if child classes
			virtual void setMaterial();
	};
}

#endif