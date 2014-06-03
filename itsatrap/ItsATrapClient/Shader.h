#pragma once
#ifndef SHADER_H
#define SHADER_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//Since the tutorial I did used glext.h library, include it
#include "glext.h"

#endif
using namespace std;
class Shader
{
	public:
		Shader();
		Shader(string vertFile, string fragFile);
		~Shader();

		GLuint vertShader;
		GLuint fragShader;
		GLuint gl2Program;

		GLuint m_prog;

		int shaderInit(void);
		int readShaderSource(GLuint shader, string file);
		void printShaderInfoLog(GLuint shader);
		void printProgramInfoLog(GLuint program);
		GLuint lightShader(char* fragFile, char* vertFile);
		GLuint getShader();
};
