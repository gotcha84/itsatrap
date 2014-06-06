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
	RedPolynoid,
	BluePolynoid,
	Caltrop,
	BRail,

	Stunned,
	Slowed,
	Flashed,

	Base_01,
	Base_02,
	Base_03,
	Base_04,
	Base_05,
	Base_06,
	Base_07,
	Base_08,
	Base_09,
	Base_10,

	Base_11,
	Base_12,
	Base_13,
	Base_14,
	Base_15,
	Base_16,
	Base_17,
	Base_18,
	Base_19,
	Base_20,

	Base_21,
	Base_22,
	Base_23,
	Base_24,
	Base_25,
	Base_26,
	Base_27,
	
	Bridge,
	Float,

	Ext_01,
	Ext_02, 
	Ext_03,
	Ext_04,
	Ext_05,

	Wedge_01,
	Rail_01,
	Metal_01,
	ERail_01,
	Climbable_01,

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