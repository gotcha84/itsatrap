#pragma once
#ifndef OBJNODE_H
#define OBJNODE_H

#include "Geode.h"
#include "ObjModel.h"

#include <string>

namespace sg {
	class ObjNode : public Geode {
		public:
			ObjModel *m_model;

			ObjNode();
			ObjNode(std::string objFilename);
			ObjNode(std::string objFilename, std::string mtlFilename);
			~ObjNode();

			void loadModel(std::string objFilename);
			void loadModel(std::string objFilename, std::string mtlFilename);
			ObjModel *getModel();

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
	};
}

#endif
