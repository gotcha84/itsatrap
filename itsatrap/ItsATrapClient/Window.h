#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "SceneGraph.h"
#include "MyPlayer.h"

class Window	  // output window related routines
{
  public:
	MyPlayer m_myPlayer;
	static MatrixTransform *root;
	MatrixTransform *trans;

    int m_width, m_height; 	            // window size
	int m_xMouse, m_yMouse;				// mouse position
	float m_xAngleChange, m_yAngleChange;
	float m_xAngleChangeFactor, m_yAngleChangeFactor;
	int x;
	int inc;
	// might need y total (90 degrees up or down)
	// might need turn factor

	Window();
	~Window();

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void displaySceneGraph(void);
	static void processNormalKeys(unsigned char, int, int);
	static void processMouseMove(int, int);
};

#endif