#include "Vector3.h"

#include <math.h>
#include <iostream> // to print to stdout
#include <sstream> // to convert float to string
#include <iomanip> // to round floats
#include <string>

using namespace std;

Vector3::Vector3() 
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}


Vector3::Vector3(float x0, float y0, float z0) 
{
	x = x0;
	y = y0;
	z = z0;
}

void Vector3::set(float x0, float y0, float z0) 
{
	x = x0;
	y = y0;
	z = z0;
}

void Vector3::setX(float x0) {
	x = x0;
}

void Vector3::setY(float y0) {
	y = y0;
}

void Vector3::setZ(float z0) {
	z = z0;
}

float Vector3::get(int index)
{
	if (index == 0)
		return x;
	else if (index == 1)
		return y;
	else
		return z;
}

float Vector3::getX()
{
	return x;
}

float Vector3::getY()
{
	return y;
}

float Vector3::getZ()
{
	return z;
}


float Vector3::operator[](const int i)
{
	return this->get(i);
}

void Vector3::add(Vector3 &a)
{
	x+=a.x;
	y+=a.y;
	z+=a.z;
}

void Vector3::add(Vector3 &a, Vector3 &b)
{
	x = a.x + b.x;
	y = a.y + b.y;
	z = a.z + b.z;
}

Vector3 Vector3::operator+(const Vector3 &other) const
{
	return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

void Vector3::subtract(Vector3 &a)
{
	x-=a.x;
	y-=a.y;
	z-=a.z;
}

float Vector3::angle(Vector3* a)
{
	return acos(a->dot(*this)/(a->magnitude()*this->magnitude()));
}
/*
void Vector3::multiply(Matrix4 &a)
{
	Vector4 tmp = Vector4(this->x, this->y, this->z);
	Vector4 tmp2 = a.multiply(tmp);
	this->set(tmp2.getX(), tmp2.getY(), tmp2.getZ());
	
}
*/
void Vector3::subtract(Vector3 &a, Vector3 &b)
{
	x = a.x - b.x;
	y = a.y - b.y;
	z = a.z - b.z;
}

Vector3 Vector3::operator-(const Vector3 &other) const
{
	return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

void Vector3::divide(float a) 
{
	x /= a;
	y /= a;
	z /= a;
}

void Vector3::negate()
{
	x = -x;
	y = -y;
	z = -z;
}

void Vector3::negate(Vector3 &a)
{
	x = -a.x;
	y = -a.y;
	z = -a.z;
}

void Vector3::scale(float s)
{
	x*=s;
	y*=s;
	z*=s;
}

void Vector3::scale(float s, Vector3 &a)
{
	x = s * a.x;
	y = s * a.y;
	z = s * a.z;
}

float Vector3::dot(Vector3 &a)
{
	return x*a.x + y*a.y + z*a.z;
}

void Vector3::cross(Vector3* b)
{
	float i = (this->y*b->z) - (this->z*b->y);
	float j = (this->z*b->x) - (this->x*b->z);
	float k = (this->x*b->y) - (this->y*b->x);
	this->set(i, j, k);
}

void Vector3::cross(Vector3 &a, Vector3 &b)
{
	//cout << "\ncrossing";
	//cout << a.x << "," << a.y << "," << a.z << "," << b.x << "," << b.y << "," << b.z << '\n';
	x = (a.y*b.z) - (a.z*b.y);
	y = (a.z*b.x) - (a.x*b.z);
	z = (a.x*b.y) - (a.y*b.x);
}

float Vector3::magnitude()
{

	//cout << "mag: " << x << ',' << y << ',' << z;
	return sqrt(x*x + y*y + z*z);
}

void Vector3::normalize()
{
	scale(float(1.0)/magnitude());
}
	
void Vector3::print()
{
	//cout << '\n';
	cout << setprecision(2) << fixed;
	cout << "(" << this->x << ", " << this->y << ", " << this->z << ")\n";
}

string Vector3::toString()
{
	ostringstream ss;
	ss << "(" << this->x << ", " << this->y << ", " << this->z << ")";
	return ss.str();
}

float Vector3::dist(Vector3 &a, Vector3 &b)
{
	float x2 = a.x*a.x + b.x*b.x;
	float y2 = a.y*a.y + b.y*b.y;
	float z2 = a.z*a.z + b.z*b.z;
	return sqrt(x2+y2+z2);
}