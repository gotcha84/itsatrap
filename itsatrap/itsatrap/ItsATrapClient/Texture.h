#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

namespace sg {

	class Texture {
		public:

			string texture_building;

			Texture();
			~Texture();
			
			GLuint loadTexture(char* the_texture);
			unsigned char* loadPPM(char* filename, int& width, int& height);

	};
	

}

#endif