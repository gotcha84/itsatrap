#include "Mesh.h"

#include <assert.h>

Mesh::MeshEntry::MeshEntry() {
    VB = -1;
    IB = -1;
    NumIndices  = 0;
    MaterialIndex = -1;
};

Mesh::MeshEntry::~MeshEntry() {
    if (VB != -1) {
        glDeleteBuffers(1, &VB);
		VB = -1;
    }

    if (IB != -1) {
        glDeleteBuffers(1, &IB);
		IB = -1;
    }
}

void Mesh::MeshEntry::Init(const vector<Vertex> &Vertices, const vector<unsigned int> &Indices) {
	NumIndices = Indices.size();

    glGenBuffers(1, &VB);
  	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

Mesh::Mesh() {

}

Mesh::~Mesh() {
	Clear();
}

void Mesh::Clear() {
  //  for (unsigned int i = 0; i < m_Textures.size(); i++) {
		//if (m_Textures[i]) {
		//	delete m_Textures[i];
		//	m_Textures[i] = nullptr;
		//}
  //  }
}

bool Mesh::LoadMesh(const string &Filename) {
    Clear(); // Release the previously loaded mesh (if it exists)

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
    }
    else {
        cout << "Error parsing '" << Filename << "': '" << Importer.GetErrorString() << endl;
    }

    return Ret;
}

bool Mesh::InitFromScene(const aiScene *pScene, const string &Filename) {
    m_Entries.resize(pScene->mNumMeshes);
    //m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    //return InitMaterials(pScene, Filename);
	return true;
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

	// anu - verts
	vector<float> verts;
	for (unsigned int i=0; i<paiMesh->mNumVertices; i+=3) {
		verts.push_back(paiMesh->mVertices->x);
		verts.push_back(paiMesh->mVertices->y);
		verts.push_back(paiMesh->mVertices->z);
	}

	// load indices
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

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

void Mesh::Render() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
        //glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

        //const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        //if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
        //    m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        //}

        //glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBegin(GL_TRIANGLES);
			for (int ind=0; ind<m_Entries[i].inds.size(); ind+=3) {
				glVertex3f(m_Entries[i].verts[m_Entries[i].inds[ind]], m_Entries[i].verts[m_Entries[i].inds[ind+1]], m_Entries[i].verts[m_Entries[i].inds[ind+2]]);
			}
		glEnd();

		//cout << "# verts : " << m_Entries[i].verts.size() << endl;
		//cout << "# inds : " << m_Entries[i].inds.size() << endl;
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
