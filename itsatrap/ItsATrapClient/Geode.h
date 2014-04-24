#pragma once
#ifndef GEODE_H
#define GEODE_H

#include "Node.h"

#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

namespace sg {

	class Geode : public Node {
		public:
			glm::vec3 color;

			Geode();
			~Geode();

			void print();

			void draw(glm::mat4 parent, glm::mat4 cam) = 0;
	};

}

#endif