#include "Camera.h"
#include "ClientInstance.h"

using namespace std;

extern ClientInstance *client;

Camera::Camera() {
	m_playerHeight = glm::vec3(0.0f, 4.0f, 0.0f);
	m_slidingHeight = glm::vec3(0.0f, 2.0f, 0.0f);

	m_xRotationAngle = 1.0f;
	m_yRotationAngle = 1.0f;
	m_camX = glm::vec3(1.0f, 0, 0);
	m_camZ = glm::vec3(0, 0, -1.0f);

	//m_cameraCenter = glm::vec3(75.0f, 4.0f, 0.0f);
	//m_cameraLookAt = glm::vec3(75.0f, 4.0f, -1.0f);
	m_playerCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cameraCenter = m_playerCenter;// + m_playerHeight;
	m_cameraLookAt = m_cameraCenter + glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraUp = glm::vec3(0, 1.0f, 0);

	m_camZSliding = glm::vec3(0.0f, 10.0f, 0.0f);

	m_yRotated = 0.0f;

	updateCameraMatrix();
}

Camera::Camera(glm::vec3 pos) {
	m_playerHeight = glm::vec3(0.0f, 4.0f, 0.0f);
	m_slidingHeight = glm::vec3(0.0f, 2.0f, 0.0f);

	m_xRotationAngle = 1.0f;
	m_yRotationAngle = 1.0f;
	m_camX = glm::vec3(1.0f, 0, 0);
	m_camZ = glm::vec3(0, 0, -1.0f);

	//m_cameraCenter = glm::vec3(75.0f, 4.0f, 0.0f);
	//m_cameraLookAt = glm::vec3(75.0f, 4.0f, -1.0f);
	//cout << "pos: " << glm::to_string(pos) << endl;
	m_playerCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cameraCenter = pos;// + m_playerHeight;
	m_cameraLookAt = m_cameraCenter + glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraUp = glm::vec3(0, 1.0f, 0);

	m_camZSliding = glm::vec3(0.0f, 10.0f, 0.0f);

	m_yRotated = 0.0f;

	updateCameraMatrix();
}

glm::mat4 Camera::getCameraMatrix() {
	return m_cameraMatrix;
}

glm::vec3 Camera::getCameraCenter() {
	return m_cameraCenter;
}

glm::vec3 Camera::getCameraLookAt() {
	return m_cameraLookAt;
}

glm::vec3 Camera::getCameraUp() {
	return m_cameraUp;
}

void Camera::handleXRotation(float magnitude) {

	glm::vec3 tmp_camZ = glm::vec3(m_camZ.x, 0.0f, m_camZ.z);

	tmp_camZ = glm::rotateY(tmp_camZ, magnitude*m_xRotationAngle);
	m_camX = glm::rotateY(m_camX, magnitude*m_xRotationAngle);

	m_camZ = glm::vec3(tmp_camZ.x, m_camZ.y, tmp_camZ.z);
	m_cameraLookAt = m_cameraCenter + m_camZ;

	updateCameraMatrix();
}

void Camera::handleYRotation(float magnitude) {
	// TODO modify upvector too for confuse ray
	if (!(m_yRotated > 80.0f && magnitude > 0) && !(m_yRotated < -80.0f && magnitude < 0)) {
		m_camZ.y+=magnitude*0.01f*m_yRotationAngle; // both this and the two lines below seem okay
		//m_camZ = glm::rotate(m_camZ, magnitude*m_yRotationAngle, m_camX);
		//m_yRotated+=magnitude*m_yRotationAngle;
	}
	//cout << "mcamX: " << glm::to_string(m_camX) << endl;
	//cout << "before: " << glm::to_string(m_cameraLookAt) << endl;
	m_cameraLookAt = m_cameraCenter + m_camZ;
	//cout << "after: " << glm::to_string(m_cameraLookAt) << endl << endl;
	
	updateCameraMatrix();
}

void Camera::calculateAxis() {
	glm::vec3 ZCameraDiff = m_cameraLookAt - m_cameraCenter;
	//ZCameraDiff.y = 0.0f;
	
	// not sure if need to normalize
	m_camZ = glm::normalize(ZCameraDiff);

	float oldmcamXY = m_camX.y;
	// or is it radians
	m_camX = glm::rotateY(m_camZ, -90.0f);
	m_camX = glm::vec3(m_camX.x, oldmcamXY, m_camX.z);
}

void Camera::updateCameraMatrix() {

	glm::vec3 zc = m_cameraCenter - m_cameraLookAt;
	zc = glm::normalize(zc);

	glm::vec3 xc = glm::cross(m_cameraUp, zc);
	xc = glm::normalize(xc);

	glm::vec3 yc = glm::cross(zc, xc);
	yc = glm::normalize(yc);

	m_cameraMatrix = glm::mat4(xc.x, xc.y, xc.z, 0.0f, yc.x, yc.y, yc.z, 0.0f, zc.x, zc.y, zc.z, 0.0f, m_cameraCenter.x, m_cameraCenter.y, m_cameraCenter.z, 1.0f); 
}

void Camera::move(glm::vec3 delta) {
	m_cameraCenter += delta;
	m_cameraLookAt += delta;
	calculateAxis();
	updateCameraMatrix();
}

void Camera::moveTo(glm::vec3 pos) {
	// cout << "b4center " << glm::to_string(client->root->getCamera()->getCameraCenter()) << endl;
	// cout << "b4look at " << glm::to_string(client->root->getCamera()->getCameraLookAt()) << endl;
	// cout << "b4up " << glm::to_string(client->root->getCamera()->getCameraUp()) << endl << endl;
	glm::vec3 oldCamCenter = m_cameraCenter;
	m_cameraCenter = glm::vec3(pos.x, pos.y, pos.z);
	m_cameraLookAt = m_cameraCenter + m_camZ;
	calculateAxis();
	updateCameraMatrix();
	// cout << "aftercenter " << glm::to_string(client->root->getCamera()->getCameraCenter()) << endl;
	// cout << "afterlook at " << glm::to_string(client->root->getCamera()->getCameraLookAt()) << endl;
	// cout << "afterup " << glm::to_string(client->root->getCamera()->getCameraUp()) << endl << endl;
}

void Camera::lookIn(glm::vec3 direction) {
	// cout << "b4center " << glm::to_string(client->root->getCamera()->getCameraCenter()) << endl;
	// cout << "b4look at " << glm::to_string(client->root->getCamera()->getCameraLookAt()) << endl;
	// cout << "b4up " << glm::to_string(client->root->getCamera()->getCameraUp()) << endl << endl;
	m_cameraLookAt = m_cameraCenter + glm::normalize(direction);
	calculateAxis();
	updateCameraMatrix();
	// cout << "aftercenter " << glm::to_string(client->root->getCamera()->getCameraCenter()) << endl;
	// cout << "afterlook at " << glm::to_string(client->root->getCamera()->getCameraLookAt()) << endl;
	// cout << "afterup " << glm::to_string(client->root->getCamera()->getCameraUp()) << endl << endl;
}

void Camera::lookAt(glm::vec3 point) {
	m_cameraLookAt = m_cameraCenter + glm::normalize(point - m_cameraCenter);
	calculateAxis();
	updateCameraMatrix();
}