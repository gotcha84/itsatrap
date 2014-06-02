#include "Material.h"


Material::Material() {
	m_ambient = new GLfloat[4];
	m_specular = new GLfloat[4];
	m_diffuse = new GLfloat[4];
	m_emission = new GLfloat[4];

	setAmbient(-1.0f, -1.0f, -1.0f, -1.0f);
	setSpecular(-1.0f, -1.0f, -1.0f, -1.0f);
	setDiffuse(-1.0f, -1.0f, -1.0f, -1.0f);
	setEmission(-1.0f, -1.0f, -1.0f, -1.0f);
	setShininess(-1.0f);
}

Material::~Material() {
	delete m_ambient;
	m_ambient = nullptr;
	
	delete m_specular;
	m_specular = nullptr;
	
	delete m_diffuse;
	m_diffuse = nullptr;
	
	delete m_emission;
	m_emission = nullptr;
}

void Material::setAmbient(float a1, float a2, float a3, float a4) {
	m_ambient[0] = (GLfloat)a1;
	m_ambient[1] = (GLfloat)a2;
	m_ambient[2] = (GLfloat)a3;
	m_ambient[3] = (GLfloat)a4;
}

void Material::setDiffuse(float a1, float a2, float a3, float a4) {
	m_diffuse[0] = (GLfloat)a1;
	m_diffuse[1] = (GLfloat)a2;
	m_diffuse[2] = (GLfloat)a3;
	m_diffuse[3] = (GLfloat)a4;
}

void Material::setSpecular(float a1, float a2, float a3, float a4) {
	m_specular[0] = (GLfloat)a1;
	m_specular[1] = (GLfloat)a2;
	m_specular[2] = (GLfloat)a3;
	m_specular[3] = (GLfloat)a4;
}

void Material::setEmission(float a1, float a2, float a3, float a4) {
	m_emission[0] = (GLfloat)a1;
	m_emission[1] = (GLfloat)a2;
	m_emission[2] = (GLfloat)a3;
	m_emission[3] = (GLfloat)a4;
}

void Material::setShininess(float a1) {
	m_shininess = (GLfloat)a1;
}
