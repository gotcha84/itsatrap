#pragma once
#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>

#include <iostream>
#include <string>

using namespace std;

namespace sg {

	class Node {
		public:
			string m_name;
			int m_objectID;
			Node *m_parent;

			Node();
			Node(string name);
			~Node();

			void setName(string name);
			string getName();

			bool hasParent();
			Node* getParent();
			void setParent(Node* p);
			void removeParent();

			int getObjectID();
			void setObjectID(int id);

			virtual void print();

			virtual void draw(glm::mat4 parent, glm::mat4 cam) = 0;
	};

}

#endif