#include <math.h>
#include <sstream> // to convert float to string
#include <iomanip> // to round floats

#include "Camera.h"

using namespace std;

static bool read = false;

Camera::Camera() {

	m_xRotationAngle = 1.0f;
	m_yRotationAngle = 1.0f;
	m_camX = glm::vec3(1.0f, 0, 0);
	m_camZ = glm::vec3(0, 0, -1.0f);

	m_cameraCenter = glm::vec3(0, 0, 10.0f);
	m_cameraLookAt = glm::vec3(0, 0, 9.0f);
	m_cameraUp = glm::vec3(0, 1.0f, 0);
}


void Camera::handleXRotation(char direction) {
	// left
	if (direction == 'l') {
		m_camZ = glm::rotateY(m_camZ, -1.0f*m_xRotationAngle);
		m_camX = glm::rotateY(m_camX, -1.0f*m_xRotationAngle);

	}

	// right
	if (direction == 'r') {
		m_camZ = glm::rotateY(m_camZ, 1.0f*m_xRotationAngle);
		m_camX = glm::rotateY(m_camX, 1.0f*m_xRotationAngle);
	}

	cout << "m_camX: " << glm::to_string(m_camX) << endl; 
	cout << "m_camZ: " << glm::to_string(m_camZ) << endl << endl; 

	m_cameraLookAt = m_cameraCenter + m_camZ;
	makeCameraMatrix();
}

void Camera::handleYRotation(char direction) {
	// TODO modify upvector too?
	// up
	if (direction == 'u') {
		m_camZ = glm::rotateX(m_camZ, -1.0f*m_yRotationAngle);
	}

	// down
	if (direction == 'd') {
		m_camZ = glm::rotateX(m_camZ, m_yRotationAngle);
	}

	m_cameraLookAt = m_cameraCenter + m_camZ;
	makeCameraMatrix();
}

void Camera::calculateAxis() {
	glm::vec3 ZCameraDiff = m_cameraLookAt - m_cameraCenter;
	ZCameraDiff.y = 0.0f;
	
	// not sure if need to normalize
	//m_camZ = glm::normalize(ZCameraDiff);

	// or is it radians
	m_camX = glm::rotateY(m_camZ, 90.0f);
}

void Camera::makeCameraMatrix() {

	glm::vec3 zc = m_cameraCenter - m_cameraLookAt;
	zc = glm::normalize(zc);

	glm::vec3 xc = glm::cross(m_cameraUp, zc);
	xc = glm::normalize(xc);

	// i assume its already normalized
	glm::vec3 yc = glm::cross(zc, xc);
	yc = glm::normalize(yc);


	m_cameraMatrix = glm::mat4(xc.x, xc.y, xc.z, 0.0f, yc.x, yc.y, yc.z, 0.0f, zc.x, zc.y, zc.z, 0.0f, m_cameraCenter.x, m_cameraCenter.y, m_cameraCenter.z, 1.0f); 

}