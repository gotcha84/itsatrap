#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "SceneGraph.h"

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Cube
{
	public:
    //Matrix4 matrix;                 // model matrix
    double angle;                   // rotation angle [degrees]
	double walk_x_factor;		// how fast you wana walk
	double walk_z_factor;

	public:
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
    static int width, height; 	            // window size
	static int mouse_X, mouse_Y;				// mouse position

	//static float angle_x_change, angle_y_change;

	//static float angle_x_change_factor, angle_y_change_factor;
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