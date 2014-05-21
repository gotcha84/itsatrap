#include "Physics.h"


float Physics::handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e) {
	float angle = e.aabb.angleIntersection(from, goTo);
	return angle;
}

int Physics::handleReflectionIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e) {
	int newDirection = e.aabb.reflectionIntersection(from, goTo);
	return newDirection;
}

bool Physics::handleNearTop(glm::vec3 from, struct staticObject e) {
	bool nearTop = e.aabb.nearTop(from);
	return nearTop;
}

bool Physics::handleClearedTop(AABB p, struct staticObject e) {
	bool nearTop = e.aabb.clearedTop(p);
	return nearTop;
}