#include "MyPlayer.h"
#include "Client.h"

MyPlayer::MyPlayer() {
	m_cam = new Camera();

	m_modelMatrix = glm::mat4();
	m_modelviewMatrix = glm::mat4();
	m_projectionMatrix = glm::mat4();
	m_viewportMatrix = glm::mat4();

	setProjectionMatrix();
	setViewportMatrix();

	m_xWalkFactor = 2.0f;
	m_zWalkFactor = 2.0f;
	m_xSlowWalkFactor = 1.0f;
	m_zSlowWalkFactor = 1.0f;

	m_physics = new Physics();
}

MyPlayer::~MyPlayer() {
	delete m_cam;
	m_cam = nullptr;

	delete m_physics;
	m_physics = nullptr;
}

Camera *MyPlayer::getCamera() {
	return m_cam;
}

glm::vec3 MyPlayer::getPosition() {
	return getCamera()->getCameraCenter();
}

Physics *MyPlayer::getPhysics() {
	return m_physics;
}

glm::mat4 MyPlayer::getTransMatrix() {
	return m_transMatrix;
}

glm::mat4 MyPlayer::getModelMatrix() {
	return m_modelMatrix;
}

glm::mat4 MyPlayer::getCameraMatrix() {
	return m_cam->getCameraMatrix();
}

glm::mat4 MyPlayer::getModelViewMatrix() {
	return m_modelviewMatrix;
}

glm::mat4 MyPlayer::getProjectionMatrix() {
	return m_projectionMatrix;
}

glm::mat4 MyPlayer::getViewPortMatrix() {
	return m_viewportMatrix;
}

void MyPlayer::handleMovement(unsigned char key) {
	glm::vec3 proposedNewPos;
	
	// DEBUG STATEMENTS
	// cout << glm::to_string(m_cam->m_cameraMatrix) << endl;	
	// cout << "center: " << glm::to_string(m_cam->m_cameraCenter) << endl;
	// cout << "lookat: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	// cout << "xx: " << glm::to_string(m_cam->m_camX) << endl;
	// cout << "zz: " << glm::to_string(m_cam->m_camZ) << endl;
	// cout << "zWalkFactor: " << m_zWalkFactor << endl;
	// cout << "center: " << glm::to_string(m_cam->m_cameraCenter) << endl;
	// cout << "lookat: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	// cout << glm::to_string(m_cam->m_cameraMatrix) << endl;
	
	// calculate proposals
	// TODO change all cam centers to player pos
	glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);
	float xWalkFactor;
	float zWalkFactor;
	if (m_physics->m_currentState == PhysicsStates::Falling) {
		xWalkFactor = m_xSlowWalkFactor;
		zWalkFactor = m_zSlowWalkFactor;
	}
	else {
		xWalkFactor = m_xWalkFactor;
		zWalkFactor = m_zWalkFactor;
	}
	switch (key) {
		case 'w':
			proposedNewPos = m_physics->m_position + zWalkFactor*tmp_camZ;
			break;

		case 's':
			proposedNewPos = m_physics->m_position + -1.0f*zWalkFactor*tmp_camZ;
			break;

		case 'a':
			proposedNewPos = m_physics->m_position + -1.0f*xWalkFactor*m_cam->m_camX;
			break;		

		case 'd':
			proposedNewPos = m_physics->m_position + xWalkFactor*m_cam->m_camX;
			break;		


	}
	
	// collision detection
	glm::vec3 oldPos = m_physics->m_position;

	// placeholder for:

	//cout << "goTo: " << glm::to_string(proposedNewPos) << endl;
	glm::vec3 newPos = m_physics->handleCollisionDetection(proposedNewPos);
	
	//glm::vec3 newPos = proposedNewPos;

	//glm::vec3 moved = newPos - oldPos;
	//cout << "moved: " << glm::to_string(moved) << endl;
	//m_physics->m_velocity = moved;

	m_physics->m_position = newPos;

	//m_physics->applyGravity();

	glm::vec3 moved = m_physics->m_position - oldPos;

	// people are 4 feet tall apparently
	m_cam->m_cameraCenter = glm::vec3(m_physics->m_position.x, m_physics->m_position.y+4.0f, m_physics->m_position.z);

	m_cam->m_cameraLookAt+=moved;
	//m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
	m_cam->updateCameraMatrix();

	this->setModelMatrix(glm::translate(m_physics->m_position));
	Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
}

void MyPlayer::updateModelViewMatrix() {
	m_modelviewMatrix = glm::inverse(this->getCameraMatrix()) * this->getModelMatrix();
}

void MyPlayer::setTransMatrix(glm::mat4 m) {
	m_transMatrix = m;
}

void MyPlayer::setModelMatrix(glm::mat4 m) {
	m_modelMatrix = m;
}

void MyPlayer::setProjectionMatrix() {
	float fov = 3.141592654*90.0/180.0;
	
	float aspect;
	/*if (fullscreen) {
		aspect = 1366.0/768.0;
	}
	else { */
		aspect = 512.0/512.0;
	//}
	float nearv = 0.1;
	float farv = 10000.0;

	m_projectionMatrix = glm::mat4(
			1.0/(aspect), 0, 0, 0,
			0, 1.0, 0, 0,
			0, 0, (nearv+farv)/(nearv-farv), 2*nearv*farv/(nearv-farv),
			0, 0, -1, 0);

	m_projectionMatrix = glm::transpose(m_projectionMatrix);
}

void MyPlayer::setViewportMatrix() {
	// TODO (if needed) adapt for 125
	/*float x = Window::width;
	float y = Window::height;
	float x0 = 0;
	float y0 = 0;

	getViewportMatrix() = 
		Matrix4((x-x0)/2, 0, 0, (x+x0)/2,
				0, (y-y0)/2, 0, (y+y0)/2,
				0, 0, 0.5, 0.5,
				0, 0, 0, 1);*/
}

void MyPlayer::move(glm::vec3 delta) {
	m_cam->move(delta);

	this->setModelMatrix(glm::translate(this->getModelMatrix(), delta));
}

void MyPlayer::moveTo(glm::vec3 pos) {
	m_cam->moveTo(pos);

	this->setModelMatrix(glm::translate(pos));
}

void MyPlayer::lookIn(glm::vec3 direction) {
	m_cam->lookIn(direction);
}

void MyPlayer::lookAt(glm::vec3 point) {
	m_cam->lookAt(point);
}
