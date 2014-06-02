#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

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