#pragma once
#ifndef MESH_H
#define MESH_H

// glut
#include <GL/glew.h>
#include <GL/glut.h>

// glm
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// assimp
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <vector>
#include <iostream>

using namespace std;

struct Vertex {
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;

    Vertex() { }

    Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const glm::vec3 &normal) {
        m_pos = glm::vec3(pos);
        m_tex = glm::vec2(tex);
        m_normal = glm::vec3(normal);
    }
};

class Mesh {
	public:
		Mesh();
		~Mesh();

		bool LoadMesh(const string& Filename);
		void draw();

	//private:
		bool InitFromScene(const aiScene* pScene, const string& Filename);
		void InitMesh(unsigned int Index, const aiMesh* paiMesh);
		//bool InitMaterials(const aiScene* pScene, const string& Filename);
		void Clear();

		struct MeshEntry {
			vector<float> verts;
			vector<unsigned int> inds;

			MeshEntry();
			~MeshEntry();

			void Init(const vector<Vertex> &Vertices, const vector<unsigned int> &Indices);

			GLuint VB;
			GLuint IB;
			unsigned int NumIndices;
			unsigned int MaterialIndex;
		};

		vector<MeshEntry> m_Entries;
		//vector<Texture*> m_Textures;
};

#endif
