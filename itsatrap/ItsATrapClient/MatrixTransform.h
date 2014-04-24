#pragma once
#ifndef MATRIXTRANSFORM_H
#define MATRIXTRANSFORM_H

#include "Group.h"

namespace sg {

	class MatrixTransform : public Group {
		public:
			glm::fmat4 m_trans;

			MatrixTransform();
			MatrixTransform(glm::mat4 m);
			~MatrixTransform();

			void setMatrix(glm::mat4 m);
			glm::mat4 getMatrix();

			void print();

			virtual void draw(glm::mat4 parent, glm::mat4 cam);
	};

}

#endif