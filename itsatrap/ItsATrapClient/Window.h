#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ClientInstance.h"
#include "SceneGraph.h"
#include "MyPlayer.h"

class Window { // output window related routines
	public:
		static int m_width, m_height; // window size
		static float spin_angle;
		static GLfloat red, green, blue;

		Window();
		~Window();

		// might need y total (90 degrees up or down)
		// might need turn factor

		static void idleCallback();
		static void reshapeCallback(int, int);
		static void displayCallback();
		static void displaySceneGraph();
		static void processNormalKeys(unsigned char, int, int);
		static void processMouseMove(int, int);

		//static void drawShape(int, vector<float>, vector<float>, vector<float>);
		static void drawShape(int*, float**, float**, float**, int*, int**);
};

#endif