#ifdef WIN32
	#define SNPRINTF _snprintf_s
#else
	#define SNPRINTF snprintf
#endif
#define INVALID_UNIFORM_LOCATION 0xffffffff

#include "MeshNode.h"

namespace sg {
	MeshNode::MeshNode() {
		m_mesh = new Mesh();
		initShader();
	}

	MeshNode::MeshNode(std::string filename) {
		m_mesh = new Mesh();
		m_mesh->LoadMesh(filename);
		initShader();
	}

	MeshNode::~MeshNode() {
		delete m_mesh;
		m_mesh = nullptr;

		delete m_shader;
		m_shader = nullptr;
	}

	void MeshNode::initShader() {
		m_shader = new Shader("../Shaders/skinning.vert", "../Shaders/skinning.frag");
		m_shaderID = m_shader->getShader();
		glUseProgram(m_shaderID);

		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
			char Name[128];
			memset(Name, 0, sizeof(Name));
			SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
			m_boneLocation[i] = GetUniformLocation(Name);
		}
	}

	Mesh *MeshNode::getMesh() {
		return m_mesh;
	}

	void MeshNode::loadMesh(std::string filename) {
		m_mesh->LoadMesh(filename);
	}

	void MeshNode::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(mv));

		glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);

		vector<glm::mat4> Transforms;
		float RunningTime = (float)((double)Utilities::GetCurrentTimeMillis() - (double)Utilities::m_startTime) / 1000.0f;
		m_mesh->BoneTransform(RunningTime, Transforms);
		m_mesh->Render();

		glPopMatrix();
	}

	void MeshNode::print() {
		cout << "(" << this->getObjectID() << " Mesh: " << this->getName() << ")";
	}

	void MeshNode::SetWVP(const glm::mat4& WVP)
	{
		glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
	}


	void MeshNode::SetWorldMatrix(const glm::mat4& WorldInverse)
	{
		glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
	}

	GLint MeshNode::GetUniformLocation(const char* pUniformName)
	{
		GLuint Location = glGetUniformLocation(m_shaderID, pUniformName);

		if (Location == INVALID_UNIFORM_LOCATION) {
			fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
		}

		return Location;
	}
}