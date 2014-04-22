#pragma once
#ifndef GEODE_H
#define GEODE_H

#include "Node.h"

#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

class Geode : public Node {
	public:
		glm::vec3 color;

		Geode();
		~Geode();
		
		void draw(glm::mat4 parent) = 0;
};

#endif