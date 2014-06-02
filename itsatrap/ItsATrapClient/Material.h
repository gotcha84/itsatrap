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

		//GLfloat m_ambient[3];
		//GLfloat m_diffuse[3];
		//GLfloat m_specular[3];
		//GLfloat m_emission[3];
		//GLfloat m_shininess;


		Material();
		Material(GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* emission, GLfloat shininess);
		~Material();

		void setAmbient(GLfloat, GLfloat, GLfloat);
		void setDiffuse(GLfloat, GLfloat, GLfloat);
		void setSpecular(GLfloat, GLfloat, GLfloat);
		void setEmission(GLfloat, GLfloat, GLfloat);
		void setShininess(GLfloat);
		
};

#endif