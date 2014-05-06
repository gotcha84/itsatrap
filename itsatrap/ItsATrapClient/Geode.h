#pragma once
#ifndef GEODE_H
#define GEODE_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

#include "Node.h"
#include "AABB.h"
#include "Texture.h"
#include "Material.h"

namespace sg {

	class Geode : public Node {
		public:
			AABB m_boundingBox;
			Material m_material;

			glm::vec3 m_position;
			glm::mat4 m_model;
			glm::vec3 m_color;

			Texture *texture;
			GLuint texturePPM;
			char* m_textureFilename;

			void setPosition(glm::vec3);
			glm::vec3 getPosition();
			void setMatrix(glm::mat4);
			glm::mat4 getMatrix();
			void setColor(glm::vec3);
			glm::vec3 getColor();

			AABB getBoundingBox();
			void setBoundingBox(glm::vec3 pos, float rad);

			Geode();
			~Geode();

			void print();

			void draw(glm::mat4 parent, glm::mat4 cam) = 0;

			virtual void calculateBoundingBox();
			virtual bool isInside(glm::vec3 point);
			// virtual bool collidesWith(Geode b);
			virtual void setMaterial();
	};

}

#endif