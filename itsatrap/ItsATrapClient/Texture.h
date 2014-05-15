#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//Since the tutorial I did used glext.h library, include it
#include "glext.h"

using namespace std;

class Texture {
	public:

		string texture_building;

		Texture();
		~Texture();
			
		GLuint loadTexture(char* the_texture);
		unsigned char* loadPPM(char* filename, int& width, int& height);

};

#endif