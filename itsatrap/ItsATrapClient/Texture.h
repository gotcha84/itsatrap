#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "glext.h" //Since the tutorial I did used glext.h library, include it

using namespace std;

// Add texture names here
enum Textures {
	ActiveNode,
	InactiveNode,
	BlueNode,
	RedNode,
	Skybox,
	Polynoid,
	Headless,
	Lightning,
	Animus,
	Base,
	BRail,
	Size
};

class Texture {
	public:
		int m_numTex;
		GLuint *m_texID;

		Texture();
		~Texture();

		void initTextures();

		void loadTexture(GLuint id, const char *the_texture);
		unsigned char* loadPPM(const char *filename, int& width, int& height);
};

#endif