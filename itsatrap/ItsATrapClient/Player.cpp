#include <math.h>
#include <sstream> // to convert float to string
#include <iomanip> // to round floats

#include "Player.h"

using namespace std;


Player::Player() {
	m_cam = Camera();
	//c.identity();
}

void Player::handleMovement(unsigned char key) {

	glm::vec3 proposedNewPos;
	cout << "center: " << glm::to_string(m_cam.m_cameraCenter) << endl;
	cout << "lookat: " << glm::to_string(m_cam.m_cameraLookAt) << endl;
	//cout << glm::to_string(m_cam.m_cameraMatrix) << endl;
	cout << endl;
	// calculate proposals
	// TODO change all cam centers to player pos
	switch (key) {
		case 'w':
			proposedNewPos = m_cam.m_cameraCenter + m_zWalkFactor*m_cam.m_camZ;
			break;

		case 's':
			proposedNewPos = m_cam.m_cameraCenter + -1.0f*m_zWalkFactor*m_cam.m_camZ;
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

	m_cam.m_cameraCenter = newPos;
	
	m_cam.m_cameraLookAt+=moved;

	m_cam.makeCameraMatrix();

	// should i do it here or in displaycallback?
	//updateModelViewMatrix();
}

void Player::updateModelViewMatrix() {
	// TODO; use model matrix in this calculation

	m_modelviewMatrix = m_cam.m_cameraMatrix;
}
