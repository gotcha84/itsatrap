#pragma once
#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"

#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

class Geode : public Node {
	public:
		glm::mat4 m_trans;

		Geode();
		Geode(glm::mat4 m);
		~Geode();

		void setMatrix(glm::mat4 m);
		glm::mat4 getMatrix();
		
		void draw(glm::mat4 parent);

		virtual void render(glm::mat4 modelview) = 0;
};

#endif