#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "Node.h"

#include <vector>

class Group : public Node {
	public:
		std::vector<Node*> m_child;
		int m_nChild;

		Group();
		~Group();

		bool hasChild();
		int getNumChildren();
		void addChild(Node *n);
		bool removeChild(int c);
		bool removeChild(Node *c);

		virtual void draw(glm::mat4 parent) = 0;
};

#endif