#include "Group.h"

namespace sg {

	Group::Group() {
		m_child.clear();
		m_nChild = 0;
	}

	Group::~Group() {

	}

	bool Group::hasChild() {
		return (m_nChild > 0) ? true : false;
	}

	int Group::getNumChildren() {
		return m_nChild;
	}

	void Group::addChild(Node *n) {
		m_child.push_back(n);
		m_nChild++;

		n->setParent(this);
	}

	// remove child at index
	bool Group::removeChild(int c) {
		for (int i=0; i<this->getNumChildren(); i++) {
			if (c == i) {
				m_child[i]->removeParent();
				m_child.erase(m_child.begin() + i);
				m_nChild--;
				return true;
			}
		}
		return false;
	}

	// remove child node
	bool Group::removeChild(Node *c) {
		for (int i=0; i<this->getNumChildren(); i++) {
			if (c == m_child[i]) {
				m_child[i]->removeParent();
				m_child.erase(m_child.begin() + i);
				m_nChild--;
				return true;
			}
		}
		return false;
	}

}