#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include <GL/glut.h>
//Since the tutorial I did used glext.h library, include it
#include "glext.h"
#include <glm/gtc/type_ptr.hpp>

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