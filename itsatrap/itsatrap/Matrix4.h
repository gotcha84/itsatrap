#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "Vector3.h"
#include "Vector4.h"

#include <string>
#include <GL/glut.h>

using namespace std;

class Vector3;

class Matrix4
{
	public:
		double m[4][4]; // matrix elements
		// given
		Matrix4(); // constructor
		Matrix4(Vector3);
		void inverse();
		// given
		// Constructor with 16 parameters to set the values of the matrix
		Matrix4(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
		void Matrix4::setRowMajorMatrix(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
		void Matrix4::setColMajorMatrix(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
		Matrix4(Vector3&, Vector3&, Vector3&, Vector3&, double, double, double, double);
		Matrix4(Matrix4&);

		double operator() (const int, const int);
		// given
		// return pointer to matrix elements
		double* getPointer(); // given
		// 'get(x,y)' function to read any matrix element
		double get(int, int);
		void set(int, int, float);

		// given
		// create identity matrix
		void identity();
		// Multiply (matrix-times-matrix)
		Matrix4 multiply(Matrix4&);
		// Multiply (matrix-times-vector)
		Vector4 multiply(Vector4&);
		void invertOrtho();
		// Make a rotation matrix about the x axis
		void rotateX(double);
		void rotateWindowX(double);
		// given
		// Make a rotation matrix about the y axis
		void rotateY(double);
		void rotateWindowY(double);
		// Make a rotation matrix about the z axis
		void rotateZ(double);
		void rotateWindowZ(double);
		// Make a rotation matrix about an arbitrary (unit) axis
		void rotate(double, Vector3&);
		Matrix4& rotate(double, float, float, float);
		void copyRot(Matrix4*);
		// Make a non-uniform scaling matrix
		void scale(double, double, double);

		// Make a translation matrix
		void translate(double, double, double);

		// Print the matrix (display all 16 matrix components numerically on the screen in a 4x4 array)
		void print();

		// Transpose the matrix
		void transpose();

		static Matrix4 Matrix4::createCameraMatrix(Vector3& e, Vector3& d, Vector3& up);
		GLfloat* Matrix4::getGLMatrix();

		Matrix4 Matrix4::trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY);
};

#endif