#include "ClientInstance.h"

ClientInstance::ClientInstance() {
	m_xMouse = 0;
	m_yMouse = 0;

	m_xAngleChange = 0.0f;
	m_yAngleChange = 0.0f;

	m_xAngleChangeFactor = 20.0f;
	m_yAngleChangeFactor = 20.0f;

	x = 0;
	inc = 0.0;

	root = new sg::Player();
}

ClientInstance::~ClientInstance() {
	delete root;
	root = nullptr;
}
