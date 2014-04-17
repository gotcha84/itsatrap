#pragma once
#ifndef _GROUP_H_
#define _GROUP_H_

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