#define INVALID_UNIFORM_LOCATION 0xffffffff
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifdef WIN32
	#define SNPRINTF _snprintf_s
#else
	#define SNPRINTF snprintf
#endif

#pragma once
#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <list>

typedef std::list<GLuint> ShaderObjList;

class Technique {
	public:
		static const unsigned int MAX_BONES = 100;
		GLuint m_boneLocation[MAX_BONES];
		GLuint m_WVPLocation;
		GLuint m_WorldMatrixLocation;

		Technique();
		~Technique();
		bool baseInit();
		bool Init();
		void Enable();

		//void SetWVP(const glm::mat4& WVP);
		//void SetWorldMatrix(const glm::mat4& WorldInverse);

	protected:
		bool AddShader(GLenum ShaderType, const char* pFilename);
		bool Finalize();
		GLint GetUniformLocation(const char* pUniformName);
		GLint GetProgramParam(GLint param);
		GLuint m_shaderProg;    

	private:
		ShaderObjList m_shaderObjList;
};

#endif