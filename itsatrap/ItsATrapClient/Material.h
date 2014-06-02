#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glut.h>

using namespace std;

class Material {
	public:
		GLfloat* m_ambient;
		GLfloat* m_diffuse;
		GLfloat* m_specular;
		GLfloat* m_emission;
		GLfloat m_shininess;

		Material();
		~Material();

		void setAmbient(float, float, float, float);
		void setDiffuse(float, float, float, float);
		void setSpecular(float, float, float, float);
		void setEmission(float, float, float, float);
		void setShininess(float);
		
};

#endif