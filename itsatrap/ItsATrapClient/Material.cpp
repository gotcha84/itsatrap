#include "Material.h"


Material::Material() {
	//m_ambient = new GLfloat[3];
	//m_specular = new GLfloat[3];
	//m_diffuse = new GLfloat[3];
	//m_emission = new GLfloat[3];

	//setAmbient(-1.0f, -1.0f, -1.0f, -1.0f);
	//setSpecular(-1.0f, -1.0f, -1.0f, -1.0f);
	//setDiffuse(-1.0f, -1.0f, -1.0f, -1.0f);
	//setEmission(-1.0f, -1.0f, -1.0f, -1.0f);
	//setShininess(-1.0f);
}

Material::Material(GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* emission, GLfloat shininess) {
	//Material();
	
	//m_ambient = new GLfloat[3];
	//m_specular = new GLfloat[3];
	//m_diffuse = new GLfloat[3];
	//m_emission = new GLfloat[3];

	//m_ambient = { 0, 0, 0, 0 };

	setAmbient(ambient[0], ambient[1], ambient[2]);
	setDiffuse(diffuse[0], diffuse[1], diffuse[2]);
	setSpecular(specular[0], specular[1], specular[2]);
	setEmission(emission[0], emission[1], emission[2]);
	setShininess(shininess);
}

Material::~Material() {
	//delete m_ambient;
	//m_ambient = nullptr;
	//
	//delete m_specular;
	//m_specular = nullptr;

	//delete m_diffuse;
	//m_diffuse = nullptr;
	//
	//delete m_emission;
	//m_emission = nullptr;
}

void Material::setAmbient(GLfloat a1, GLfloat a2, GLfloat a3) {
	m_ambient[0] = a1;
	m_ambient[1] = a2;
	m_ambient[2] = a3;
}

void Material::setDiffuse(GLfloat a1, GLfloat a2, GLfloat a3) {
	m_diffuse[0] = a1;
	m_diffuse[1] = a2;
	m_diffuse[2] = a3;
}

void Material::setSpecular(GLfloat a1, GLfloat a2, GLfloat a3) {
	m_specular[0] = a1;
	m_specular[1] = a2;
	m_specular[2] = a3;
}

void Material::setEmission(GLfloat a1, GLfloat a2, GLfloat a3) {
	m_emission[0] = a1;
	m_emission[1] = a2;
	m_emission[2] = a3;
}

void Material::setShininess(GLfloat a1) {
	m_shininess = a1;
}
