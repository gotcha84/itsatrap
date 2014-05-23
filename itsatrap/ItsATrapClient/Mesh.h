#define NUM_BONES_PER_VEREX 4
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

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

#include "Utilities.h"

#include <vector>
#include <iostream>

using namespace std;
typedef unsigned int uint;

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
		void Render();
		uint NumBones() const {
			return m_NumBones;
		}
		void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);

		//private:
		struct BoneInfo
		{
			glm::mat4 BoneOffset;
			glm::mat4 FinalTransformation;        

			BoneInfo() {
				BoneOffset = glm::mat4(0);
				FinalTransformation = glm::mat4(0);
			}
		};
    
		struct VertexBoneData
		{        
			uint IDs[NUM_BONES_PER_VEREX];
			float Weights[NUM_BONES_PER_VEREX];

			VertexBoneData() {
				Reset();
			};
        
			void Reset() {
				ZERO_MEM(IDs);
				ZERO_MEM(Weights);        
			}
        
			void AddBoneData(uint BoneID, float Weight);
		};

		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
		uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
		bool InitFromScene(const aiScene* pScene, const string& Filename);
		void InitMesh(uint MeshIndex,
					  const aiMesh* paiMesh,
					  vector<glm::vec3>& Positions,
					  vector<glm::vec3>& Normals,
					  vector<glm::vec2>& TexCoords,
					  vector<VertexBoneData>& Bones,
					  vector<unsigned int>& Indices);
		void LoadBones(uint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
		bool InitMaterials(const aiScene* pScene, const string& Filename);
		void Clear();

		enum VB_TYPES {
			INDEX_BUFFER,
			POS_VB,
			NORMAL_VB,
			TEXCOORD_VB,
			BONE_VB,
			NUM_VBs            
		};

			GLuint m_VAO;
			GLuint m_Buffers[NUM_VBs];

		struct MeshEntry {
			MeshEntry()
			{
				NumIndices    = 0;
				BaseVertex    = 0;
				BaseIndex     = 0;
				MaterialIndex = -1;
			}
        
			unsigned int NumIndices;
			unsigned int BaseVertex;
			unsigned int BaseIndex;
			unsigned int MaterialIndex;
		};

		vector<MeshEntry> m_Entries;
		//vector<Texture*> m_Textures;
     
		map<string,uint> m_BoneMapping; // maps a bone name to its index
		uint m_NumBones;
		vector<BoneInfo> m_BoneInfo;
		glm::mat4 m_GlobalInverseTransform;

		const aiScene* m_pScene;
		Assimp::Importer m_Importer;
};

#endif
