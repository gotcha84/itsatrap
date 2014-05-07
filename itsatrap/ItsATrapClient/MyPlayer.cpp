#include "MyPlayer.h"
#include "Client.h"

MyPlayer::MyPlayer() {
	m_modelMatrix = glm::mat4();
	m_modelviewMatrix = glm::mat4();
	m_projectionMatrix = glm::mat4();
	m_viewportMatrix = glm::mat4();

	setProjectionMatrix();
	setViewportMatrix();

	m_xWalkFactor = 0.5f;
	m_zWalkFactor = 0.5f;
	m_xSlowWalkFactor = 0.5f;
	m_zSlowWalkFactor = 0.5f;
	m_wallJumpFactor = 0.5f;
	m_wallJumpTime = 1.0f;
	m_teleportFactor = 100.0f;
	m_slideFactor = 2.0f;

	m_cam = new Camera();
	m_physics = new Physics();
	m_boundingBox = new AABB(this->getPosition(), 10.0f);
}

MyPlayer::MyPlayer(glm::vec3 pos) {
	m_modelMatrix = glm::mat4();
	m_modelviewMatrix = glm::mat4();
	m_projectionMatrix = glm::mat4();
	m_viewportMatrix = glm::mat4();

	setProjectionMatrix();
	setViewportMatrix();

	m_xWalkFactor = 0.5f;
	m_zWalkFactor = 0.5f;
	m_xSlowWalkFactor = 0.5f;
	m_zSlowWalkFactor = 0.5f;
	m_wallJumpFactor = 0.5f;
	m_wallJumpTime = 1.0f;
	m_teleportFactor = 100.0f;
	m_slideFactor = 2.0f;

	m_cam = new Camera(pos);
	m_physics = new Physics(pos, 1.0f);
	m_boundingBox = new AABB(pos, 10.0f);
}

MyPlayer::~MyPlayer() {
	delete m_cam;
	m_cam = nullptr;

	delete m_physics;
	m_physics = nullptr;

	delete m_boundingBox;
	m_boundingBox = nullptr;
}

void MyPlayer::handleSliding() {
	
	clock_t end = clock();
	glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);
	
	if (m_physics->m_currentState == PhysicsStates::Sliding) {
		
		if (((float)(end - m_physics->m_lastSlid) / CLOCKS_PER_SEC) > m_physics->m_slideDelay) {
			cout << "ending SLIDE TIMES UP:" << ((float)(end - m_physics->m_lastSlid) / CLOCKS_PER_SEC) << endl;
			m_physics->m_currentState = PhysicsStates::None;
		}
		else {
			cout << "SLIDING IN PROGRESS " << endl;
			glm::vec3 proposedNewPos = m_physics->m_position + m_slideFactor*m_zWalkFactor*tmp_camZ;
			m_physics->m_velocityDiff = m_slideFactor*m_zWalkFactor*tmp_camZ;

			glm::vec3 oldPos = m_physics->m_position;
			glm::vec3 newPos = m_physics->handleCollisionDetection(proposedNewPos);

			if (oldPos == newPos) {
				m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
				cout << "ending SLIDE HIT SOMETHING" << endl;
				m_physics->m_currentState = PhysicsStates::None;
			}

			m_physics->m_velocity+=m_physics->m_velocityDiff;

			glm::vec3 moved = newPos - oldPos;

			// people are 4 feet tall apparently
			m_cam->m_cameraCenter+=moved; // glm::vec3(m_physics->m_position.x, m_physics->m_position.y, m_physics->m_position.z);
			//cout << "before: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	
			// anurag
			m_cam->m_cameraLookAt+=moved;

			//cout << "after: " << glm::to_string(m_cam->m_cameraLookAt) << endl << endl;
			//m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
			m_cam->updateCameraMatrix();

			this->setModelMatrix(glm::translate(newPos));
			Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
			this->updateBoundingBox();
		}

	}

	else if (m_physics->m_currentState == PhysicsStates::None && ((float)(end - m_physics->m_lastSlid) / CLOCKS_PER_SEC) > m_physics->m_slideDelay) {
		cout << "WEE STARTING TO SLIDE" << endl;
		m_physics->m_lastSlid = clock();

		m_physics->m_currentState = PhysicsStates::Sliding;

		glm::vec3 proposedNewPos = m_physics->m_position + m_slideFactor*m_zWalkFactor*tmp_camZ;
		m_physics->m_velocityDiff = m_slideFactor*m_zWalkFactor*tmp_camZ;

		glm::vec3 oldPos = m_physics->m_position;
		glm::vec3 newPos = m_physics->handleCollisionDetection(proposedNewPos);

		if (oldPos == newPos) {
			m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			cout << "ending SLIDE HIT SOMETHING" << endl;
			m_physics->m_currentState = PhysicsStates::None;
		}

		m_physics->m_velocity+=m_physics->m_velocityDiff;

		glm::vec3 moved = newPos - oldPos;

		// people are 4 feet tall apparently
		m_cam->m_cameraCenter+=moved; // glm::vec3(m_physics->m_position.x, m_physics->m_position.y, m_physics->m_position.z);
		//cout << "before: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	
		// anurag
		m_cam->m_cameraLookAt+=moved;

		//cout << "after: " << glm::to_string(m_cam->m_cameraLookAt) << endl << endl;
		//m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
		m_cam->updateCameraMatrix();

		this->setModelMatrix(glm::translate(newPos));
		Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
		this->updateBoundingBox();
	}
	

}


void MyPlayer::handleTeleport() {
	
	clock_t end = clock();
	if (((float)(end - m_physics->m_lastTeleported) / CLOCKS_PER_SEC) > m_physics->m_teleportDelay) {

		m_physics->m_lastTeleported = clock();

		glm::vec3 proposedNewPos;
		glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);

		proposedNewPos = m_physics->m_position + m_teleportFactor*tmp_camZ;
		m_physics->m_velocityDiff = m_teleportFactor*tmp_camZ;

		glm::vec3 oldPos = m_physics->m_position;
		glm::vec3 newPos = m_physics->handleCollisionDetection(proposedNewPos);

		if (oldPos == newPos) {
			m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		m_physics->m_velocity+=m_physics->m_velocityDiff;

		glm::vec3 moved = newPos - oldPos;

		// people are 4 feet tall apparently
		m_cam->m_cameraCenter+=moved; // glm::vec3(m_physics->m_position.x, m_physics->m_position.y/*+4.0f*/, m_physics->m_position.z);
		//cout << "before: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	
		// anurag
		m_cam->m_cameraLookAt+=moved;

		//cout << "after: " << glm::to_string(m_cam->m_cameraLookAt) << endl << endl;
		//m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
		m_cam->updateCameraMatrix();

		this->setModelMatrix(glm::translate(newPos));
		Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
		this->updateBoundingBox();
	}
}


void MyPlayer::handleMovement(unsigned char key) {

	//cout << "curr state: " << m_physics->m_currentState << endl;

	glm::vec3 proposedNewPos;
	
	glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);
	float xWalkFactor;
	float zWalkFactor;

	if (m_physics->m_currentState == PhysicsStates::Jumping || m_physics->m_currentState == PhysicsStates::Falling || m_physics->m_currentState == PhysicsStates::WallJumping) {
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
			m_physics->m_velocityDiff = zWalkFactor*tmp_camZ;
			break;

		case 's':
			proposedNewPos = m_physics->m_position + -1.0f*zWalkFactor*tmp_camZ;
			m_physics->m_velocityDiff = -1.0f*zWalkFactor*tmp_camZ;
			break;

		case 'a':
			proposedNewPos = m_physics->m_position + -1.0f*xWalkFactor*m_cam->m_camX;
			m_physics->m_velocityDiff = -1.0f*xWalkFactor*m_cam->m_camX;
			break;		

		case 'd':
			proposedNewPos = m_physics->m_position + xWalkFactor*m_cam->m_camX;
			m_physics->m_velocityDiff = xWalkFactor*m_cam->m_camX;
			break;
	}

	// TODO: use server clock?
	//clock_t end;
	
	glm::vec3 oldPos = m_physics->m_position;

	// USE THESE FOR COLLISION DETECTION ON
	glm::vec3 newPos = m_physics->handleCollisionDetection(proposedNewPos);

	glm::vec3 pointOfImpact = m_physics->handleIntersection(oldPos, proposedNewPos);

	//if (m_physics->m_currentState == PhysicsStates::WallJumping) {
	//	end = clock();
	//	cout << "seconds elapsed: " << double(end - m_physics->m_stateStart) / CLOCKS_PER_SEC << endl;
	//	cout << "trying to walk with vector: " << glm::to_string(proposedNewPos-oldPos);
	//}

	//if (m_physics->m_currentState == PhysicsStates::WallJumping && (double(end - m_physics->m_stateStart) / CLOCKS_PER_SEC) > 20.0f) {
	//		
	//		cout << "ending the jump " << endl;
	//		m_physics->m_velocityDiff = glm::rotate(m_physics->m_velocityDiff, 90.0f, m_cam->m_camX);
	//		
	//		m_cam->m_camX = m_cam->m_camXWallJump;
	//		m_cam->m_camZ = m_cam->m_camZWallJump;
	//		m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;
	//		
	//		m_physics->m_currentState = PhysicsStates::Falling;
	//}

	if (newPos == oldPos) {
		
		//cout << "amiatrest??: " << m_physics->atRest() << endl;

		
		if (m_physics->m_currentState == PhysicsStates::Jumping && !(m_physics->atRest())) {

			cout << "starting the jump " << endl;
			m_cam->m_cameraLookAtWallJump = m_cam->m_cameraCenter - m_cam->m_camZ;
			m_cam->m_camZWallJump = m_cam->m_camZ*-1.0f;
			m_cam->m_camXWallJump = m_cam->m_camX*-1.0f;
			m_physics->m_velocityDiffWallJump = m_physics->m_velocity*-1.0f;

			m_physics->m_stateStart = clock();

			/*m_physics->m_velocityDiff = glm::rotate(m_physics->m_velocityDiff, 90.0f, m_cam->m_camX);
			m_physics->m_velocityDiff.y += 2.0f;*/

			m_cam->m_camZ = glm::rotate(m_cam->m_camZ, 90.f, m_cam->m_camX);
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
			//cout << "after: " << glm::to_string(m_cameraLookAt) << endl << endl;
	
			m_cam->updateCameraMatrix();
			m_physics->m_currentState = PhysicsStates::WallJumping;

			m_physics->m_velocity-=m_physics->m_velocityDiff;
		}
	
		else {
			m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
	m_physics->m_velocity+=m_physics->m_velocityDiff;
	
	
	//cout << "seconds elapsed: " << double(end - m_physics->m_stateStart) / CLOCKS_PER_SEC << endl;

	
	//cout << "velocitydiff after movment: " << glm::to_string(m_physics->m_velocityDiff) << endl;
	
	// USE THIS FOR COLLISION DETECTION OFF
	//glm::vec3 newPos = proposedNewPos;

	// USED BEFORE
	//m_physics->m_position = newPos;

	//m_physics->applyGravity();

	glm::vec3 moved = newPos - oldPos;

	// people are 4 feet tall apparently
	m_cam->m_cameraCenter+=moved; // glm::vec3(m_physics->m_position.x, m_physics->m_position.y/*+4.0f*/, m_physics->m_position.z);
	//cout << "before: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	
	// anurag
	m_cam->m_cameraLookAt+=moved;

	//cout << "after: " << glm::to_string(m_cam->m_cameraLookAt) << endl << endl;
	//m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
	m_cam->updateCameraMatrix();

	this->setModelMatrix(glm::translate(newPos));
	Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
	this->updateBoundingBox();
	//this->getAABB()->print();

}

void MyPlayer::handleJump() {

	if(m_physics->m_currentState != PhysicsStates::Jumping && m_physics->m_currentState != PhysicsStates::WallJumping) {
		m_physics->m_velocity.y += m_physics->m_yJumpFactor;
		m_physics->m_currentState = PhysicsStates::Jumping; 
	}
}

void MyPlayer::applyWallJump() {
	//cout << "lookat: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	clock_t end = clock();
	if (((float)(end - m_physics->m_stateStart) / CLOCKS_PER_SEC) > m_wallJumpTime) {
		cout << "END WALL JUMPING " << endl;
		m_physics->m_currentState = PhysicsStates::Falling;
		m_cam->m_camX = m_cam->m_camXWallJump;
		m_cam->m_camZ = m_cam->m_camZWallJump;
		m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;
		m_physics->m_velocityDiff = m_physics->m_velocityDiffWallJump;
	}
	else {
		m_physics->m_velocityDiff = glm::vec3(0.0f, m_wallJumpFactor, 0.0f);
	}
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

Camera *MyPlayer::getCamera() {
	return m_cam;
}

glm::vec3 MyPlayer::getPosition() {
	return this->getPhysics()->m_position;
}

Physics *MyPlayer::getPhysics() {
	return m_physics;
}

AABB *MyPlayer::getAABB() {
	return m_boundingBox;
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

void MyPlayer::move(glm::vec3 delta) {
	m_cam->move(delta);
	m_physics->move(delta);

	this->updateBoundingBox();
	this->setModelMatrix(glm::translate(this->getModelMatrix(), delta));
}

void MyPlayer::moveTo(glm::vec3 pos) {
	m_cam->moveTo(pos);
	m_physics->moveTo(pos);

	this->updateBoundingBox();
	this->setModelMatrix(glm::translate(pos));
}

void MyPlayer::lookIn(glm::vec3 direction) {
	m_cam->lookIn(direction);
}

void MyPlayer::lookAt(glm::vec3 point) {
	m_cam->lookAt(point);
}

void MyPlayer::updateBoundingBox() {
	m_boundingBox->setAABB(this->getPosition(), 10.0f);
}

bool MyPlayer::collidesWith(MyPlayer *other) {
	return this->getAABB()->collidesWith(*other->getAABB());
}