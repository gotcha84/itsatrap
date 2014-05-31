#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "ObjNode.h"

namespace sg {
	class Skybox : public ObjNode {
		public:
			Skybox();
			Skybox(std::string objFilename, std::string mtlFilename);
			~Skybox();

			void draw(glm::mat4 parent, glm::mat4 cam);
			void drawModel();

			void print();
	};
}

#endif