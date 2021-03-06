#include <gl/glew.h>
#include "ObjModel.h"

ObjModel::ObjModel() {
	this->initCommon();

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(int id) {
	this->initCommon();
	m_id = id;

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename) {
	this->initCommon();

	this->loadModel(objFilename);

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename, string mtlFilename) {
	this->initCommon();

	this->loadModel(objFilename, mtlFilename);
	
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename, glm::vec3 currPos) {
	this->initCommon();

	m_position = currPos;
	this->loadModel(objFilename);

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename, string mtlFilename, glm::vec3 currPos) {
	this->initCommon();

	m_position = currPos;
	this->loadModel(objFilename, mtlFilename);

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(int id, string objFilename) {
	this->initCommon();

	m_id = id;
	this->loadModel(objFilename);

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(int id, string objFilename, string mtlFilename) {
	this->initCommon();

	m_id = id;
	this->loadModel(objFilename, mtlFilename);

	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::~ObjModel() {
	delete shader2;
	shader2 = nullptr;
}

void ObjModel::initCommon() {
	m_turtleScale = 0.01f;
	m_cityScale = 0.1f;
	m_canScale = 5.0f;
	m_defaultScale = 1.0f;

	m_flipTex = false;
	/*shader2->loadShader("../Shaders/light.vert", "../Shaders/light.frag");
	light = shader2->getShader();*/
}

void ObjModel::setVertices(vector<float> arr) {
	m_vertices.push_back(arr);
	setNVertices(arr.size());
}
	
void ObjModel::setNormals(vector<float> arr) {
	m_normals.push_back(arr);
}
	
void ObjModel::setTexcoords(vector<float> arr) {
	m_texcoords.push_back(arr);
}
	
void ObjModel::setIndices(vector<int> arr) {
	m_indices.push_back(arr);
	setNIndices(arr.size());
}
	
void ObjModel::setNVertices(int ele) {
	m_nVertices.push_back(ele);
}
	
void ObjModel::setNIndices(int ele) {
	m_nIndices.push_back(ele);
}

void ObjModel::draw(glm::mat4 parent, glm::mat4 cam) {

	this->setMatrix(glm::translate(this->getPosition()) *  glm::scale(m_scaleVec));
	glm::mat4 new_model = parent * this->getMatrix();
	glm::mat4 mv = glm::inverse(cam) * new_model;

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(mv));
		glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
		drawModel();
	glPopMatrix();

}

void ObjModel::drawModel() {
	// change order of vertices for backface culling for can
	if (this->m_objFilename == "../Models/Can.obj") {
		glFrontFace(GL_CW);
	}

	int p = 0;
	int k = 0;
	int l = 0;
	int t = 0;
	int max_ele = FLT_MAX;

	// bind texture
	if (m_texID != 0) {
		//glUseProgram(light);
		glEnable(GL_TEXTURE_2D);
		//for light vert and frag
		/*glUniform3f(glGetUniformLocation(light, "lightPos"), 0.0, 0.0, 0.0);
		glUniform3f(glGetUniformLocation(light, "mambient"), 0.8, 0.8, 0.8);
		glUniform3f(glGetUniformLocation(light, "mdiffuse"), 0.6, 0.6, 0.6);
		glUniform3f(glGetUniformLocation(light, "mspecular"), 1.0, 1.0, 1.0);
		glUniform3f(glGetUniformLocation(light, "lambient"), 0.8, 0.8, 0.8);
		glUniform3f(glGetUniformLocation(light, "ldiffuse"), 0.6, 0.6, 0.6);
		glUniform3f(glGetUniformLocation(light, "lspecular"), 1.0, 1.0, 1.0);//changes color of light
		glUniform1f(glGetUniformLocation(light, "shininess"), 16.0);
		glUniform1i(glGetUniformLocation(light, "img"), 0);*/
		glBindTexture(GL_TEXTURE_2D, m_texID);
		

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
	for (int k = 0; k < m_nIndices.size(); k++) {
		t = 0;
		int temp = m_materials.size();

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_materials[k].m_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_materials[k].m_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_materials[k].m_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_materials[k].m_emission);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_materials[k].m_shininess);

		for (int i = 0; i < m_nIndices[k] / 3; i++) {
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				if (!m_normals.empty()) {
					glNormal3f(m_normals[k][3 * m_indices[k][3 * i + j]], m_normals[k][3 * m_indices[k][3 * i + j] + 1], m_normals[k][3 * m_indices[k][3 * i + j] + 2]);
				}
				if (m_flipTex) {
					glTexCoord2f(m_texcoords[k][2 * m_indices[k][t]], 1.0f - m_texcoords[k][2 * m_indices[k][t] + 1]);
				}
				else {
					glTexCoord2f(m_texcoords[k][2 * m_indices[k][t]], m_texcoords[k][2 * m_indices[k][t] + 1]);
				}
				glVertex3f(m_vertices[k][3 * m_indices[k][3 * i + j]], m_vertices[k][3 * m_indices[k][3 * i + j] + 1], m_vertices[k][3 * m_indices[k][3 * i + j] + 2]);
				t++;
			}
			glEnd();
		}
	}
	
	if (this->getName() == "penis") {
		int kkk = 0;
	}

	// unbind texture
	if (m_texID != 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		//glUseProgram(0);
	}

	// reset backface culling
	//glFrontFace(GL_CCW);

	if (m_drawBB) {
		//this->getBoundingBox().draw();
	}
}

void ObjModel::loadFilename(string objFilename) {
	m_objFilename = objFilename;

	if (m_objFilename == "../Models/Can.obj") {
		m_scaleVec = glm::vec3(m_canScale);
	}
	else {
		m_scaleVec = glm::vec3(m_defaultScale);
	}
}

void ObjModel::loadFilename(string objFilename, string mtlFilename) {
	m_objFilename = objFilename;
	m_mtlFilename = mtlFilename;

	if (m_objFilename == "../Models/Can.obj") {
		m_scaleVec = glm::vec3(m_canScale);
	}
	else {
		m_scaleVec = glm::vec3(m_defaultScale);
	}
}

void ObjModel::loadModel(string objFilename) {
	this->loadFilename(objFilename);
	this->loadModel();
}

void ObjModel::loadModel(string objFilename, string mtlFilename) {
	this->loadFilename(objFilename, mtlFilename);
	this->loadModel();
}

void ObjModel::loadModel() {

	vector<tinyobj::shape_t> shapes;

	string err;
	if (!m_objFilename.empty() && !m_mtlFilename.empty()) {
		//cout << "[ObjModel] loading obj : " << m_objFilename << endl;
		//cout << "[ObjModel] loading mtl : " << m_mtlFilename << endl;
		err = tinyobj::LoadObj(shapes, m_objFilename.c_str(), m_mtlFilename.c_str());
	}
	else if (!m_objFilename.empty()) {
		//cout << "[ObjModel] loading obj : " << m_objFilename << endl;
		err = tinyobj::LoadObj(shapes, m_objFilename.c_str());
	}
	else {
		cout << "[ObjModel] ERROR : no obj file specified" << endl;
		return;
	}

	if (!err.empty()) {
		cout << "\t[tinyobj] " << err << " (" << m_objFilename << ")" << endl;
	}

	int indicesCount = 0;
	int verticesCount = 0;
	int texturesCount = 0;
	int normalsCount = 0;
	int max_ele = -1;
	int added = 0;

	for (int j = 0; j < shapes.size(); j++) {
		vector<int> tmpArr2;

		for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
			tmpArr2.push_back(shapes[j].mesh.indices[i]);
		}

		setVertices(shapes[j].mesh.positions);
		setNormals(shapes[j].mesh.normals);
		setTexcoords(shapes[j].mesh.texcoords);
		setIndices(tmpArr2);

		added++;

		// Add materials for given model part in same order
		Material * temp = new Material(shapes[j].material.ambient, 
			shapes[j].material.diffuse, 
			shapes[j].material.specular,
			shapes[j].material.emission,
			shapes[j].material.shininess);

		addMaterial(temp->m_ambient, temp->m_diffuse, temp->m_specular, temp->m_emission, temp->m_shininess);
	}

	//updateHeightMap();
	calculateBoundingBox();
}

void ObjModel::setTexture(GLuint tex) {
	m_texID = tex;
}

void ObjModel::flipTexture() {
	m_flipTex = true;
}

void ObjModel::unflipTexture() {
	m_flipTex = false;
}

void ObjModel::calculateBoundingBox() {

	this->calculateBoundingBox(glm::mat4());
}

void ObjModel::calculateBoundingBox(glm::mat4 model) {
	float minx = FLT_MAX;
	float miny = FLT_MAX;
	float minz = FLT_MAX;
	float maxx = -1.0f*FLT_MAX;
	float maxy = -1.0f*FLT_MAX;
	float maxz = -1.0f*FLT_MAX;

	for (int k = 0; k < m_nVertices.size(); k++) {
		for (int i = 0; i < m_nVertices[k]/*-m_nVertices[k]%12*/; i+=3) {
			if (m_vertices[k][i] < minx) {
				minx = m_vertices[k][i];
			}
			if (m_vertices[k][i+1] < miny) {
				miny = m_vertices[k][i+1];
			}
			if (m_vertices[k][i+2] < minz) {
				minz = m_vertices[k][i+2];
			}
			if (m_vertices[k][i] > maxx) {
				maxx = m_vertices[k][i];
			}
			if (m_vertices[k][i+1] > maxy) {
				maxy = m_vertices[k][i+1];
			}
			//if (k == 7) {
			//	cout << m_vertices[k][i+2] << endl;
			//}
			if (m_vertices[k][i+2] > maxz) {
				maxz = m_vertices[k][i+2];
				/*if (k == 7) {
					cout << "i'm working" << endl;
				}*/
			}
		}
	}

	glm::vec4 minVec = model*glm::vec4(minx, miny, minz, 1);
	glm::vec4 maxVec = model*glm::vec4(maxx, maxy, maxz, 1);

	minx = minVec.x;
	miny = minVec.y;
	minz = minVec.z;
	maxx = maxVec.x;
	maxy = maxVec.y;
	maxz = maxVec.z;

	m_boundingBox.setAABB(minx, miny, minz, maxx, maxy, maxz);
}

bool ObjModel::isInside(glm::vec3 point) {		
	return (m_boundingBox.inside(point));
}

bool ObjModel::collidesWith(ObjModel* o) {
	return (m_boundingBox.collidesWith(o->m_boundingBox));
}
	
void ObjModel::setMaterial() {
	//m_material.setAmbient(0.7f, 0.7f, 0.7f, 1.0f); 
	//m_material.setDiffuse(0.1f, 0.5f, 0.8f, 1.0f);
	//m_material.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	//m_material.setEmission(0.3f, 0.2f, 0.2f, 0.0f);
	//m_material.setShininess(128.0f);
}

void ObjModel::addMaterial(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emission[4], GLfloat shininess) {
	m_materials.push_back(Material(ambient, diffuse, specular, emission, shininess));

	//m_material.setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	//m_material.setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	//m_material.setSpecular(specular[0], specular[1], specular[2], specular[3]);
	//m_material.setEmission(emission[0], emission[1], emission[2], emission[3]);
	//m_material.setShininess(shininess);
}

void ObjModel::print() {
	cout << "(" << this->getObjectID() << " ObjModel " << m_id << ": " << this->getName() << ")";
}
