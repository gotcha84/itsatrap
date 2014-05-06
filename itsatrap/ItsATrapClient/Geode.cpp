#include "Geode.h"

namespace sg {

	Geode::Geode() {
		color = glm::vec3(1,0,0);
		texture = new Texture();
		
		//texturePPM2 = texture->loadTexture("building1.ppm");
		shader = new Shader();
		light = shader->lightShader("phongandtexture.frag", "phongandtexture.vert");

		//this let the shaders on texture, so when you try to use shader on texture you add this on eline of a code
		glUniform1i(glGetUniformLocation(light, "coke-label.ppm"), 0);
		texturePPM = texture->loadTexture("coke-label.ppm");
		glUniform1i(glGetUniformLocation(light, "building1.ppm"), 0);
		texturePPM2 = texture->loadTexture("building1.ppm");
	}

	Geode::~Geode() {
		delete texture;
		texture = nullptr;
		delete shader;
		shader = nullptr;
	}

	void Geode::print() {
		cout << "(" << this->getObjectID() << " Geode: " << this->getName() << ")";
	}

	// TODO : implement base class
	void Geode::calculateBoundingBox() {

	}

	// TODO : implement base class
	bool Geode::isInside(glm::vec3 goTo) {
		cout << "geode isInside" << endl;
		return false;
	}

	// void Geode::collidesWith(Geode b) {

	// }
	
	void Geode::setMaterial() {

	}
}

