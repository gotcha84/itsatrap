#pragma once
#ifndef CUBE_H
#define CUBE_H

//#include "GLee.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "SceneGraph.h"

#include <glm/ext.hpp>


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "MyPlayer.h"

class Cube
{
	public:
    //Matrix4 matrix;                 // model matrix
    double angle;                   // rotation angle [degrees]
	
	MyPlayer m_myPlayer;

    Cube();   // Constructor
    //Matrix4& getMatrix();
	//void setMatrix(Matrix4&);
	double getAngle();
	void setAngle(double);
    void spin(double);      // spin cube [degrees]
	
};

class Window	  // output window related routines
{
  public:
    static int m_width, m_height; 	            // window size
	
	static int m_xMouse, m_yMouse;				// mouse position

	static float m_xAngleChange, m_yAngleChange;

	static float m_xAngleChangeFactor, m_yAngleChangeFactor;
	// might need y total (90 degrees up or down)
	// might need turn factor


    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void displaySceneGraph(void);
	static void processNormalKeys(unsigned char, int, int);
	static void processMouseMove(int, int);
};

#endif