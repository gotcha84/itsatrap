#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Geode.h"

class Sphere : public Geode {
	public:
		virtual void draw(glm::mat4 parent);
};

#endif