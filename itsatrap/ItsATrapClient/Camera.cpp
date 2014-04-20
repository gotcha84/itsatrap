#include "Camera.h"

Camera::Camera() {

	m_xRotationAngle = 1.0f;
	m_yRotationAngle = 1.0f;
	m_camX = glm::vec3(1.0f, 0, 0);
	m_camZ = glm::vec3(0, 0, -1.0f);

	//m_cameraCenter = glm::vec3(0, 0, 10.0f);
	//m_cameraLookAt = glm::vec3(0, 0, 9.0f);
	m_cameraCenter = glm::vec3(0, 0, 0);
	m_cameraLookAt = glm::vec3(0, 0, -1.0f);
	m_cameraUp = glm::vec3(0, 1.0f, 0);

	updateCameraMatrix();
}

glm::mat4 Camera::getCameraMatrix() {
	return m_cameraMatrix;
}

void Camera::handleXRotation(char direction) {
	glm::vec3 tmp_camZ = glm::vec3(m_camZ.x, 0.0f, m_camZ.z);

	// left
	if (direction == 'r') {
		tmp_camZ = glm::rotateY(tmp_camZ, -1.0f*m_xRotationAngle);
		m_camX = glm::rotateY(m_camX, -1.0f*m_xRotationAngle);

	}

	// right
	if (direction == 'l') {
		tmp_camZ = glm::rotateY(tmp_camZ, 1.0f*m_xRotationAngle);
		m_camX = glm::rotateY(m_camX, 1.0f*m_xRotationAngle);
	}
	
	m_camZ = glm::vec3(tmp_camZ.x, m_camZ.y, tmp_camZ.z);
	m_cameraLookAt = m_cameraCenter + m_camZ;

	updateCameraMatrix();
}

void Camera::handleYRotation(char direction) {
	// TODO modify upvector too for confuse ray
	// up
	if (direction == 'u') {
		m_camZ = glm::rotateX(m_camZ, 1.0f*m_yRotationAngle);
		//m_cameraUp = glm::rotateX(m_cameraUp, 1.0f*m_yRotationAngle);
	}

	// down
	if (direction == 'd') {
		m_camZ = glm::rotateX(m_camZ, -1.0f*m_yRotationAngle);
		//m_cameraUp = glm::rotateX(m_cameraUp, -1.0f*m_yRotationAngle);
	}

	m_cameraLookAt = m_cameraCenter + m_camZ;
	
	updateCameraMatrix();
}

void Camera::calculateAxis() {
	glm::vec3 ZCameraDiff = m_cameraLookAt - m_cameraCenter;
	//ZCameraDiff.y = 0.0f;
	
	// not sure if need to normalize
	m_camZ = glm::normalize(ZCameraDiff);

	// or is it radians
	m_camX = glm::rotateY(m_camZ, -90.0f);
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
	m_cameraCenter = pos;
	m_cameraLookAt = glm::normalize(m_cameraLookAt - m_cameraCenter);
	calculateAxis();
	updateCameraMatrix();
}

void Camera::lookIn(glm::vec3 direction) {
	m_cameraLookAt = m_cameraCenter + direction;
	calculateAxis();
	updateCameraMatrix();
}

void Camera::lookAt(glm::vec3 point) {
	m_cameraLookAt = glm::normalize(point - m_cameraCenter);
	calculateAxis();
	updateCameraMatrix();
}