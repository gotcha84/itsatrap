#pragma once
#ifndef GEODE_H
#define GEODE_H

#include "Node.h"
#include "Texture.h"

#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

class Geode : public sg::Node {
	public:
		glm::vec3 color;
		sg::Texture *texture;
		GLuint textuPpm;

		Geode();
		~Geode();
		
		void draw(glm::mat4 parent, glm::mat4 camera) = 0;
};

#endif