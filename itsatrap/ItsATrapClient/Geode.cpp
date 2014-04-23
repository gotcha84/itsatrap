#include "Geode.h"

Geode::Geode() {
	color = glm::vec3(1,0,0);
	texture = new sg::Texture();
	textuPpm = texture->loadTexture("building1.ppm");
}

Geode::~Geode() {
	delete texture;
	texture = nullptr;
}
