#include "ClientInstance.h"

ClientInstance::ClientInstance() {
	m_myPlayer = MyPlayer();

	m_xMouse = 0;
	m_yMouse = 0;

	m_xAngleChange = 0.0f;
	m_yAngleChange = 0.0f;

	m_xAngleChangeFactor = 10.0f;
	m_yAngleChangeFactor = 10.0f;

	x = 0;
	inc = 0.0;

	root = new MatrixTransform();
}

ClientInstance::~ClientInstance() {
	delete root;
	root = nullptr;
}
