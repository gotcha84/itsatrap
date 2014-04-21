#pragma once
#ifndef CLIENTINSTANCE_H
#define CLIENTINSTANCE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneGraph.h"

class ClientInstance {
	public:
		ClientInstance();
		~ClientInstance();

		sg::Player *root;

		int m_xMouse, m_yMouse; // mouse position
		float m_xAngleChange, m_yAngleChange;
		float m_xAngleChangeFactor, m_yAngleChangeFactor;

		int x, inc;

		// add model,camera,proj,viewport matrices here? (init in ctor)
};

#endif CLIENTINSTANCE_H
