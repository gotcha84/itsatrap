#pragma once
#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream> // to convert float to string
#include <string>
#include <math.h>
#include <iomanip> // to round floats

#include "Camera.h"
//#include "Physics.h"
#include "NetworkObjects.h"
#include "ConfigSettings.h"
#include "InfoMessage.h"

using namespace std;

#define KNIFE_RANGE 15.0f

class MyPlayer {
	public:
		int m_numKills, m_numDeaths, m_health, m_stunDuration, m_slowDuration, m_resources, m_timeUntilRespawn, m_flashDuration, m_bloodDuration, m_hitCrosshairDuration;
		int m_recallElapsed;
		bool m_startBlood;
		InfoMessage m_infoMsg;
		bool m_deathState;

		Camera *m_cam;
		//Physics *m_physics;
		AABB *m_boundingBox;

		glm::vec3 m_position;
		PhysicsStates m_currPhysState;


		glm::mat4 m_transMatrix;

		glm::mat4 m_modelMatrix;
		glm::mat4 m_modelviewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewportMatrix;

		float m_xWalkFactor; // how fast you want to walk
		float m_zWalkFactor;
		float m_xSlowWalkFactor;
		float m_zSlowWalkFactor;
		float m_climbFactor;
		float m_pullingUpFactor;
		float m_wallJumpTime;
		float m_holdingEdgeTime;
		float m_teleportFactor;
		float m_slideFactor;
		float m_bounceFactor;

		float m_miniJumpYVelocityThreshold;

		int m_wallJumpingBuildingId;
		int m_onTopOfBuildingId;

		MyPlayer();
		MyPlayer(glm::vec3 pos);
		~MyPlayer();

		void initCommon();

		Camera *getCamera();
		glm::vec3 getPosition();
		//Physics *getPhysics();
		
		AABB *getAABB();
		void setAABB(AABB *bbox);
		//void setAABB(AABB bbox);

		glm::mat4 getTransMatrix();
		glm::mat4 getModelMatrix();
		glm::mat4 getCameraMatrix();
		glm::mat4 getModelViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewPortMatrix();

		void handleXRotation(float magnitude);
		void handleYRotation(float magnitude);
		void handleSliding();
		void handleTeleport();
		
		void Unstuck(unsigned char key);
		
		void updateModelViewMatrix();
		void setTransMatrix(glm::mat4 m);
		void setModelMatrix(glm::mat4 m);
		void setProjectionMatrix();
		void setViewportMatrix();

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);

		void lookIn(glm::vec3 direction);
		void lookAt(glm::vec3 point);

		void updateBoundingBox();
		bool collidesWith(MyPlayer *other);
		//bool knifeHitWith(MyPlayer *other);

		int getHealth();
		void setHealth(int health);
		void damage(int dmg);
		bool isDead();
		bool isAlive();
};

#endif