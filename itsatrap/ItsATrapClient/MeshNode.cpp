#include "ClientInstance.h"
extern ClientInstance *client;

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

		m_WVPLocation = GetUniformLocation("gWVP");
		m_WorldMatrixLocation = GetUniformLocation("gWorld");

		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
			char Name[128];
			memset(Name, 0, sizeof(Name));
			SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
			m_boneLocation[i] = GetUniformLocation(Name);
			cout << m_boneLocation[i] << endl;
		}

		m_proj = glm::perspective(90.0f, 3.0f / 3.0f, 0.1f, 100.0f);
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

			glUseProgram(m_shaderID);

			vector<glm::mat4> Transforms;
			float RunningTime = (float)((double)Utilities::GetCurrentTimeMillis() - (double)Utilities::m_startTime) / 1000.0f;
			RunningTime = 0;
			m_mesh->BoneTransform(RunningTime, Transforms);
			for (uint i = 0 ; i < Transforms.size() ; i++) {
				SetBoneTransform(i, Transforms[i]);
			}

			// ANURAG IS NOOB
			//Pipeline p;
			//p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
			//p.SetPerspectiveProj(m_persProjInfo);
			//p.Scale(0.1f, 0.1f, 0.1f);

			//Vector3f Pos(m_position);
			//p.WorldPos(Pos);
			//p.Rotate(270.0f, 180.0f, 0.0f);

			//SetWVP(p.GetWVPTrans());
			//SetWorldMatrix(p.GetWorldTrans());

			SetWVP(parent * glm::inverse(cam) * m_proj);
			SetWorldMatrix(parent);

			m_mesh->Render();

			glUseProgram(0);

		glPopMatrix();
	}

	void MeshNode::print() {
		cout << "(" << this->getObjectID() << " Mesh: " << this->getName() << ")";
	}

	void MeshNode::SetWVP(const glm::mat4& WVP)
	{
		//glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, glm::value_ptr(WVP));
		glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, glm::value_ptr(WVP));
	}

	void MeshNode::SetWorldMatrix(const glm::mat4& WorldInverse)
	{
		//glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, glm::value_ptr(WorldInverse));
		glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, glm::value_ptr(WorldInverse));
	}

	void MeshNode::SetBoneTransform(uint Index, const glm::mat4& Transform) {
		//assert(Index < MAX_BONES);
		//Transform.Print();
		glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE, glm::value_ptr(Transform));
		//cout << glm::to_string(Transform) << endl;
	}

	GLint MeshNode::GetUniformLocation(const char* pUniformName)
	{
		GLuint Location = glGetUniformLocation(m_shaderID, pUniformName);

		if (Location == INVALID_UNIFORM_LOCATION) {
			cout << "Warning! Unable to get the location of uniform '" << pUniformName << "'" << endl;
		}

		return Location;
	}
}