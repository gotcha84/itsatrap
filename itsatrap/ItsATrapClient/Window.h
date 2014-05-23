#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "MyPlayer.h"
#include "SceneGraph.h"

class Window {
	public:
		static int m_width, m_height; // window size
		static int m_heightMapXShift, m_heightMapZShift;
		static int m_fpsCounter;
		static clock_t m_timer;

		static bool *keyState;
		static bool *specialKeyState;
		static int modifierKey;

		Window();
		~Window();

		// might need y total (90 degrees up or down)
		// might need turn factor

		static void idleCallback();
		static void reshapeCallback(int, int);
		static void displayCallback();
		static void displayCallback2();
		static void keyDown(unsigned char, int, int);
		static void keyUp(unsigned char key, int x, int y);
		static void specialKeyDown(int key, int x, int y);
		static void specialKeyUp(int key, int x, int y);

		static void processKeys();

		static void processMouseKeys(int, int, int, int);
		static void processMouseMove(int, int);
};

#endif