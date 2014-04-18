#include <math.h>
#include <sstream> // to convert float to string
#include <iomanip> // to round floats

#include "MyPlayer.h"

using namespace std;


MyPlayer::MyPlayer() {
	m_cam = Camera();
	m_physics = Physics();
	setProjectionMatrix();
	setViewportMatrix();
	//c.identity();
}

void MyPlayer::handleMovement(unsigned char key) {

	glm::vec3 proposedNewPos;
	cout << "center: " << glm::to_string(m_cam.m_cameraCenter) << endl;
	cout << "lookat: " << glm::to_string(m_cam.m_cameraLookAt) << endl;
	//cout << glm::to_string(m_cam.m_cameraMatrix) << endl;
	cout << endl;
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

		case 'a':
			proposedNewPos = m_cam.m_cameraCenter + -1.0f*m_xWalkFactor*m_cam.m_camX;
			break;		

		case 'd':
			proposedNewPos = m_cam.m_cameraCenter + m_xWalkFactor*m_cam.m_camX;
			break;		
	}

	// TODO collision detection

	// placeholder for:
	// glm::vec3 newPos = collisionDetection(proposedNewPos);
	glm::vec3 newPos = proposedNewPos;

	glm::vec3 moved = newPos - m_cam.m_cameraCenter;

	m_physics.m_position = newPos;

	m_physics.applyGravity();

	moved = m_physics.m_position - m_cam.m_cameraCenter;

	m_cam.m_cameraCenter = m_physics.m_position;

	m_cam.m_cameraLookAt+=moved;

	m_cam.makeCameraMatrix();

	// should i do it here or in displaycallback?
	//updateModelViewMatrix();
}

void MyPlayer::updateModelViewMatrix() {
	// TODO; use model matrix in this calculation

	m_modelviewMatrix = glm::inverse(m_cam.m_cameraMatrix);
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
