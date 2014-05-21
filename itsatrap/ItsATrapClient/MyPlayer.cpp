#include "MyPlayer.h"
#define BBOX_RAD 5.0f

MyPlayer::MyPlayer() {
	m_cam = new Camera();
	//m_physics = new Physics();
	m_boundingBox = new AABB(this->getPosition(), BBOX_RAD);

	initCommon();
}

MyPlayer::MyPlayer(glm::vec3 pos) {
	m_cam = new Camera(pos);
	//m_physics = new Physics(pos, 1.0f);
	m_boundingBox = new AABB(pos, BBOX_RAD);

	initCommon();
}

MyPlayer::~MyPlayer() {
	delete m_cam;
	m_cam = nullptr;

	/*delete m_physics;
	m_physics = nullptr;*/

	delete m_boundingBox;
	m_boundingBox = nullptr;
}

void MyPlayer::initCommon() {

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
	m_wallJumpFactor = 0.25f;
	m_pullingUpFactor = 0.125f;
	m_wallJumpTime = 1.0f;
	m_holdingEdgeTime = 3.0f;
	m_teleportFactor = 100.0f;
	m_slideFactor = 2.0f;
	m_bounceFactor = 1.0f;

	m_miniJumpYVelocityThreshold = -1.0f;

	m_numDeaths = 0;
	m_numKills = 0;
	m_health = 100;
	m_deathState = false;
	m_timeUntilRespawn = 0;
	m_resources = 0;

	m_wallJumpingBuildingId = -1;
	m_onTopOfBuildingId = -1;

}

void MyPlayer::setAABB(AABB *bbox) {
	m_boundingBox = bbox;
}


void MyPlayer::handleSliding() {
/*	
	AABB* oldAABB = this->getAABB();

	clock_t end = clock();
	glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);
	
	if (m_physics->m_currentState == PhysicsStates::Sliding) {
		
		if (((float)(end - m_physics->m_lastSlid) / CLOCKS_PER_SEC) > m_physics->m_slideDelay) {
			cout << "ending SLIDE TIMES FORWARD:" << ((float)(end - m_physics->m_lastSlid) / CLOCKS_PER_SEC) << endl;
			m_physics->m_currentState = PhysicsStates::None;
		}
		else {
			cout << "SLIDING IN PROGRESS " << endl;
			glm::vec3 proposedNewPos = m_physics->m_position + m_slideFactor*m_zWalkFactor*tmp_camZ;
			m_physics->m_velocityDiff = m_slideFactor*m_zWalkFactor*tmp_camZ;

			glm::vec3 oldPos = m_physics->m_position;
			glm::vec3 newPos;

			m_physics->m_position = proposedNewPos;
			this->updateBoundingBox();
			int buildingId = m_physics->handleCollisionDetection(this->getAABB());

			m_physics->m_position = oldPos;

			if (buildingId != -1) {
				newPos = oldPos;
				m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
				cout << "ending SLIDE HIT SOMETHING" << endl;
				m_physics->m_currentState = PhysicsStates::None;
			}
			else {
				newPos = proposedNewPos;
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
			//Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
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
		glm::vec3 newPos;
		m_physics->m_position = proposedNewPos;
		this->updateBoundingBox();
		int buildingId = m_physics->handleCollisionDetection(this->getAABB());
		m_physics->m_position = oldPos;

		if (buildingId != -1) {
			newPos = oldPos;
			m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			cout << "ending SLIDE HIT SOMETHING" << endl;
			m_physics->m_currentState = PhysicsStates::None;
		}
		else {
			newPos = proposedNewPos;
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
		//Client::sendStateUpdate(Client::getPlayerId(), newPos.x, newPos.y, newPos.z);
		this->updateBoundingBox();
	}
*/	

}

void MyPlayer::handleTeleport() {
	
	/*
	clock_t end = clock();
	if (((float)(end - m_physics->m_lastTeleported) / CLOCKS_PER_SEC) > m_physics->m_teleportDelay) {

		m_physics->m_lastTeleported = clock();

		glm::vec3 proposedNewPos;
		glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);

		proposedNewPos = m_physics->m_position + m_teleportFactor*tmp_camZ;
		m_physics->m_velocityDiff = m_teleportFactor*tmp_camZ;

		glm::vec3 oldPos = m_physics->m_position;
		glm::vec3 newPos;
		m_physics->m_position = proposedNewPos;
		this->updateBoundingBox();
		int buildingId = m_physics->handleCollisionDetection(this->getAABB());
		m_physics->m_position = oldPos;

		if (buildingId != -1) {
			newPos = oldPos;
			m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			newPos = proposedNewPos;
		}

		m_physics->m_velocity+=m_physics->m_velocityDiff;

		glm::vec3 moved = newPos - oldPos;

		// people are 4 feet tall apparently
		m_cam->m_cameraCenter+=moved; // 
		//cout << "before: " << glm::to_string(m_cam->m_cameraLookAt) << endl;
	
		// anurag
		m_cam->m_cameraLookAt+=moved;

		//cout << "after: " << glm::to_string(m_cam->m_cameraLookAt) << endl << endl;
		//m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
		m_cam->updateCameraMatrix();

		this->setModelMatrix(glm::translate(newPos));
		//Client::sendPlayerUpdate(client->root->getPlayerObjectForNetworking());
		this->updateBoundingBox();
	}
*/
}

void MyPlayer::handleXRotation(float magnitude) {
	this->getCamera()->handleXRotation(magnitude);
}

void MyPlayer::handleYRotation(float magnitude) {
	this->getCamera()->handleYRotation(magnitude);
}

// TODO: imp if needed
//void MyPlayer::Unstuck() {
//
//}

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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
	//glTranslatef(0, 0, -20);
	glMatrixMode(GL_MODELVIEW);
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
	return this->m_position;
}

//Physics *MyPlayer::getPhysics() {
//	return m_physics;
//}

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
	//m_physics->move(delta);
	m_position += delta;
	this->updateBoundingBox();
	this->setModelMatrix(glm::translate(this->getModelMatrix(), delta));
}

void MyPlayer::moveTo(glm::vec3 pos) {
	m_cam->moveTo(pos);
	//m_physics->moveTo(pos);
	m_position = pos;
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
	m_boundingBox->setAABB(this->getPosition(), BBOX_RAD);
}

bool MyPlayer::collidesWith(MyPlayer *other) {
	return this->getAABB()->collidesWith(*other->getAABB());
}

//bool MyPlayer::knifeHitWith(MyPlayer *other)
//{
//	glm::vec3 difVec = this->getCamera()->getCameraLookAt() - this->getCamera()->getCameraCenter();
//	glm::vec3 target = this->getCamera()->getCameraCenter() + (KNIFE_RANGE * difVec);
//
//	return other->getAABB()->inside(target);
//}

int MyPlayer::getHealth() {
	return m_health;
}

void MyPlayer::setHealth(int health) {
	m_health = health;
}

void MyPlayer::damage(int dmg) {
	m_health -= dmg;
}

bool MyPlayer::isDead() {
	return m_health <= 0;
}

bool MyPlayer::isAlive() {
	return m_health > 0;
}