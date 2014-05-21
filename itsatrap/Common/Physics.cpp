#include "Physics.h"


float Physics::handleAngleIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e) {
	float angle = e.aabb.angleIntersection(from, goTo);
	return angle;
}

int Physics::handleReflectionIntersection(glm::vec3 from, glm::vec3 goTo, struct staticObject e) {
	int newDirection = e.aabb.reflectionIntersection(from, goTo);
	return newDirection;

}