#include "Matrix4.h"

#include <math.h>
#include <iostream> // to print to stdout
#include <sstream> // to convert float to string
#include <iomanip> // to round floats
#include <string>
#include <GL/glut.h>

using namespace std;

Matrix4::Matrix4()
{
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			m[i][j] = 0;
		}
	}

	m[3][3] = 1;
}

Matrix4::Matrix4(Vector3 a) {
	m[0][0] = a.getX();
	m[1][0] = a.getY();
	m[2][0] = a.getZ();
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			m[i][j] = 0;
		}
	}
	m[3][0] = 1;
}

Matrix4::Matrix4(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33 ) {
	
	/*
	// row major ordering
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
	*/

	// column major ordering
	m[0][0] = m00;
	m[1][0] = m01;
	m[2][0] = m02;
	m[3][0] = m03;
	m[0][1] = m10;
	m[1][1] = m11;
	m[2][1] = m12;
	m[3][1] = m13;
	m[0][2] = m20;
	m[1][2] = m21;
	m[2][2] = m22;
	m[3][2] = m23;
	m[0][3] = m30;
	m[1][3] = m31;
	m[2][3] = m32;
	m[3][3] = m33;
}

void Matrix4::setRowMajorMatrix(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33 ) {
	
	// row major ordering
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

void Matrix4::setColMajorMatrix(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33 ) {
	
	// column major ordering
	m[0][0] = m00;
	m[1][0] = m01;
	m[2][0] = m02;
	m[3][0] = m03;
	m[0][1] = m10;
	m[1][1] = m11;
	m[2][1] = m12;
	m[3][1] = m13;
	m[0][2] = m20;
	m[1][2] = m21;
	m[2][2] = m22;
	m[3][2] = m23;
	m[0][3] = m30;
	m[1][3] = m31;
	m[2][3] = m32;
	m[3][3] = m33;
}

void Matrix4::inverse() {
	
	// calculate determinant
	double det = m[0][0]*m[1][1]*m[2][2]*m[3][3]+m[0][0]*m[1][2]*m[2][3]*m[3][1]+m[0][0]*m[1][3]*m[2][1]*m[3][2]
		+m[0][1]*m[1][0]*m[2][3]*m[3][2]+m[0][1]*m[1][2]*m[2][0]*m[3][3]+m[0][1]*m[1][3]*m[2][2]*m[3][0]
		+m[0][2]*m[1][0]*m[2][1]*m[3][3]+m[0][2]*m[1][1]*m[2][3]*m[3][0]+m[0][2]*m[1][3]*m[2][0]*m[3][1]
		+m[0][3]*m[1][0]*m[2][2]*m[3][1]+m[0][3]*m[1][1]*m[2][0]*m[3][2]+m[0][3]*m[1][2]*m[2][1]*m[3][0]
		-m[0][0]*m[1][1]*m[2][3]*m[3][2]-m[0][0]*m[1][2]*m[2][1]*m[3][3]-m[0][0]*m[1][3]*m[2][2]*m[3][1]
		-m[0][1]*m[1][0]*m[2][2]*m[3][3]-m[0][1]*m[1][2]*m[2][3]*m[3][0]-m[0][1]*m[1][3]*m[2][0]*m[3][2]
		-m[0][2]*m[1][0]*m[2][3]*m[3][1]-m[0][2]*m[1][1]*m[2][0]*m[3][3]-m[0][2]*m[1][3]*m[2][1]*m[3][0]
		-m[0][3]*m[1][0]*m[2][1]*m[3][2]-m[0][3]*m[1][1]*m[2][2]*m[3][0]-m[0][3]*m[1][2]*m[2][0]*m[3][1];
 
	// find inverse
	double b[4][4];
 
	b[0][0] = m[1][1]*m[2][2]*m[3][3]+m[1][2]*m[2][3]*m[3][1]+m[1][3]*m[2][1]*m[3][2]-m[1][1]*m[2][3]*m[3][2]-m[1][2]*m[2][1]*m[3][3]-m[1][3]*m[2][2]*m[3][1];
	b[0][1] = m[0][1]*m[2][3]*m[3][2]+m[0][2]*m[2][1]*m[3][3]+m[0][3]*m[2][2]*m[3][1]-m[0][1]*m[2][2]*m[3][3]-m[0][2]*m[2][3]*m[3][1]-m[0][3]*m[2][1]*m[3][2];
	b[0][2] = m[0][1]*m[1][2]*m[3][3]+m[0][2]*m[1][3]*m[3][1]+m[0][3]*m[1][1]*m[3][2]-m[0][1]*m[1][3]*m[3][2]-m[0][2]*m[1][1]*m[3][3]-m[0][3]*m[1][2]*m[3][1];
	b[0][3] = m[0][1]*m[1][3]*m[2][2]+m[0][2]*m[1][1]*m[2][3]+m[0][3]*m[1][2]*m[2][1]-m[0][1]*m[1][2]*m[2][3]-m[0][2]*m[1][3]*m[2][1]-m[0][3]*m[1][1]*m[2][2];
	b[1][0] = m[1][0]*m[2][3]*m[3][2]+m[1][2]*m[2][0]*m[3][3]+m[1][3]*m[2][2]*m[3][0]-m[1][0]*m[2][2]*m[3][3]-m[1][2]*m[2][3]*m[3][0]-m[1][3]*m[2][0]*m[3][2];
	b[1][1] = m[0][0]*m[2][2]*m[3][3]+m[0][2]*m[2][3]*m[3][0]+m[0][3]*m[2][0]*m[3][2]-m[0][0]*m[2][3]*m[3][2]-m[0][2]*m[2][0]*m[3][3]-m[0][3]*m[2][2]*m[3][0];
	b[1][2] = m[0][0]*m[1][3]*m[3][2]+m[0][2]*m[1][0]*m[3][3]+m[0][3]*m[1][2]*m[3][0]-m[0][0]*m[1][2]*m[3][3]-m[0][2]*m[1][3]*m[3][0]-m[0][3]*m[1][0]*m[3][2];
	b[1][3] = m[0][0]*m[1][2]*m[2][3]+m[0][2]*m[1][3]*m[2][0]+m[0][3]*m[1][0]*m[2][2]-m[0][0]*m[1][3]*m[2][2]-m[0][2]*m[1][0]*m[2][3]-m[0][3]*m[1][2]*m[2][0];
	b[2][0] = m[1][0]*m[2][1]*m[3][3]+m[1][1]*m[2][3]*m[3][0]+m[1][3]*m[2][0]*m[3][1]-m[1][0]*m[2][3]*m[3][1]-m[1][1]*m[2][0]*m[3][3]-m[1][3]*m[2][1]*m[3][0];
	b[2][1] = m[0][0]*m[2][3]*m[3][1]+m[0][1]*m[2][0]*m[3][3]+m[0][3]*m[2][1]*m[3][0]-m[0][0]*m[2][1]*m[3][3]-m[0][1]*m[2][3]*m[3][0]-m[0][3]*m[2][0]*m[3][1];
	b[2][2] = m[0][0]*m[1][1]*m[3][3]+m[0][1]*m[1][3]*m[3][0]+m[0][3]*m[1][0]*m[3][1]-m[0][0]*m[1][3]*m[3][1]-m[0][1]*m[1][0]*m[3][3]-m[0][3]*m[1][1]*m[3][0];
	b[2][3] = m[0][0]*m[1][3]*m[2][1]+m[0][1]*m[1][0]*m[2][3]+m[0][3]*m[1][1]*m[2][0]-m[0][0]*m[1][1]*m[2][3]-m[0][1]*m[1][3]*m[2][0]-m[0][3]*m[1][0]*m[2][1];
	b[3][0] = m[1][0]*m[2][2]*m[3][1]+m[1][1]*m[2][0]*m[3][2]+m[1][2]*m[2][1]*m[3][0]-m[1][0]*m[2][1]*m[3][2]-m[1][1]*m[2][2]*m[3][0]-m[1][2]*m[2][0]*m[3][1];
	b[3][1] = m[0][0]*m[2][1]*m[3][2]+m[0][1]*m[2][2]*m[3][0]+m[0][2]*m[2][0]*m[3][1]-m[0][0]*m[2][2]*m[3][1]-m[0][1]*m[2][0]*m[3][2]-m[0][2]*m[2][1]*m[3][0];
	b[3][2] = m[0][0]*m[1][2]*m[3][1]+m[0][1]*m[1][0]*m[3][2]+m[0][2]*m[1][1]*m[3][0]-m[0][0]*m[1][1]*m[3][2]-m[0][1]*m[1][2]*m[3][0]-m[0][2]*m[1][0]*m[3][1];
	b[3][3] = m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[0][2]*m[1][0]*m[2][1]-m[0][0]*m[1][2]*m[2][1]-m[0][1]*m[1][0]*m[2][2]-m[0][2]*m[1][1]*m[2][0];
	
	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++) {
			m[i][j] = b[i][j]/det;
		}
	}
 
}

double Matrix4::operator() (const int i, const int j) {
	return get(i, j);
}

Matrix4::Matrix4(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, 
								 double w, double x, double y, double z)
{
	// col major order
	m[0][0] = a.x;
	m[1][0] = a.y;
	m[2][0] = a.z;

	m[0][1] = b.x;
	m[1][1] = b.y;
	m[2][1] = b.z;

	m[0][2] = c.x;
	m[1][2] = c.y;
	m[2][2] = c.z;

	m[0][3] = d.x;
	m[1][3] = d.y;
	m[2][3] = d.z;

	m[3][0] = w;
	m[3][1] = x;
	m[3][2] = y;
	m[3][3] = z;
}

Matrix4::Matrix4(Matrix4 &a)
{
	m[0][0] = a.m[0][0];
	m[0][1] = a.m[0][1];
	m[0][2] = a.m[0][2];
	m[0][3] = a.m[0][3];
	m[1][0] = a.m[1][0];
	m[1][1] = a.m[1][1];
	m[1][2] = a.m[1][2];
	m[1][3] = a.m[1][3];
	m[2][0] = a.m[2][0];
	m[2][1] = a.m[2][1];
	m[2][2] = a.m[2][2];
	m[2][3] = a.m[2][3];
	m[3][0] = a.m[3][0];
	m[3][1] = a.m[3][1];
	m[3][2] = a.m[3][2];
	m[3][3] = a.m[3][3];
}

double* Matrix4::getPointer()
{
	return &m[0][0];
}

double Matrix4::get(int index1, int index2)
{
	return m[index1][index2];
}

void Matrix4::set(int index1, int index2, float a)
{
	m[index1][index2] = a;
}

void Matrix4::identity()
{
	double ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

	for (int i=0; i<4; ++i) {
		for (int j=0; j<4; ++j) {
			m[i][j] = ident[i][j];
		}
	}
}

Matrix4 Matrix4::multiply(Matrix4 &a)
{
	double n[4][4];
	double temp_sum;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp_sum = 0;
			for (int x = 0; x < 4; ++x)
			{
				temp_sum+=m[i][x]*a.m[x][j];
			}
			n[j][i] = temp_sum;
		}
	}

	return Matrix4(
		n[0][0], n[0][1], n[0][2], n[0][3], 
		n[1][0], n[1][1], n[1][2], n[1][3], 
		n[2][0], n[2][1], n[2][2], n[2][3], 
		n[3][0], n[3][1], n[3][2], n[3][3]
	);
}

Vector4 Matrix4::multiply(Vector4 &a)
{
	float n[4];
	float temp_sum;

	for (int i = 0; i < 4; ++i) {
		temp_sum = 0;
		for (int j = 0; j < 4; ++j) {
			temp_sum+=m[i][j]*a.get(j);
		}
		n[i] = temp_sum;
	}

	return Vector4(n[0], n[1], n[2], n[3]);
}

void Matrix4::invertOrtho()
{
	this->transpose();
}

void Matrix4::rotateX(double angle)
{
	Matrix4 n = Matrix4(
		1, 0, 0, 0, 
		0, cos(angle), -sin(angle), 0,  
		0, sin(angle), cos(angle), 0, 
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = this->multiply(n);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::rotateWindowX(double angle)
{
	Matrix4 n = Matrix4(
		1, 0, 0, 0, 
		0, cos(angle), -sin(angle), 0,  
		0, sin(angle), cos(angle), 0, 
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = n.multiply(*this);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}
// angle in radians
void Matrix4::rotateY(double angle)
{
	Matrix4 n = Matrix4(
		cos(angle), 0, sin(angle), 0, 
		0, 1, 0, 0, 
		-sin(angle), 0, cos(angle), 0, 
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = this->multiply(n);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::rotateWindowY(double angle)
{
	Matrix4 n = Matrix4(
		cos(angle), 0, sin(angle), 0, 
		0, 1, 0, 0, 
		-sin(angle), 0, cos(angle), 0, 
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = n.multiply(*this);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::rotateZ(double angle)
{
	Matrix4 n = Matrix4(
		cos(angle), -sin(angle), 0, 0, 
		sin(angle), cos(angle), 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = this->multiply(n);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::rotateWindowZ(double angle)
{
	Matrix4 n = Matrix4(
		cos(angle), -sin(angle), 0, 0, 
		sin(angle), cos(angle), 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = n.multiply(*this);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::copyRot(Matrix4 *a)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->set(i, j, a->get(i, j));
		}
	}
}

Matrix4& Matrix4::rotate(double angle, float a, float b, float c)
{
	Vector3 tmp = Vector3(a, b, c);	
	this->rotate(angle, tmp);
	return *this;
}

void Matrix4::rotate(double angle, Vector3 &a)
{
	// Transformations slide 44
	Vector3 b = Vector3(a.getX(), a.getY(), a.getZ());
	b.normalize();

	Matrix4 rot = Matrix4(
		b.x*b.x + cos(angle)*(1-b.x*b.x),
		b.x*b.y*(1-cos(angle)) - b.z*sin(angle),
		b.x*b.z*(1-cos(angle)) + b.y*sin(angle),
		0,

		b.x*b.y*(1-cos(angle)) + b.z*sin(angle),
		b.y*b.y + cos(angle)*(1-b.y*b.y),
		b.y*b.z*(1-cos(angle)) - b.x*sin(angle),
		0,

		b.x*b.z*(1-cos(angle)) - b.y*sin(angle),
		b.y*b.z*(1-cos(angle)) + b.x*sin(angle),
		b.z*b.z + cos(angle)*(1-b.z*b.z),
		0,

		0,
		0,
		0,
		1
	);
	rot.transpose(); // make row major

	Matrix4 result = this->multiply(rot);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::scale(double a, double b, double c)
{
	Matrix4 n = Matrix4(
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		0, 0, 0, 1
	);
	n.transpose(); // make row major

	Matrix4 result = n.multiply(*this);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = result.get(i, j);
		}
	}
}

void Matrix4::translate(double a, double b, double c)
{
	/*

	Vector4 n = Vector4(a, b, c, 1);
	n = this->multiply(n);

	m[0][3] = n.getX();
	m[1][3] = n.getY();
	m[2][3] = n.getZ();
	*/
	
	m[0][3] += a;
	m[1][3] += b;
	m[2][3] += c;
	
}

void Matrix4::print()
{

	for (int i=0; i<4; i++)
	{
		cout << setprecision(2) << fixed;

		Matrix4 tpose = Matrix4(*this);
		tpose.transpose();

		cout << "[";
		for (int j=0; j<4; j++)
		{
			cout << ("%f", tpose.m[i][j]);
			if (j != 3)
				cout << "\t";
		}

		cout << "]\n";
	}
	cout << "\n";
}

void Matrix4::transpose()
{
	double temp[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp[i][j] = m[j][i];
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = temp[i][j];
		}
	}

}

Matrix4 Matrix4::createCameraMatrix(Vector3 &e, Vector3 &d, Vector3 &up) {
	Vector3 zc = e - d;
	float mag_zc = zc.magnitude();
	zc.divide(mag_zc);

	Vector3 xc;
	xc.cross(up, zc);
	float mag_xc = xc.magnitude();
	xc.divide(mag_xc);
	
	Vector3 yc;
	yc.cross(zc, xc);
	return Matrix4(xc, yc, zc, e, 0.0, 0.0, 0.0, 1.0);
}

GLfloat* Matrix4::getGLMatrix() {
	
	//if (read == false) {
	//	read = true;
		//for (int i = 0; i < 4; i++) { // the hell is up with this line
			//for (int j = 0; j < 4; j++) {
				//cout << c.get(j, i);
			//}

		//}
	//}	

	int i;

	GLfloat retval[16];

	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			retval[(i*4)+j] = m[j][i];
		}
	}

	return retval;
}

/** Rotates the matrix according to a fictitious trackball, placed in
    the middle of the given window.
    The trackball is approximated by a Gaussian curve.
    The trackball coordinate system is: x=right, y=up, z=to viewer<BR>
    The origin of the mouse coordinates zero (0,0) is considered to be top left.
  @param width, height  window size in pixels
  @param fromX, fromY   mouse starting position in pixels
  @param toX, toY       mouse end position in pixels
*/
Matrix4 Matrix4::trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY)
{
  const float TRACKBALL_SIZE = 1.3f;              // virtual trackball size (empirical value)
  Matrix4 mInv;                                   // inverse of ObjectView matrix
  Vector3 v1, v2;                                 // mouse drag positions in normalized 3D space
  float smallSize;                                // smaller window size between width and height
  float halfWidth, halfHeight;                    // half window sizes
  float angle;                                    // rotational angle
  float d;                                        // distance

  // Compute mouse coordinates in window and normalized to -1..1
  // ((0,0)=window center, (-1,-1) = bottom left, (1,1) = top right)
  halfWidth   = (float)width  / 2.0f;
  halfHeight  = (float)height / 2.0f;
  smallSize   = (halfWidth < halfHeight) ? halfWidth : halfHeight;
  v1.setX(((float)fromX - halfWidth)  / smallSize);
  v1.setY(((float)(height-fromY) - halfHeight) / smallSize);
  v2.setX(((float)toX   - halfWidth)  / smallSize);
  v2.setY(((float)(height-toY)   - halfHeight) / smallSize);

  // Compute z-coordinates on Gaussian trackball:
  d       = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
  v1.setZ(expf(-TRACKBALL_SIZE * d * d));
  d       = sqrtf(v2[0] * v2[0] + v2[1] * v2[1]);
  v2.setZ(expf(-TRACKBALL_SIZE * d * d));
  /*
  v1[0]       = ((float)fromX - halfWidth)  / smallSize;
  v1[1]       = ((float)(height-fromY) - halfHeight) / smallSize;
  v2[0]       = ((float)toX   - halfWidth)  / smallSize;
  v2[1]       = ((float)(height-toY)   - halfHeight) / smallSize;

  // Compute z-coordinates on Gaussian trackball:
  d       = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
  v1[2]   = expf(-TRACKBALL_SIZE * d * d);
  d       = sqrtf(v2[0] * v2[0] + v2[1] * v2[1]);
  v2[2]   = expf(-TRACKBALL_SIZE * d * d);
  */
  // Compute rotational angle:
  angle = v1.angle(&v2);                          // angle = angle between v1 and v2

  // Compute rotational axis:
  v2.cross(&v1);                                  // v2 = v2 x v1 (cross product)

  // Convert axis coordinates (v2) from WCS to OCS:
  mInv.identity();
  mInv.copyRot(this);                             // copy rotational part of mv to mInv
  mInv.invertOrtho();                             // invert orthogonal matrix mInv
  // MIGHT BE 1 BELOW
  Vector4 tmp = Vector4(v2.getX(), v2.getY(), v2.getZ(), 1);
  Vector4 tmp2 = mInv.multiply(tmp);
  v2 = Vector3(tmp2.getX(), tmp2.getY(), tmp2.getZ()); 
  //v2.multiply(&mInv);                             // v2 = v2 x mInv (matrix multiplication)
  v2.normalize();                                 // normalize v2 before rotation

  // Perform acutal model view matrix modification:
  return rotate(-angle, v2[0], v2[1], v2[2]);      // rotate model view matrix
}
