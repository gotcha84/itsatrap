#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Material {
	public:
		float* m_ambient;
		float* m_diffuse;
		float* m_specular;
		float* m_emission;
		float m_shininess;

		Material();
		~Material();

		void setAmbient(float, float, float, float);
		void setDiffuse(float, float, float, float);
		void setSpecular(float, float, float, float);
		void setEmission(float, float, float, float);
		void setShininess(float);
		
};

#endif