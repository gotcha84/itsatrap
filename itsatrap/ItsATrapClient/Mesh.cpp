#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

#include "Mesh.h"

#include <assert.h>

void Mesh::VertexBoneData::AddBoneData(uint BoneID, float Weight)
{
    for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i] = BoneID;
            Weights[i] = Weight;
            return;
        }        
    }
    
    // should never get here - more bones than we have space for
    assert(0);
}

Mesh::Mesh()
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = nullptr;
}

Mesh::~Mesh() 
{
	Clear();
}

void Mesh::Clear()
{
    for (uint i = 0 ; i < m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }
       
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}

bool Mesh::LoadMesh(const string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();
 
    // Create the VAO
    glGenVertexArrays(1, &m_VAO);   
    glBindVertexArray(m_VAO);
    
    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;    
  
    m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (m_pScene) {  
        m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();
        Ret = InitFromScene(m_pScene, Filename);
    }
    else {
        cout << "Error parsing '" << Filename.c_str() << "': '" << m_Importer.GetErrorString() << "'" << endl;
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);	

    return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const string& Filename)
{  
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    vector<glm::vec3> Positions;
    vector<glm::vec3> Normals;
    vector<glm::vec2> TexCoords;
    vector<VertexBoneData> Bones;
    vector<uint> Indices;
       
    uint NumVertices = 0;
    uint NumIndices = 0;
    
    // Count the number of vertices and indices
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;
        
        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);
        
    // Initialize the meshes in the scene one by one
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
    }

    if (!InitMaterials(pScene, Filename)) {
        return false;
    }

    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    return glGetError() == GL_NO_ERROR;
}

void Mesh::InitMesh(unsigned int Index, const aiMesh *paiMesh) {
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

    vector<Vertex> Vertices;
    vector<unsigned int> Indices;
	
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
	// load vertices, normal vectors, texture coords
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(
			glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z)
		);

        Vertices.push_back(v);
    }

	// ANURAG - verts
	vector<float> verts;
	for (unsigned int i=0; i<paiMesh->mNumVertices; i++) {
		verts.push_back(paiMesh->mVertices[i].x);
		verts.push_back(paiMesh->mVertices[i].y);
		verts.push_back(paiMesh->mVertices[i].z);
	}

	// load indices
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	// ANURAG - inds
	vector<unsigned int> inds;
	for (unsigned int i=0; i<paiMesh->mNumFaces; i++) {
		aiFace &face = paiMesh->mFaces[i];
		inds.push_back(face.mIndices[0]);
		inds.push_back(face.mIndices[1]);
		inds.push_back(face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices, Indices);
	m_Entries[Index].verts = vector<float>(verts);
	m_Entries[Index].inds = vector<unsigned int>(inds);
}

// TODO - fix textures + materials to use our Texture class
/*
bool Mesh::InitMaterials(const aiScene *pScene, const string &Filename) {
	// Extract the directory part from the file name
    string::size_type SlashIndex = Filename.find_last_of("/");
    string Dir;

    if (SlashIndex == string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;    
	
    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (!m_Textures[i]) {
            m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

            Ret = m_Textures[i]->Load();
        }
    }

    return Ret;
}
*/

void Mesh::draw() {
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	//for (unsigned int i = 0; i < m_Entries.size(); i++) {
	//	glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

	//	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
	//	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//		//m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
	//	//}

	//	glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);

	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);

	// ANURAG
	for (unsigned int i=0; i<m_Entries.size(); i++) {
		glBegin(GL_TRIANGLES);
			for (int ind=0; ind<m_Entries[i].inds.size(); ind++) {
				glVertex3f(m_Entries[i].verts[3*m_Entries[i].inds[ind]], m_Entries[i].verts[3*m_Entries[i].inds[ind]+1], m_Entries[i].verts[3*m_Entries[i].inds[ind]+2]);
			}
		glEnd();
	}
}
