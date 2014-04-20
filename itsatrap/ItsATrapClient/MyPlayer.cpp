#include "MyPlayer.h"

MyPlayer::MyPlayer() {
	m_cam = Camera();

	m_modelMatrix = glm::mat4();
	m_modelviewMatrix = glm::mat4();
	m_projectionMatrix = glm::mat4();
	m_viewportMatrix = glm::mat4();

	setProjectionMatrix();
	setViewportMatrix();

	m_xWalkFactor = 1.0f;
	m_zWalkFactor = 1.0f;

	//m_physics = Physics();
}

MyPlayer::~MyPlayer() {

}

Camera *MyPlayer::getCamera() {
	return &m_cam;
}

glm::mat4 MyPlayer::getModelMatrix() {
	return m_modelMatrix;
}

glm::mat4 MyPlayer::getCameraMatrix() {
	return m_cam.getCameraMatrix();
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
	
	cout << "center: " << glm::to_string(m_cam.m_cameraCenter) << endl;
	cout << "lookat: " << glm::to_string(m_cam.m_cameraLookAt) << endl;
	cout << "xx: " << glm::to_string(m_cam.m_camX) << endl;
	cout << "zz: " << glm::to_string(m_cam.m_camZ) << endl;
	cout << "zWalkFactor: " << m_zWalkFactor << endl;
	//cout << glm::to_string(m_cam.m_cameraMatrix) << endl;
	
	// calculate proposals
	// TODO change all cam centers to player pos
	glm::vec3 tmp_camZ = glm::vec3(m_cam.m_camZ.x, 0.0f, m_cam.m_camZ.z);
	switch (key) {
		case 'w':
			proposedNewPos = m_cam.m_cameraCenter + m_zWalkFactor*tmp_camZ;
			break;

		case 's':
			proposedNewPos = m_cam.m_cameraCenter + -1.0f*m_zWalkFactor*tmp_camZ;
			break;

		case 'd':
			proposedNewPos = m_cam.m_cameraCenter + -1.0f*m_xWalkFactor*m_cam.m_camX;
			break;

		case 'a':
			proposedNewPos = m_cam.m_cameraCenter + m_xWalkFactor*m_cam.m_camX;
			break;
	}

	// TODO collision detection

	// placeholder for:
	// glm::vec3 newPos = collisionDetection(proposedNewPos);

	
	glm::vec3 newPos = proposedNewPos;
	cout << "NEWPOS!!: " << glm::to_string(newPos) << endl;
	glm::vec3 moved = newPos - m_cam.m_cameraCenter;

	//m_physics.m_position = newPos;
	//m_physics.applyGravity();

	// TODO - remove this and uncomment below
	m_cam.m_cameraCenter = newPos;

	//moved = m_physics.m_position - m_cam.m_cameraCenter;
	//m_cam.m_cameraCenter = m_physics.m_position;

	m_cam.m_cameraLookAt+=moved;
	m_cam.updateCameraMatrix();

	// should i do it here or in displaycallback?
	//updateModelViewMatrix();
}

void MyPlayer::updateModelViewMatrix() {
	// TODO; use model matrix in this calculation

	m_modelviewMatrix = glm::inverse(m_cam.m_cameraMatrix) * m_modelMatrix;
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

	m_projectionMatrix = glm::mat4(1.0/(aspect), 0, 0, 0,
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
	m_cam.move(delta);
}

void MyPlayer::moveTo(glm::vec3 pos) {
	m_cam.moveTo(pos);
}

void MyPlayer::lookIn(glm::vec3 direction) {
	m_cam.lookIn(direction);
}

void MyPlayer::lookAt(glm::vec3 point) {
	m_cam.lookAt(point);
}