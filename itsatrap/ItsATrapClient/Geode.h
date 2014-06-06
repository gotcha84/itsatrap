#pragma once
#ifndef GEODE_H
#define GEODE_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

#include "Node.h"
#include "../Common/AABB.h"
#include "Material.h"
#include "MatrixTransform.h"

namespace sg {

	class Geode : public Node {
		public:
			AABB m_boundingBox;
			vector<Material> m_materials;

			glm::vec3 m_position;
			glm::mat4 m_model;
			glm::vec4 m_color;

			bool m_drawBB;

			void setPosition(glm::vec3);
			glm::vec3 getPosition();
			
			void setMatrix(glm::mat4);
			glm::mat4 getMatrix();
			
			void setColor(glm::vec4 color);
			glm::vec4 getColor();

			AABB getBoundingBox();
			void setBoundingBox(glm::vec3 pos, float rad);
			void setBoundingBox(AABB boundingBox);

			void enableDrawBB();
			void disableDrawBB();

			Geode();
			~Geode();

			void print();

			void draw(glm::mat4 parent, glm::mat4 cam) = 0;

			glm::mat4 getWorldTransformMatrix();
			virtual void calculateBoundingBox();
			virtual void calculateBoundingBox(glm::mat4 model);
			
			virtual bool isInside(glm::vec3 point);
			virtual void setMaterial();

			bool shouldDraw();
	};

}

#endif