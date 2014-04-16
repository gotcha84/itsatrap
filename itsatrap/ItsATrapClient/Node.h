#pragma once
#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>

class Node {
	public:
		Node *m_parent;

		Node();
		~Node();

		bool hasParent();
		Node* getParent();
		void setParent(Node* p);
		void removeParent();

		virtual void draw(glm::mat4 parent) = 0;
};

#endif