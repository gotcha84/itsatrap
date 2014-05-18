#pragma once
#ifndef MESHNODE_H
#define MESHNODE_H

#include "Geode.h"
#include "Mesh.h"

namespace sg {
	class MeshNode : public Geode {
		public:
			Mesh *m_model;

			MeshNode();
			MeshNode(std::string filename);
			~MeshNode();

			void loadModel(std::string filename);
			Mesh *loadModel();

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
	};
}

#endif
