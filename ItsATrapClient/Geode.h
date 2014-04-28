#pragma once
#ifndef GEODE_H
#define GEODE_H

#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

#include "Node.h"
#include "AABB.h"
#include "Texture.h"

namespace sg {

	class Geode : public Node {
		public:
			glm::vec3 color;
			AABB m_boundingBox;
			
			Texture *texture;
			GLuint texturePPM;

			Geode();
			~Geode();

			void print();

			void draw(glm::mat4 parent, glm::mat4 cam) = 0;

			virtual void calculateBoundingBox();
			virtual bool isInside(glm::vec3 point);
	};

}

#endif