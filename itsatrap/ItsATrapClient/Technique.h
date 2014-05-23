#define INVALID_UNIFORM_LOCATION 0xffffffff

#pragma once
#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include <GL/glew.h>
#include <GL/glut.h>

class Technique {
	public:
		static const unsigned int MAX_BONES = 100;
		GLuint m_boneLocation[MAX_BONES];

		Technique();
		virtual ~Technique();
		virtual bool Init();
		void Enable();

	protected:
		bool AddShader(GLenum ShaderType, const char* pFilename);
		bool Finalize();
		GLint GetUniformLocation(const char* pUniformName);
		GLint GetProgramParam(GLint param);
		GLuint m_shaderProg;    

	private:
		typedef std::list<GLuint> ShaderObjList;
		ShaderObjList m_shaderObjList;
};

#endif