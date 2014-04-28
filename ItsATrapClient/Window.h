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

#include "MyPlayer.h"
#include "SceneGraph.h"

class Window {
	public:
		static int m_width, m_height; // window size
		static int m_heightMapXShift, m_heightMapZShift;

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
};

#endif