#pragma once
#ifndef TRAP_H
#define TRAP_H

#include "Geode.h"
#include "AABB.h"

namespace sg {
	class Trap : public Geode {
		public:
			glm::vec3 m_position;
			glm::mat4 m_model;
			glm::vec3 m_color;
			AABB *m_boundingBox;

			Trap();
			Trap(glm::vec3 pos);
			~Trap();

			void setPosition(glm::vec3 pos);
			glm::vec3 getPosition();

			void setMatrix(glm::mat4 model);
			glm::mat4 getMatrix();

			void setColor(glm::vec3 color);
			glm::vec3 getColor();

			void setBoundingBox(AABB *box);
			AABB *getBoundingBox();
			
			void draw(glm::mat4 parent, glm::mat4 cam);

			void print();
	};
}

#endif