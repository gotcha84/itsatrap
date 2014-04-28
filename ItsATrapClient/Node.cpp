#include "Node.h"

namespace sg {

	Node::Node() {
		m_parent = nullptr;
		m_objectID = -1;
		m_name = "";
	}

	Node::Node(string name) {
		m_parent = nullptr;
		m_objectID = -1;
		m_name = name;
	}

	Node::~Node() {

	}

	void Node::setName(string name) {
		m_name = name;
	}

	string Node::getName() {
		return m_name;
	}

	bool Node::hasParent() {
		if (m_parent != nullptr) {
			return true;
		}
		return false;
	}

	Node* Node::getParent() {
		return m_parent;
	}

	void Node::setParent(Node *p) {
		m_parent = p;
	}

	void Node::removeParent() {
		m_parent = nullptr;
	}

	int Node::getObjectID() {
		return m_objectID;
	}

	void Node::setObjectID(int id) {
		m_objectID = id;
	}

	
	void Node::print() {
		cout << "(" << this->getObjectID() << " Node: " << this->getName() << ")";
	}
	
}