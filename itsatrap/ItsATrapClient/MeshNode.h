#pragma once
#ifndef MESHNODE_H
#define MESHNODE_H

#include "Geode.h"
#include "Mesh.h"

namespace sg {
	class MeshNode : public Geode {
		public:
			Mesh *m_mesh;

			MeshNode();
			MeshNode(std::string filename);
			~MeshNode();

			Mesh *getMesh();
			void loadMesh(std::string filename);

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
	};
}

#endif
