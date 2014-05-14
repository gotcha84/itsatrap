#include "MyPlayer.h"
//#include "Client.h"

#define BBOX_RAD 5.0f

MyPlayer::MyPlayer() {
	m_cam = new Camera();
	m_physics = new Physics();
	m_boundingBox = new AABB(this->getPosition(), BBOX_RAD);

	initCommon();
}

MyPlayer::MyPlayer(glm::vec3 pos) {
	m_cam = new Camera(pos);
	m_physics = new Physics(pos, 1.0f);
	m_boundingBox = new AABB(pos, BBOX_RAD);

	initCommon();
}

MyPlayer::~MyPlayer() {
	delete m_cam;
	m_cam = nullptr;

	delete m_physics;
	m_physics = nullptr;

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
			cout << "ending SLIDE TIMES UP:" << ((float)(end - m_physics->m_lastSlid) / CLOCKS_PER_SEC) << endl;
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
			int canMove = m_physics->handleCollisionDetection(this->getAABB());

			m_physics->m_position = oldPos;

			if (canMove != -1) {
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
		int canMove = m_physics->handleCollisionDetection(this->getAABB());
		m_physics->m_position = oldPos;

		if (canMove != -1) {
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
		int canMove = m_physics->handleCollisionDetection(this->getAABB());
		m_physics->m_position = oldPos;

		if (canMove != -1) {
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

void MyPlayer::handleMovement(unsigned char key) {

	if (this->getPhysics()->m_currentState == PhysicsStates::Climbing || 
		this->getPhysics()->m_currentState == PhysicsStates::Sliding ||
		this->getPhysics()->m_currentState == PhysicsStates::PullingUp) {
		return;
	}

	glm::vec3 proposedNewPos;
	
	glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);
	float xWalkFactor;
	float zWalkFactor;

	float speedMultiplier = 1.0;
	if (m_slowDuration > 0)
	{
		float slowFactor = 0;
		ConfigSettings::getConfig()->getValue("SlowFactor", slowFactor);
		speedMultiplier = slowFactor;
	}

	if (m_physics->m_currentState == PhysicsStates::Jumping || m_physics->m_currentState == PhysicsStates::Falling || m_physics->m_currentState == PhysicsStates::Climbing) {
		xWalkFactor = m_xSlowWalkFactor * speedMultiplier;
		zWalkFactor = m_zSlowWalkFactor * speedMultiplier;
	}
	else {
		xWalkFactor = m_xWalkFactor * speedMultiplier;
		zWalkFactor = m_zWalkFactor * speedMultiplier;
	}
	glm::vec3 toAdd;
	switch (key) {
		case 'w':
			proposedNewPos = m_physics->m_position + zWalkFactor*tmp_camZ;
			toAdd = zWalkFactor*tmp_camZ;
			break;

		case 's':
			proposedNewPos = m_physics->m_position + -1.0f*zWalkFactor*tmp_camZ;
			toAdd = -1.0f*zWalkFactor*tmp_camZ;
			break;

		case 'a':
			proposedNewPos = m_physics->m_position + -1.0f*xWalkFactor*m_cam->m_camX;
			toAdd = -1.0f*xWalkFactor*m_cam->m_camX;
			break;		

		case 'd':
			proposedNewPos = m_physics->m_position + xWalkFactor*m_cam->m_camX;
			toAdd = xWalkFactor*m_cam->m_camX;
			break;
	}
	// TODO: use server clock?
	//clock_t end;
	
	glm::vec3 oldPos = m_physics->m_position;
	glm::vec3 newPos;

	m_physics->m_position = proposedNewPos;
	this->updateBoundingBox();
	int canMove = m_physics->handleCollisionDetection(this->getAABB());
	m_physics->m_position = oldPos;

	if (canMove != -1) {
		int oldOnTopOfBuildingId = m_onTopOfBuildingId;
		m_onTopOfBuildingId = -1;
		// TODO: check guy is facing wall too
		if (m_physics->m_currentState == PhysicsStates::Jumping && !(m_physics->atRest())) {
			float angle = m_physics->handleAngleIntersection(oldPos, proposedNewPos, this->getAABB(), canMove);
			if (abs(90.0f-angle) < 22.5f && m_physics->m_velocity.y >= m_miniJumpYVelocityThreshold) {
				newPos = oldPos;
				cout << "starting the climb with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << canMove << endl;
				//m_cam->m_cameraLookAtWallJump = m_cam->m_cameraCenter - m_cam->m_camZ;
				
				m_cam->m_camZWallJump = m_cam->m_camZ;
				m_cam->m_camXWallJump = m_cam->m_camX;
				m_physics->m_velocityDiffWallJump = (m_physics->m_velocityDiff+toAdd) /* *-1.0f*/;

				m_physics->m_stateStart = clock();		
	
				m_physics->m_wallJumpLookedDown = false;
				m_physics->m_wallJumpLookedRight = false;

				// technically dont need below commented lines as they will be executed regardless
				/*m_cam->m_camZ = glm::rotate(m_cam->m_camZ, m_physics->m_wallJumpLookUpIncrement, m_cam->m_camX);
				m_cam->m_yRotated+=m_physics->m_wallJumpLookUpIncrement;
				m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;*/
			
				m_physics->m_wallJumpLookXHolder = m_cam->m_xRotated;

				//float newXRotated = m_cam->getXRotated() + 180.0f;
				//this->getCamera()->setXRotated(newXRotated);
				
				//toAdd*=-1.0f;
			
				//m_cam->updateCameraMatrix();
				m_physics->m_currentState = PhysicsStates::Climbing;
				m_wallJumpingBuildingId = canMove;
			}
			else {
				if (oldOnTopOfBuildingId != canMove) {
				//if (m_onTopOfBuildingId != -1) {
					cout << "starting the minijump with angle: " << abs(90.0f-angle) << ", and y velo: " << m_physics->m_velocity.y << ", on building: " << canMove << endl;
					// 0,1 = x, -1 = y, 4,5 = z

				
					int newDirection = m_physics->handleReflectionIntersection(oldPos, proposedNewPos, this->getAABB(), canMove);
					if (newDirection == 0 || newDirection == 1) {
						m_cam->m_camZ.x*=-1.0f;
						m_physics->m_velocityDiff.x*=-1.0f;
						toAdd.x*=-1.0f;
					
					}
					if (newDirection == 4 || newDirection == 5) {
						m_cam->m_camZ.z*=-1.0f;
						m_physics->m_velocityDiff.z*=-1.0f;
						toAdd.z*=-1.0f;
					}
					/*	1 +,-
					2 -,+
					3 -,+
					4 +,-*/
					if (angle < 90.0f) {
						if (newDirection == 0 || newDirection == 5) {
							m_cam->m_xRotated-=(2.0f*angle);
						}
						if (newDirection == 1 || newDirection == 4) {
							m_cam->m_xRotated-=(2.0f*(180.0f-angle));
						}
					}
					if (angle > 90.0f) {
						if (newDirection == 0 || newDirection == 5) {
							m_cam->m_xRotated+=(2.0f*(180.0f-angle));
						}
						if (newDirection == 1 || newDirection == 4) {
							m_cam->m_xRotated+=(2.0f*angle);
						}
					}
	
					m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
					m_cam->calculateAxis();
					m_cam->updateCameraMatrix();
					m_physics->m_currentState = PhysicsStates::WallJumping;
				}
				// TODO: need else?
				/*else {
				}*/
			}
		}
	
		else {
			toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
			m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
			m_physics->m_velocity = glm::vec3(0.0f, m_physics->m_velocity.y, 0.0f);
		}
	}
	else {
		newPos = proposedNewPos;
	}
	m_physics->m_velocityDiff+=toAdd;

	if (m_physics->m_currentState == PhysicsStates::WallJumping) {
		m_physics->m_velocityDiff*=m_bounceFactor;
	}
	if (m_physics->m_currentState == PhysicsStates::Climbing) {
		m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	}
		
	// USE THIS FOR COLLISION DETECTION OFF
	//glm::vec3 newPos = proposedNewPos;

	//glm::vec3 moved = newPos - oldPos;
	//m_cam->m_cameraCenter+=moved;
	//m_cam->m_cameraLookAt+=moved;

	//m_cam->updateCameraMatrix();
	//this->updateBoundingBox();
}

void MyPlayer::handleJump() {
	if(m_physics->m_currentState != PhysicsStates::Jumping && 
		m_physics->m_currentState != PhysicsStates::Climbing) {
		
		m_physics->m_velocity.y += m_physics->m_yJumpFactor;
		m_physics->m_currentState = PhysicsStates::Jumping;
	}
}

void MyPlayer::applyClimbing() {
	clock_t end = clock();
	if (((float)(end - m_physics->m_stateStart) / CLOCKS_PER_SEC) > m_wallJumpTime) {
		cout << "END CLIMB " << endl;
		m_physics->m_currentState = PhysicsStates::Falling;
		m_cam->m_camX = m_cam->m_camXWallJump;
		m_cam->m_camZ = m_cam->m_camZWallJump;
		m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;
		//m_physics->m_velocityDiff = m_physics->m_velocityDiffWallJump;
		m_wallJumpingBuildingId = -1;
	}
	else {
		m_physics->m_velocityDiff = glm::vec3(0.0f, m_wallJumpFactor, 0.0f);
		m_physics->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

		if (m_cam->m_yRotated < m_physics->m_wallJumpLookUp) {
			m_cam->m_camZ = glm::rotate(m_cam->m_camZ, m_physics->m_wallJumpLookUpIncrement, m_cam->m_camX);
			m_cam->m_yRotated+=m_physics->m_wallJumpLookUpIncrement;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
		}

		if (m_physics->handleNearTop(m_physics->m_position, m_wallJumpingBuildingId)) {
			cout << "started holding edge" << endl; 
			m_physics->m_stateStart = clock();
			m_physics->m_currentState = PhysicsStates::HoldingEdge;
			/*m_cam->m_camX = -1.0f*m_cam->m_camXWallJump;
			m_cam->m_camZ = -1.0f*m_cam->m_camZWallJump;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;*/
		}
	}
	m_cam->updateCameraMatrix();
}

void MyPlayer::handleHoldingEdge(unsigned char key) {
	glm::vec3 proposedNewPos;
	
	glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);
	float xWalkFactor;
	float zWalkFactor;

	float speedMultiplier = 1.0;
	if (m_slowDuration > 0)
	{
		float slowFactor = 0;
		ConfigSettings::getConfig()->getValue("SlowFactor", slowFactor);
		speedMultiplier = slowFactor;
	}

	if (m_physics->m_currentState == PhysicsStates::Jumping || m_physics->m_currentState == PhysicsStates::Falling || m_physics->m_currentState == PhysicsStates::Climbing) {
		xWalkFactor = m_xSlowWalkFactor * speedMultiplier;
		zWalkFactor = m_zSlowWalkFactor * speedMultiplier;
	}
	else {
		xWalkFactor = m_xWalkFactor * speedMultiplier;
		zWalkFactor = m_zWalkFactor * speedMultiplier;
	}
	glm::vec3 toAdd;
	switch (key) {
		case 'w':
			cout << "started pulling up" << endl;
			m_physics->m_currentState = PhysicsStates::PullingUp;
			return;
			break;

		case 's':
			cout << "decided to fall back down" << endl;
			m_physics->m_currentState = PhysicsStates::Falling;
			m_cam->m_camX = m_cam->m_camXWallJump;
			m_cam->m_camZ = m_cam->m_camZWallJump;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;
			//m_physics->m_velocityDiff = m_physics->m_velocityDiffWallJump;
			m_wallJumpingBuildingId = -1;
			break;

		case 'a':
			proposedNewPos = m_physics->m_position + -1.0f*xWalkFactor*m_cam->m_camX;
			toAdd = -1.0f*xWalkFactor*m_cam->m_camX;
			break;		

		case 'd':
			proposedNewPos = m_physics->m_position + xWalkFactor*m_cam->m_camX;
			toAdd = xWalkFactor*m_cam->m_camX;
			break;

		case ' ':
			/*cout << "started pulling up" << endl;
			m_physics->m_currentState = PhysicsStates::PullingUp;*/
			return;
			break;

	}
	// TODO: use server clock?
	//clock_t end;
	
	glm::vec3 oldPos = m_physics->m_position;
	glm::vec3 newPos;

	m_physics->m_position = proposedNewPos;
	this->updateBoundingBox();
	int canMove = m_physics->handleCollisionDetection(this->getAABB());
	m_physics->m_position = oldPos;

	if (canMove != -1) {
		int oldOnTopOfBuildingId = m_onTopOfBuildingId;
		m_onTopOfBuildingId = -1;

		toAdd = glm::vec3(0.0f, 0.0f, 0.0f);
		m_physics->m_velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
		m_physics->m_velocity = glm::vec3(0.0f, m_physics->m_velocity.y, 0.0f);
	}
	else {
		newPos = proposedNewPos;
	}
	m_physics->m_velocityDiff+=toAdd;

}

void MyPlayer::applyHoldingEdge() {
	clock_t end = clock();
	if (((float)(end - m_physics->m_stateStart) / CLOCKS_PER_SEC) > m_holdingEdgeTime) {
		cout << "END HOLD " << endl;
		m_physics->m_currentState = PhysicsStates::Falling;
		m_cam->m_camX = m_cam->m_camXWallJump;
		m_cam->m_camZ = m_cam->m_camZWallJump;
		m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;
		//m_physics->m_velocityDiff = m_physics->m_velocityDiffWallJump;
		m_wallJumpingBuildingId = -1;
	}

}

void MyPlayer::applyPullingUp() {

	if (m_physics->handleClearedTop(m_boundingBox, m_wallJumpingBuildingId)) {
		cout << "CLEARED" << endl;
		m_cam->m_camX = m_cam->m_camXWallJump;
		m_cam->m_camZ = m_cam->m_camZWallJump;
		m_cam->m_cameraLookAt = m_cam->m_cameraCenter+m_cam->m_camZ;
		m_physics->m_velocityDiff = 15.0f*m_physics->m_velocityDiffWallJump;
		m_physics->m_velocityDiff.y*=15.0f*m_bounceFactor;
		//m_physics->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

		m_physics->m_currentState = PhysicsStates::None;
		m_onTopOfBuildingId = m_wallJumpingBuildingId;
		m_wallJumpingBuildingId = -1;

	}
	else {

		if (!m_physics->m_wallJumpLookedDown && m_cam->m_yRotated > m_physics->m_wallJumpLookDown) {
			m_cam->m_camZ = glm::rotate(m_cam->m_camZ, m_physics->m_wallJumpLookDownIncrement, m_cam->m_camX);
			m_cam->m_yRotated+=m_physics->m_wallJumpLookDownIncrement;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;

			if (m_cam->m_yRotated < m_physics->m_wallJumpLookDown) {
				m_physics->m_wallJumpLookedDown = true;
			}
		}
		else if (m_physics->m_wallJumpLookedDown && m_cam->m_yRotated < 0.0f) {
			m_cam->m_camZ = glm::rotate(m_cam->m_camZ, m_physics->m_wallJumpLookReadjustIncrement, m_cam->m_camX);
			m_cam->m_yRotated+=m_physics->m_wallJumpLookReadjustIncrement;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;

			// technically should never happen
			if (m_cam->m_yRotated > 0.0f) {
				cout << "readjusting y" << endl;
				m_cam->m_camZ = glm::rotate(m_cam->m_camZ, -1.0f*m_cam->m_yRotated, m_cam->m_camX);
				m_cam->m_yRotated+=-1.0f*m_cam->m_yRotated;
				m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
			}

		}

		if (!m_physics->m_wallJumpLookedRight && m_cam->m_xRotated < m_physics->m_wallJumpLookRight + m_physics->m_wallJumpLookXHolder) {
			
			glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);

			tmp_camZ = glm::rotate(tmp_camZ, m_physics->m_wallJumpLookXIncrement, glm::vec3(0.0f, 1.0f, 0.0f));
			m_cam->m_camX = glm::rotate(m_cam->m_camX, m_physics->m_wallJumpLookXIncrement, glm::vec3(0.0f, 1.0f, 0.0f));

			m_cam->m_camZ = glm::vec3(tmp_camZ.x, m_cam->m_camZ.y, tmp_camZ.z);
			m_cam->m_xRotated+=m_physics->m_wallJumpLookXIncrement;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;

			if (m_cam->m_xRotated > m_physics->m_wallJumpLookRight + m_physics->m_wallJumpLookXHolder) {
				m_physics->m_wallJumpLookedRight = true;
			}
					
		}

		else if (m_physics->m_wallJumpLookedRight && m_cam->m_xRotated > m_physics->m_wallJumpLookXHolder) {
			glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);

			tmp_camZ = glm::rotate(tmp_camZ, -1.0f*m_physics->m_wallJumpLookXIncrement, glm::vec3(0.0f, 1.0f, 0.0f));
			m_cam->m_camX = glm::rotate(m_cam->m_camX, -1.0f*m_physics->m_wallJumpLookXIncrement, glm::vec3(0.0f, 1.0f, 0.0f));
				
			m_cam->m_camZ = glm::vec3(tmp_camZ.x, m_cam->m_camZ.y, tmp_camZ.z);
			m_cam->m_xRotated+= -1.0f*m_physics->m_wallJumpLookXIncrement;
			m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;

			// technically should never happen
			if (m_cam->m_xRotated < m_physics->m_wallJumpLookXHolder) {
				cout << "readjusting x" << endl;
				glm::vec3 tmp_camZ = glm::vec3(m_cam->m_camZ.x, 0.0f, m_cam->m_camZ.z);

				tmp_camZ = glm::rotateY(tmp_camZ, m_physics->m_wallJumpLookXHolder-m_cam->m_xRotated);
				m_cam->m_camX = glm::rotateY(m_cam->m_camX,m_physics->m_wallJumpLookXHolder-m_cam->m_xRotated);

				m_cam->m_camZ = glm::vec3(tmp_camZ.x, m_cam->m_camZ.y, tmp_camZ.z);
				m_cam->m_xRotated+=m_physics->m_wallJumpLookXHolder-m_cam->m_xRotated;
				m_cam->m_cameraLookAt = m_cam->m_cameraCenter + m_cam->m_camZ;
			}

		}
		

		m_physics->m_velocityDiff = glm::vec3(0.0f, m_wallJumpFactor, 0.0f);
		m_physics->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	m_cam->updateCameraMatrix();

}

// TODO: imp if needed
void MyPlayer::Unstuck() {
	/*glm::vec3 newPos = m_physics->handleUnstuck() {
	}*/
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