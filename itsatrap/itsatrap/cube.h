#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "Matrix4.h"

class Cube
{
	public:
    Matrix4 matrix;                 // model matrix
    double angle;                   // rotation angle [degrees]
	double walk_lateral_factor;		// how fast you wana walk
	double walk_linear_factor;
	
	public:
    Cube();   // Constructor
    Matrix4& getMatrix();
		void setMatrix(Matrix4&);
		double getAngle();
		void setAngle(double);
    void spin(double);      // spin cube [degrees]
	
};

class Window	  // output window related routines
{
  public:
    static int width, height; 	            // window size

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void processNormalKeys(unsigned char, int, int);
};

#endif