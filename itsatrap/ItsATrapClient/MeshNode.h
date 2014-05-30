#pragma once
#ifndef MESHNODE_H
#define MESHNODE_H

#include "Geode.h"
#include "Mesh.h"
#include "Shader.h"
#include "Utilities.h"

namespace sg {
	class MeshNode : public Geode {
		public:
			Mesh *m_mesh;
			Shader *m_shader;
			GLuint m_shaderID;

			GLuint m_WVPLocation;
			GLuint m_WorldMatrixLocation;
			static const uint MAX_BONES = 100;
			GLuint m_boneLocation[MAX_BONES];

			MeshNode();
			MeshNode(std::string filename);
			~MeshNode();

			void initShader();

			Mesh *getMesh();
			void loadMesh(std::string filename);

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();



			void SetWVP(const glm::mat4 &WVP);
			void SetWorldMatrix(const glm::mat4 &WorldInverse);
			void SetBoneTransform(uint Index, const glm::mat4 &Transform);
			GLint GetUniformLocation(const char* pUniformName);
	};
}

#endif
