#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "Node.h"

#include <vector>

namespace sg {

	class Group : public Node {
		public:
			vector<Node*> m_child;
			int m_nChild;

			Group();
			~Group();

			bool hasChild();
			int getNumChildren();
			vector<Node*> getChildren();
			Node *getChild(int c);
			void addChild(Node *n);
			bool removeChild(int c);
			bool removeChild(Node *c);

			void print();

			virtual void draw(glm::mat4 parent, glm::mat4 cam) = 0;
	};

}

#endif