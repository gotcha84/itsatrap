#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <gl/freeglut.h>
#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>



/*Those objects holds Shader info. as GLuint*/
GLuint vertShader;
GLuint fragShader;
GLuint gl2Program;

Shader::Shader(void)
{
}
Shader::Shader(string vertFile, string fragFile) {
	shaderInit();

	// create vert+frag shaders
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

	// read source code
	readShaderSource(vert, vertFile);
	readShaderSource(frag, fragFile);

	// compile shaders
	GLint compiled;
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(vert);
	if (compiled == GL_FALSE) {
		cout << "Compile error in vertex shader." << endl;
	}

	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(frag);
	if (compiled == GL_FALSE) {
		cout << "Compile error in fragment shader." << endl;
	}

	// create program and attach shaders
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);

	// attached to program, don't need refs to shaders anymore
	glDeleteShader(vert);
	glDeleteShader(frag);

	// link program
	GLint linked = GL_TRUE;
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	printProgramInfoLog(prog);
	if (linked == GL_FALSE) {
		cout << "Link error in shader program." << endl;
	}

	m_prog = prog;
}


Shader::~Shader(void)
{
}


//reseting GLSL()
int Shader::shaderInit(void)
{
  int error = 0;


  return error;
}

void Shader::loadShader(string vertFile, string fragFile) {
	shaderInit();
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}

	// create vert+frag shaders
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

	// read source code
	readShaderSource(vert, vertFile);
	readShaderSource(frag, fragFile);

	// compile shaders
	GLint compiled;
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(vert);
	if (compiled == GL_FALSE) {
		cout << "Compile error in vertex shader." << endl;
	}

	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(frag);
	if (compiled == GL_FALSE) {
		cout << "Compile error in fragment shader." << endl;
	}

	// create program and attach shaders
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);

	// attached to program, don't need refs to shaders anymore
	glDeleteShader(vert);
	glDeleteShader(frag);

	// link program
	GLint linked = GL_TRUE;
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	printProgramInfoLog(prog);
	if (linked == GL_FALSE) {
		cout << "Link error in shader program." << endl;
	}

	m_prog = prog;
}

/*
 *	Thies function reads a souce program of shader into memory 
 */
int Shader::readShaderSource(GLuint shader, string file)
{
	FILE *fp;
	const GLchar *source;
	GLsizei length;
	int ret;

	/* Opens input file */
	fp = fopen(file.c_str(), "rb");
	if (fp == NULL) {
		perror(file.c_str());
		return -1;
	}

	/* This is getting file size*/
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);

	/* Allocate memory for the file size which is gotten above */
	source = (GLchar *)malloc(length);
	if (source == NULL) {
		fprintf(stderr, "Could not allocate read buffer.\n");
		return -1;
	}

	/*This reads the file from the first lien*/
	fseek(fp, 0L, SEEK_SET);
	ret = fread((void *)source, 1, length, fp) != (size_t)length;
	fclose(fp);

	/*This is pushing shader's source program into shader object*/
	if (ret)
		fprintf(stderr, "Could not read file: %s.\n", file);
	else
		glShaderSource(shader, 1, &source, &length);

	/* Free the memory which allocated earlier */
	free((void *)source);

	return ret;
}

/*
* This willprint out shader's information
* This may not be needed
*/
void Shader::printShaderInfoLog(GLuint shader)
{
	GLsizei bufSize;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		GLchar *infoLog;

		infoLog = (GLchar *)malloc(bufSize);
		if (infoLog != NULL) {
			GLsizei length;

			glGetShaderInfoLog(shader, bufSize, &length, infoLog);
			fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
			free(infoLog);
		}
		else
			fprintf(stderr, "Could not allocate InfoLog buffer.\n");
	}
}

/*
* This shows program information.
* This may not be needed
*/
void Shader::printProgramInfoLog(GLuint program)
{
	GLsizei bufSize;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		GLchar *infoLog;

		infoLog = (GLchar *)malloc(bufSize);
		if (infoLog != NULL) {
			GLsizei length;

			glGetProgramInfoLog(program, bufSize, &length, infoLog);
			fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
			free(infoLog);
		}
		else
			fprintf(stderr, "Could not allocate InfoLog buffer.\n");
	}
}

/*This function require fragment shder info and vertex shader info files*/
GLuint Shader::lightShader(char* fragFile, char* vertFile){
	/* These getting shader result of compile and link */
	GLint compiled, linked;

	/* Default setting */
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	/* Default setting of light shader */
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	/* reseting shader */
	if (shaderInit()) exit(1);

	/* Creating shader objrcts for vertex and fragment shader */
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	/* reading shader program from the source */
	if (readShaderSource(vertShader, vertFile)) exit(1);
	if (readShaderSource(fragShader, fragFile)) exit(1);

	/* Compiling vertex shader sorce program */
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(vertShader);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in vertex shader.\n");
		//exit(1);
	}

	/* Compiling fragment shader sorce program */
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(fragShader);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in fragment shader.\n");
		//exit(1);
	}

	/* Creating program object */
	gl2Program = glCreateProgram();

	/* attaching chader object to the program object */
	glAttachShader(gl2Program, vertShader);
	glAttachShader(gl2Program, fragShader);

	/* deleting shader object since those are already attached to program object*/
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	/* make a link of shader program  */
	glLinkProgram(gl2Program);
	glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
	printProgramInfoLog(gl2Program);
	if (linked == GL_FALSE) {
		fprintf(stderr, "Link error.\n");
		//exit(1);
	}

	/* applying shader */
	return gl2Program;
}

GLuint Shader::getShader() {
	return m_prog;
}