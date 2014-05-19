#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "AABB.h"
#include "Utilities.h"
#include "World.h"

using namespace std;

enum PhysicsStates {
	None = 0,
	Colliding = 2,
	Moving = 3,
	Climbing = 5,
	Sliding = 6,
	WallJumping = 7,
	PullingUp = 8,
	HoldingEdge = 9,
	WallRunning = 10
};

enum WallRunningYStates {
	WRY_Off = 0, // same as readjusted
	WRY_Adjusting = 1,
	WRY_Adjusted = 2,
	WRY_Readjusting = 3
};

enum WallRunningXStates {
	WRX_Off = 0, // same as readjusted
	WRX_Adjusting = 1,
	WRX_Adjusted = 2,
	WRX_Readjusting = 3
};

class Physics {
	public:

		PhysicsStates m_currentState;
		WallRunningYStates m_currentWallRunningYState;
		WallRunningXStates m_currentWallRunningXState;
		clock_t m_stateStart;
		clock_t m_lastTeleported;
		clock_t m_lastSlid;
		float m_teleportDelay;
		float m_slideDelay;

		glm::vec3 m_gravity;
		glm::vec3 m_velocity;
		glm::vec3 m_velocityDiff;
		glm::vec3 m_velocityDiffWallJump;
		glm::vec3 m_velocityDiffWallRun;
		glm::vec3 m_position;
		glm::vec3 m_lastMoved;

		float m_gravityConstant; 
		float m_elasticityConstant;
		glm::vec3 m_restConstant;

		float m_climbLookUp;
		float m_climbLookDown;
		float m_climbLookUpDownNumFrames;
		float m_climbLookDownReadjustNumFrames;
		float m_climbLookDownNumFramesFraction;
		float m_climbLookReadjustNumFramesFraction;
		float m_climbLookUpIncrement;
		float m_climbLookDownIncrement;
		float m_climbLookReadjustIncrement;
		bool m_climbLookedDown;

		float m_climbLookXHolder;
		float m_climbLookRight;
		float m_climbLookXsNumFrames;
		float m_climbLookXNumFramesFraction;
		float m_climbLookBackXNumFramesFraction;
		float m_climbLookXIncrement;
		float m_climbLookBackXIncrement;
		bool m_climbLookedRight;

		glm::vec3 m_wallRunForward;

		float m_mass;
		float m_yJumpFactor;

		bool m_triedToRun;
		bool m_canJump;
		bool m_feetPlanted;

		float m_wallRunLookUpAdjustNumFrames;
		float m_wallRunLookUpReadjustNumFrames;
		glm::vec3 m_wallRunLookUpAdjustIncrement;
		glm::vec3 m_wallRunLookUpReadjustIncrement;
		
		float m_wallRunLookXAdjustNumFrames;
		float m_wallRunLookXReadjustNumFrames;
		float m_wallRunLookXAdjustIncrement;
		float m_wallRunLookXReadjustIncrement;


		bool m_wallRunAdjustedLookUp;
		int m_wallRunAdjustUpCounter;
		bool m_wallRunReadjustedLookUp;
		int m_wallRunReadjustUpCounter;

		bool m_wallRunAdjustedLookX;
		int m_wallRunAdjustXCounter;
		bool m_wallRunReadjustedLookX;
		int m_wallRunReadjustXCounter;

		Physics();
		Physics(glm::vec3 pos);
		Physics(glm::vec3 pos, float mass);
		~Physics();

		void initCommon();

		int applyGravity(AABB* player);

		bool atRest();
		void toggleTriedToRun();
		int handleCollisionDetection(AABB* other);
		float handleIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other); // not used
		float handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other, int buildingId);
		int handleReflectionIntersection(glm::vec3 from, glm::vec3 goTo, AABB* other, int buildingId);
		bool handleNearTop(glm::vec3 from, int buildingId);
		bool handleClearedTop(AABB* other, int buildingId);

		void move(glm::vec3 delta);
		void moveTo(glm::vec3 pos);

};

#endif