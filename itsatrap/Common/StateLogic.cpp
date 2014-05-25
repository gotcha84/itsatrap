#include "StateLogic.h"

struct stateInfo StateLogic::statesInfo[MAX_PLAYERS];

void StateLogic::clearStateInfo(int id) {
	statesInfo[id].initialX = 0.0f;
	statesInfo[id].initialY = 0.0f;
	statesInfo[id].initialUp = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].numFrames = 0.0f;

	statesInfo[id].Holder.camX = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.camZ = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.cameraUp = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.position = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Holder.xRotated = 0.0f;
	statesInfo[id].Holder.yRotated = 0.0f;

	statesInfo[id].Start.lookX = 0.0f;
	statesInfo[id].Start.lookY = 0.0f;
	statesInfo[id].Start.camUp = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Start.fraction = 0.0f;
	statesInfo[id].Start.lookXIncrement = 0.0f;
	statesInfo[id].Start.lookYIncrement = 0.0f;
	statesInfo[id].Start.camUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Start.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Start.counter = 0;

	statesInfo[id].Mid.lookX = 0.0f;
	statesInfo[id].Mid.lookY = 0.0f;
	statesInfo[id].Mid.camUp = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Mid.fraction = 0.0f;
	statesInfo[id].Mid.lookXIncrement = 0.0f;
	statesInfo[id].Mid.lookYIncrement = 0.0f;
	statesInfo[id].Mid.camUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Mid.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].Mid.counter = 0;

	statesInfo[id].End.lookX = 0.0f;
	statesInfo[id].End.lookY = 0.0f;
	statesInfo[id].End.camUp = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].End.fraction = 0.0f;
	statesInfo[id].End.lookXIncrement = 0.0f;
	statesInfo[id].End.lookYIncrement = 0.0f;
	statesInfo[id].End.camUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].End.velocityDiff = glm::vec3(0.0f, 0.0f, 0.0f);
	statesInfo[id].End.counter = 0;
}


void StateLogic::startClimbing(struct playerObject *e, int buildingId) {
	cout << "started climbing" << endl;
	StateLogic::clearStateInfo(e->id);
	// CLIMBING
	float CLinitialX = e->cameraObject.xRotated;
	float CLinitialY = e->cameraObject.yRotated;

	float CLnumFrames = 100.0f;
	//ConfigSettings::getConfig()->getValue("CLnumFrames", CLnumFrames);

	// start
	float CLStartlookX = 0.0f - CLinitialX;
	float CLStartlookY = 70.0f - CLinitialY;

	float CLStartfraction = 1.0f;

	float CLStartlookXIncrement = 0.0f;
	float CLStartlookYIncrement = CLStartlookY*CLStartfraction / CLnumFrames;
	glm::vec3 CLStartlookUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);

	float climbFactor = 3.0f;
	//ConfigSettings::getConfig()->getValue("climbFactor", climbFactor);

	glm::vec3 CLStartvelocityDiff = glm::vec3(0.0f, climbFactor, 0.0f);

	int CLStartCounter = 0;

	// Mid
	glm::vec3 CLMidvelocityDiff = glm::vec3(0.0f, climbFactor, 0.0f);

	// End
	float CLEndfraction = 0.1f;
	float CLEndlookYIncrement = -70.0f*CLEndfraction / CLnumFrames;

	int CLEndCounter = 0;

	StateLogic::statesInfo[e->id].initialX = CLinitialX;
	StateLogic::statesInfo[e->id].initialY = CLinitialY;
	StateLogic::statesInfo[e->id].numFrames = CLnumFrames;

	StateLogic::statesInfo[e->id].Start.lookX = CLStartlookX;
	StateLogic::statesInfo[e->id].Start.lookY = CLStartlookY;
	StateLogic::statesInfo[e->id].Start.fraction = CLStartfraction;
	StateLogic::statesInfo[e->id].Start.lookXIncrement = CLStartlookXIncrement;
	StateLogic::statesInfo[e->id].Start.lookYIncrement = CLStartlookYIncrement;
	StateLogic::statesInfo[e->id].Start.velocityDiff = CLStartvelocityDiff;
	StateLogic::statesInfo[e->id].Start.counter = CLStartCounter;

	StateLogic::statesInfo[e->id].Mid.velocityDiff = CLMidvelocityDiff;

	StateLogic::statesInfo[e->id].End.fraction = CLEndfraction;
	StateLogic::statesInfo[e->id].End.lookYIncrement = CLEndlookYIncrement;
	StateLogic::statesInfo[e->id].End.counter = CLEndCounter;

	e->interactingWithBuildingId = buildingId;
	e->currminiState = innerStates::Starting;
	e->currState = PhysicsStates::Climbing;

}

void StateLogic::startHoldingEdge(struct playerObject *e, int buildingId) {
	cout << "started holding edge" << endl;
	StateLogic::clearStateInfo(e->id);
	//int direction = staticObjects[e->interactingWithBuildingId].aabb.reflectionIntersection(e->position, e->position + e->velocity + e->velocityDiff);

	// TODO: imp if necessary
	/*
	switch (direction) {
	case 0:
	//set HEstartlookX etc

	break;
	case 1:
	break;
	case 4:
	break;
	case 5:
	break;
	}
	*/

	float HEinitialX = e->cameraObject.xRotated;
	float HEinitialY = e->cameraObject.yRotated;
	glm::vec3 HEHoldervelocityDiff = e->velocityDiff;

	float HEnumFrames = 100.0f;
	//ConfigSettings::getConfig()->getValue("HEnumFrames", HEnumFrames);

	// start
	float HEStartlookY = 70.0f - HEinitialY;

	float HEStartfraction = 1.0f;

	float HEStartlookYIncrement = HEStartlookY*HEStartfraction / HEnumFrames;
	glm::vec3 HEStartlookUpIncrement = glm::vec3(0.0f, 0.0f, 0.0f);

	int HEStartCounter = 0;

	// End
	float HEEndlookX = 180.0f;// calculate
	float HEEndlookY = -70.0f;

	float HEEndfraction = 0.1f;

	float HEEndlookXIncrement = HEEndlookX*HEEndfraction / HEnumFrames;
	float HEEndlookYIncrement = HEEndlookY*HEEndfraction / HEnumFrames;

	float bounceFactor = 5.0f;
	//ConfigSettings::getConfig()->getValue("bounceFactor", bounceFactor);


	glm::vec3 HEEndVeloDiff = glm::vec3(-1.0f*HEHoldervelocityDiff.x, bounceFactor, -1.0f*HEHoldervelocityDiff.z);

	int HEEndCounter = 0;

	StateLogic::statesInfo[e->id].initialX = HEinitialX;
	StateLogic::statesInfo[e->id].initialY = HEinitialY;
	StateLogic::statesInfo[e->id].numFrames = HEnumFrames;

	StateLogic::statesInfo[e->id].Holder.velocityDiff = HEHoldervelocityDiff;

	StateLogic::statesInfo[e->id].Start.lookY = HEStartlookY;
	StateLogic::statesInfo[e->id].Start.fraction = HEStartfraction;
	StateLogic::statesInfo[e->id].Start.lookYIncrement = HEStartlookYIncrement;
	StateLogic::statesInfo[e->id].Start.counter = HEStartCounter;

	StateLogic::statesInfo[e->id].End.lookX = HEEndlookX;
	StateLogic::statesInfo[e->id].End.lookY = HEEndlookY;
	StateLogic::statesInfo[e->id].End.fraction = HEEndfraction;
	StateLogic::statesInfo[e->id].End.lookXIncrement = HEEndlookXIncrement;
	StateLogic::statesInfo[e->id].End.lookYIncrement = HEEndlookYIncrement;
	StateLogic::statesInfo[e->id].End.counter = HEEndCounter;

	e->interactingWithBuildingId = buildingId;
	e->currminiState = innerStates::Starting;
	e->currState = PhysicsStates::HoldingEdge;

}

void StateLogic::startPullingUp(struct playerObject *e, int buildingId) {
	cout << "start pulling up\n";
	StateLogic::clearStateInfo(e->id);
	float PUinitialX = e->cameraObject.xRotated;
	float PUinitialY = e->cameraObject.yRotated;

	float PUnumFrames = 30.0f;
	//ConfigSettings::getConfig()->getValue("PUnumFrames", PUnumFrames);

	// start
	float PUStartlookX = 45.0f - PUinitialX;
	float PUStartlookY = -20.0f - PUinitialY;

	float PUStartfraction = 0.7f;

	float PUStartlookXIncrement = PUStartlookX*PUStartfraction / PUnumFrames;
	float PUStartlookYIncrement = PUStartlookY*PUStartfraction / PUnumFrames;

	float pullingUpFactor = 3.0f;
	//ConfigSettings::getConfig()->getValue("pullingUpFactor", pullingUpFactor);
	glm::vec3 PUStartvelocityDiff = glm::vec3(0.0f, pullingUpFactor, 0.0f);

	int PUStartCounter = 0;

	// end
	float PUEndlookX = -1.0f*PUEndlookX;
	float PUEndlookY = -1.0f*PUEndlookY;

	float PUEndfraction = 0.3f;

	float PUEndlookXIncrement = PUEndlookX*PUEndfraction / PUnumFrames;
	float PUEndlookYIncrement = PUEndlookY*PUEndfraction / PUnumFrames;

	glm::vec3 PUEndvelocityDiff = glm::vec3(0.0f, pullingUpFactor, 0.0f);

	int PUEndCounter = 0;

	statesInfo[e->id].initialX = PUinitialX;
	statesInfo[e->id].initialY = PUinitialY;
	statesInfo[e->id].numFrames = PUnumFrames;

	statesInfo[e->id].Start.lookX = PUStartlookX;
	statesInfo[e->id].Start.lookY = PUStartlookY;
	statesInfo[e->id].Start.fraction = PUStartfraction;
	statesInfo[e->id].Start.lookXIncrement = PUStartlookXIncrement;
	statesInfo[e->id].Start.lookYIncrement = PUStartlookYIncrement;
	statesInfo[e->id].Start.velocityDiff = PUStartvelocityDiff;
	statesInfo[e->id].Start.counter = PUStartCounter;

	statesInfo[e->id].End.lookX = PUEndlookX;
	statesInfo[e->id].End.lookY = PUEndlookY;
	statesInfo[e->id].End.fraction = PUEndfraction;
	statesInfo[e->id].End.lookXIncrement = PUEndlookXIncrement;
	statesInfo[e->id].End.lookYIncrement = PUEndlookYIncrement;
	statesInfo[e->id].End.velocityDiff = PUEndvelocityDiff;
	statesInfo[e->id].End.counter = PUEndCounter;

	e->interactingWithBuildingId = buildingId;
	e->currminiState = innerStates::Starting;
	e->currState = PhysicsStates::PullingUp;

	return;
}

void StateLogic::startWallRunning(struct playerObject *e, int newDirection, glm::vec3 toAdd, float angle, int buildingId) {

	// WallRunning
	cout << "starting wall running" << endl;
	StateLogic::clearStateInfo(e->id);
	float WRinitialX = e->cameraObject.xRotated;
	float WRinitialY = e->cameraObject.yRotated;
	glm::vec3 WRinitialcamUp = e->cameraObject.cameraUp;

	float WRnumFrames = 60.0f;
	//ConfigSettings::getConfig()->getValue("WRnumFrames", WRnumFrames);

	float WRStartfraction = 0.7f;
	//ConfigSettings::getConfig()->getValue("WRStartfraction", WRStartfraction);

	float WREndfraction = 0.3f;
	//ConfigSettings::getConfig()->getValue("WREndfraction", WREndfraction);

	glm::vec3 WRHoldercamZ;
	glm::vec3  WRHoldervelocityDiff;
	glm::vec3 WRStartcamZ;
	glm::vec3 WRStartcamX;
	glm::vec3 WRStartcamUp;
	glm::vec3 WRStartcamUpIncrement;
	glm::vec3 WREndcamUpIncrement;
	glm::vec3 WRMidvelocityDiff;

	float WRStartlookXIncrement;
	float WREndlookXIncrement;
	float WRHolderxRotated;

	if (newDirection == 0 || newDirection == 1) {
		WRHoldercamZ = glm::vec3(e->cameraObject.camZ.x*-1.0f, e->cameraObject.camZ.y, e->cameraObject.camZ.z);
		toAdd.z *= -1.0f;
		WRHoldervelocityDiff = glm::vec3(e->velocityDiff.x*-1.0f, e->velocityDiff.y, e->velocityDiff.z);
		WRHoldervelocityDiff += toAdd;
		
		// TODO: change 0.8 to use config file,
		WRStartcamZ = glm::vec3(0.0f, 0.0f, 1.0f);
		WRStartcamX = glm::vec3(0.0f, -1.0f, 0.0f);
		WRStartcamUp = glm::vec3(-0.8f, 0.8f, 0.0f);

	}
	if (newDirection == 4 || newDirection == 5) {
		WRHoldercamZ = glm::vec3(e->cameraObject.camZ.x, e->cameraObject.camZ.y, e->cameraObject.camZ.z*-1.0f);
		toAdd.z *= -1.0f;
		WRHoldervelocityDiff = glm::vec3(e->velocityDiff.x, e->velocityDiff.y, e->velocityDiff.z*-1.0f);
		WRHoldervelocityDiff += toAdd;

		WRStartcamZ = glm::vec3(1.0f, 0.0f, 0.0f);
		WRStartcamX = glm::vec3(0.0f, 1.0f, 0.0f);
		WRStartcamUp = glm::vec3(0.0f, 0.8f, -0.8f);
	}

	if (newDirection % 2 == 1) {
		WRStartcamX *= -1.0f;
		WRStartcamUp.x *= -1.0f;
		WRStartcamUp.z *= -1.0f;
	}

	WRStartcamUpIncrement = glm::vec3(WRStartcamUp - WRinitialcamUp)*WRStartfraction / WRnumFrames;
	WREndcamUpIncrement = -1.0f*glm::vec3(WRStartcamUp - WRinitialcamUp)*WREndfraction / WRnumFrames;

	/*	1 +,-
	2 -,+
	3 -,+
	4 +,-*/
	/*cout << "from: " << glm::to_string(m_cam->m_cameraUp) << endl;
	cout << "get to: " << glm::to_string(m_cam->m_cameraUpWallRun) << endl;
	cout << "with: " << glm::to_string(m_physics->m_wallRunLookUpReadjustIncrement) << endl;*/



	if (angle < 90.0f) {

		WRStartlookXIncrement = -1.0f*angle*WRStartfraction / WRnumFrames;
		WREndlookXIncrement = -1.0f*angle*WREndfraction / WRnumFrames;

		if (newDirection == 0 || newDirection == 5) {
			WRHolderxRotated = e->cameraObject.xRotated - (2.0f*angle);
		}
		if (newDirection == 1 || newDirection == 4) {
			WRHolderxRotated = e->cameraObject.xRotated - (2.0f*(180.0f - angle));
			WRStartlookXIncrement *= -1.0f;
			WREndlookXIncrement *= -1.0f;
		}
	}
	if (angle > 90.0f) {

		WRStartlookXIncrement = (abs(180.0f - angle))*WRStartfraction / WRnumFrames;
		WREndlookXIncrement = (abs(180.0f - angle))*WREndfraction / WRnumFrames;

		if (newDirection == 0 || newDirection == 5) {
			WRHolderxRotated = e->cameraObject.xRotated + (2.0f*(180.0f - angle));
		}
		if (newDirection == 1 || newDirection == 4) {
			WRHolderxRotated = e->cameraObject.xRotated + (2.0f*angle);
			WRStartlookXIncrement *= -1.0f;
			WREndlookXIncrement *= -1.0f;
		}
		WRStartcamZ *= -1.0f;
		WRStartcamX *= -1.0f;

	}

	float wallRunFactor = 3.0f;
	//ConfigSettings::getConfig()->getValue("wallRunFactor", wallRunFactor);

	WRMidvelocityDiff = WRStartcamZ*wallRunFactor;

	statesInfo[e->id].initialX = WRinitialX;
	statesInfo[e->id].initialY = WRinitialY;
	statesInfo[e->id].initialUp = WRinitialcamUp;
	statesInfo[e->id].numFrames = WRnumFrames;

	statesInfo[e->id].Holder.camZ = WRHoldercamZ;
	statesInfo[e->id].Holder.velocityDiff = WRHoldervelocityDiff;
	statesInfo[e->id].Holder.xRotated = WRHolderxRotated;

	statesInfo[e->id].Start.camUp = WRStartcamUp;
	statesInfo[e->id].Start.fraction = WRStartfraction;
	statesInfo[e->id].Start.lookXIncrement = WRStartlookXIncrement;
	statesInfo[e->id].Start.camUpIncrement = WRStartcamUpIncrement;

	statesInfo[e->id].Mid.velocityDiff = WRMidvelocityDiff;

	statesInfo[e->id].End.fraction = WREndfraction;
	statesInfo[e->id].End.lookXIncrement = WREndlookXIncrement;

	e->feetPlanted = true; // not sure if makes a dfference
	e->interactingWithBuildingId = buildingId;
	e->currminiState = innerStates::Starting;
	e->currState = PhysicsStates::WallRunning;
}

void StateLogic::applyClimbing(struct playerObject *p) {

	switch (p->currminiState) {
	case innerStates::Starting:
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].Start.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].Start.lookYIncrement);
		p->velocityDiff = StateLogic::statesInfo[p->currState].Start.velocityDiff;
		p->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		StateLogic::statesInfo[p->currState].Start.counter++;
		if (StateLogic::statesInfo[p->currState].Start.counter >= StateLogic::statesInfo[p->currState].Start.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Mid;
		}
		break;
	case innerStates::Mid:
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].Mid.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].Mid.lookYIncrement);
		p->velocityDiff = StateLogic::statesInfo[p->currState].Mid.velocityDiff;
		p->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case innerStates::Ending:
		p->interactingWithBuildingId = -1;
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].End.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].End.lookYIncrement);
		p->cameraObject.cameraUp += StateLogic::statesInfo[p->currState].End.camUpIncrement;
		p->velocityDiff = StateLogic::statesInfo[p->currState].End.velocityDiff;
		StateLogic::statesInfo[p->currState].End.counter++;
		if (StateLogic::statesInfo[p->currState].End.counter >= StateLogic::statesInfo[p->currState].End.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Off;
			cout << "ended state: " << p->currState << endl;
			p->currState = PhysicsStates::None;
		}
		break;
	}

}

void StateLogic::applyHoldingEdge(struct playerObject *p) {

	switch (p->currminiState) {
	case innerStates::Starting:
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].Start.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].End.lookYIncrement);
		StateLogic::statesInfo[p->currState].Start.counter++;
		if (StateLogic::statesInfo[p->currState].Start.counter >= StateLogic::statesInfo[p->currState].Start.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Mid;
		}
		break;
	case innerStates::Ending:
		p->interactingWithBuildingId = -1;
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].End.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].End.lookYIncrement);
		p->velocityDiff = StateLogic::statesInfo[p->currState].End.velocityDiff;
		StateLogic::statesInfo[p->currState].End.counter++;
		if (StateLogic::statesInfo[p->currState].End.counter >= StateLogic::statesInfo[p->currState].End.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Off;
			cout << "ended state: " << p->currState << endl;
			p->currState = PhysicsStates::None;
		}
		break;
	}

}

void StateLogic::applyPullingUp(struct playerObject *p) {

	switch (p->currminiState) {
	case innerStates::Starting:
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].Start.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].Start.lookYIncrement);
		p->velocityDiff = StateLogic::statesInfo[p->currState].Start.velocityDiff;
		StateLogic::statesInfo[p->currState].Start.counter++;
		if (StateLogic::statesInfo[p->currState].Start.counter >= StateLogic::statesInfo[p->currState].Start.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Ending;
		}
		break;
	case innerStates::Ending:
		p->interactingWithBuildingId = -1;
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].End.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].End.lookYIncrement);
		p->velocityDiff = StateLogic::statesInfo[p->currState].End.velocityDiff;
		StateLogic::statesInfo[p->currState].End.counter++;
		if (StateLogic::statesInfo[p->currState].End.counter >= StateLogic::statesInfo[p->currState].End.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Off;
			cout << "ended state: " << p->currState << endl;
			p->currState = PhysicsStates::None;
		}
		break;
	}

}

void StateLogic::applyWallRunning(struct playerObject *p) {

	switch (p->currminiState) {
	case innerStates::Starting:
		if (p->triedToRun) {
			StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].Start.lookXIncrement);
			StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].Start.lookYIncrement);
			p->cameraObject.cameraUp += StateLogic::statesInfo[p->currState].Start.camUpIncrement;
			p->velocityDiff = StateLogic::statesInfo[p->currState].Start.velocityDiff;
			StateLogic::statesInfo[p->currState].Start.counter++;
			if (StateLogic::statesInfo[p->currState].Start.counter >= StateLogic::statesInfo[p->currState].Start.fraction*StateLogic::statesInfo[p->currState].numFrames) {
				p->currminiState = innerStates::Mid;
			}
		}
		else {
			p->currminiState = innerStates::Ending;
		}
		break;
	case innerStates::Mid:
		if (p->triedToRun) {
			p->velocityDiff = StateLogic::statesInfo[p->currState].Mid.velocityDiff;
		}
		else {
			p->currminiState = innerStates::Ending;
		}
		break;
	case innerStates::Ending:
		p->interactingWithBuildingId = -1;
		StateLogic::handleXRotation(p, StateLogic::statesInfo[p->currState].End.lookXIncrement);
		StateLogic::handleYRotation(p, StateLogic::statesInfo[p->currState].End.lookYIncrement);
		p->cameraObject.cameraUp += StateLogic::statesInfo[p->currState].End.camUpIncrement;
		p->velocityDiff = StateLogic::statesInfo[p->currState].End.velocityDiff;
		StateLogic::statesInfo[p->currState].End.counter++;
		if (StateLogic::statesInfo[p->currState].End.counter >= StateLogic::statesInfo[p->currState].End.fraction*StateLogic::statesInfo[p->currState].numFrames) {
			p->currminiState = innerStates::Off;
			cout << "ended state: " << p->currState << endl;
			p->currState = PhysicsStates::None;
		}
		break;
	}

}


void StateLogic::handleXRotation(struct playerObject *e, float angle) {
	glm::vec3 tmp_camZ = glm::vec3(e->cameraObject.camZ.x, 0.0f, e->cameraObject.camZ.z);

	tmp_camZ = glm::rotateY(tmp_camZ, angle);
	e->cameraObject.camX = glm::rotateY(e->cameraObject.camX, angle);

	e->cameraObject.camZ = glm::vec3(tmp_camZ.x, e->cameraObject.camZ.y, tmp_camZ.z);
	e->cameraObject.cameraLookAt = e->cameraObject.cameraCenter + e->cameraObject.camZ;

	e->cameraObject.xRotated += angle;
}

void StateLogic::handleYRotation(struct playerObject *e, float angle) {
	// TODO modify upvector too for confuse ray
	if (!(e->cameraObject.yRotated > 80.0f && angle > 0) && !(e->cameraObject.yRotated < -80.0f && angle < 0)) {

		//m_camZ.y+=magnitude*m_yRotationAngle; // both this and the two lines below seem okay
		e->cameraObject.camZ = glm::rotate(e->cameraObject.camZ, angle, e->cameraObject.camX);
		e->cameraObject.yRotated += angle;
		//cout << "ROTATING CAM: " << m_yRotated << ", " << glm::radians(80.0f) << endl;
	}
	//cout << "mcamX: " << glm::to_string(m_camX) << endl;
	//cout << "before: " << glm::to_string(m_cameraLookAt) << endl;
	e->cameraObject.cameraLookAt = e->cameraObject.cameraCenter + e->cameraObject.camZ;

	//cout << "after: " << glm::to_string(m_cameraLookAt) << endl << endl;
}

void StateLogic::calculateAxis(struct playerObject *e) {
	glm::vec3 ZCameraDiff = e->cameraObject.cameraLookAt - e->cameraObject.cameraCenter;
	//ZCameraDiff.y = 0.0f;

	// not sure if need to normalize
	e->cameraObject.camZ = glm::normalize(ZCameraDiff);

	float oldmcamXY = e->cameraObject.camX.y;
	// or is it radians
	e->cameraObject.camX = glm::rotateY(e->cameraObject.camZ, -90.0f);
	e->cameraObject.camX = glm::vec3(e->cameraObject.camX.x, oldmcamXY, e->cameraObject.camX.z);

}

