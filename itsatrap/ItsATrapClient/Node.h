#pragma once
#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>

namespace sg {

	class Node {
		public:
			int m_objectID;
			Node *m_parent;

			Node();
			~Node();

			bool hasParent();
			Node* getParent();
			void setParent(Node* p);
			void removeParent();

			int getID();

			virtual void draw(glm::mat4 parent, glm::mat4 camera) = 0;
	};

}

#endif