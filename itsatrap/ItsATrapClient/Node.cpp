#include "Node.h"

namespace sg {

	Node::Node() {
		m_parent = nullptr;
	}

	Node::~Node() {

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

}