#include "ObjModel.h"

ObjModel::ObjModel() {
	this->initScales();

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(int id) {
	this->initScales();
	m_id = id;

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename) {
	this->initScales();

	this->loadModel(objFilename);

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename, string mtlFilename) {
	this->initScales();

	this->loadModel(objFilename, mtlFilename);

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename, glm::vec3 currPos) {
	this->initScales();

	m_position = currPos;
	this->loadModel(objFilename);

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(string objFilename, string mtlFilename, glm::vec3 currPos) {
	this->initScales();

	m_position = currPos;
	this->loadModel(objFilename, mtlFilename);

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(int id, string objFilename) {
	this->initScales();

	m_id = id;
	this->loadModel(objFilename);

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::ObjModel(int id, string objFilename, string mtlFilename) {
	this->initScales();

	m_id = id;
	this->loadModel(objFilename, mtlFilename);

	//m_physics = Physics();
	this->setColor(glm::vec4(1,0,0,1));
}

ObjModel::~ObjModel() {

}

void ObjModel::initScales() {
	m_turtleScale = 0.01f;
	m_cityScale = 0.1f;
	m_canScale = 5.0f;
	m_defaultScale = 1.0f;
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
	//cout << glm::to_string(this->getMatrix()) << endl;
}

void ObjModel::draw(glm::mat4 parent, glm::mat4 cam) {
	this->setMatrix(glm::translate(this->getPosition()) * glm::scale(m_scaleVec));
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

	glEnable(GL_TEXTURE_2D);

	// change order of vertices for backface culling for can
	if (this->m_objFilename == "Can.obj") {
		glFrontFace(GL_CW);
	}

	//cout << "nverts is : " << m_nIndices[0] << endl;
	int p = 0;
	int k = 0;
	int l = 0;
	int t = 0;
	//cout << m_nm_indices << endl;
	int max_ele = 10000;
			
	// bind texture here
	//glBindTexture(GL_TEXTURE_2D, texturePPM);

	// if city and want colorful ObjModels!
	glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);

	//cout << "m_id: " << m_id << endl;
	glMaterialfv( GL_FRONT, GL_AMBIENT, m_material.m_ambient);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, m_material.m_diffuse);
	glMaterialfv( GL_FRONT, GL_SPECULAR, m_material.m_specular);
	glMaterialfv( GL_FRONT, GL_EMISSION, m_material.m_emission);
	glMaterialf( GL_FRONT, GL_SHININESS, m_material.m_shininess);

	for (int k = 0; k < m_nIndices.size(); k++) {
		for (int i = 0; i < m_nIndices[k]/3; i++) {
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				//cout << i << ", " << j << ", " << k << endl;
				//cout << "1 " << endl;
				//glNormal3f(m_normals[k][3*m_indices[k][3*i+j]], m_normals[k][3*m_indices[k][3*i+j]+1], m_normals[k][3*m_indices[k][3*i+j]+2]);
				//cout << "2 " << endl;
				//glTexCoord2f(m_texcoords[k][2*m_indices[k][t]], m_texcoords[k][2*m_indices[k][t]+1]);
				//cout << "3 " << endl;
				glVertex3f(m_vertices[k][3*m_indices[k][3*i+j]], m_vertices[k][3*m_indices[k][3*i+j]+1], m_vertices[k][3*m_indices[k][3*i+j]+2]);
				t++;
			}
			glEnd();
		}
	}
	//cout << max_ele << endl;	

	// reset backface culling
	glFrontFace(GL_CCW);
}

void ObjModel::loadFilename(string objFilename) {
	m_objFilename = objFilename;

	if (m_objFilename == "Can.obj") {
		m_scaleVec = glm::vec3(m_canScale);
	}
	else {
		m_scaleVec = glm::vec3(m_defaultScale);
	}
}

void ObjModel::loadFilename(string objFilename, string mtlFilename) {
	m_objFilename = objFilename;
	m_mtlFilename = mtlFilename;

	if (m_objFilename == "Can.obj") {
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
		cout << "[ObjModel] loading obj : " << m_objFilename << endl;
		cout << "[ObjModel] loading mtl : " << m_mtlFilename << endl;
		err = tinyobj::LoadObj(shapes, m_objFilename.c_str(), m_mtlFilename.c_str());
	}
	else if (!m_objFilename.empty()) {
		cout << "[ObjModel] loading obj : " << m_objFilename << endl;
		err = tinyobj::LoadObj(shapes, m_objFilename.c_str());
	}
	else {
		cout << "[ObjModel] ERROR : no obj file specified" << endl;
		return;
	}

	if (!err.empty()) {
		cout << "\t[tinyobj] " << err << endl;
	}

	int indicesCount = 0;
	int verticesCount = 0;
	int texturesCount = 0;
	int normalsCount = 0;
	int max_ele = -1;
	int added = 0;
	//int added = 0;
	//string tmp;
	//stringstream ss;
		
	for (int j = 0; j < shapes.size(); j++) {
		/*
		vector<float> tmpArr;

		if (m_filename == "city.obj") {
			tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_cityScale, 0.0f, -1.0f, 0.0f);
		}
		else if (m_filename == "Can.obj") {
			tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_canScale, 0.0f, 0.0f, 0.0f);
		}
		else if (m_filename == "turtle_obj.obj") {
			tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_turtleScale, 0.0f, 0.0f, 0.0f);
		}
		else {
			tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_defaultScale, 0.0f, 0.0f, 0.0f);
		}
		*/
		vector<int> tmpArr2;

		for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
			tmpArr2.push_back(shapes[j].mesh.indices[i]);
		}

		setVertices(shapes[j].mesh.positions);
		setNormals(shapes[j].mesh.normals);
		setTexcoords(shapes[j].mesh.texcoords);
		setIndices(tmpArr2);

		/*while (!tmpArr.empty()) {
			tmpArr.pop_back();
		}

		while (!tmpArr2.empty()) {
			tmpArr2.pop_back();
		}*/
			
		//cout << "sizes: " << shapes[j].mesh.indices.size() << endl;
		added++;

	}
	//cout << "please: "<< m_vertices[1][379] << ", " << m_vertices[1][381] << endl;
	//Utilities::writeIntArrayToFile(m_nVertices, added, "nverts.txt");
		
	//updateHeightMap();

	calculateBoundingBox();
	setMaterial();

	// TODO: send new bounding box to server and see if can make trap at that location
		
}

void ObjModel::calculateBoundingBox() {
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
				
	m_boundingBox.setAABB(minx+m_position.x, miny+m_position.y, minz+m_position.z, maxx+m_position.x, maxy+m_position.y, maxz+m_position.z);
}

bool ObjModel::isInside(glm::vec3 point) {		
	return (m_boundingBox.inside(point));
}

bool ObjModel::collidesWith(ObjModel* o) {
	return (m_boundingBox.collidesWith(o->m_boundingBox));
}
	
void ObjModel::setMaterial() {
	m_material.setAmbient(0.7f, 0.7f, 0.7f, 1.0f); 
	m_material.setDiffuse(0.1f, 0.5f, 0.8f, 1.0f);
	m_material.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	m_material.setEmission(0.3f, 0.2f, 0.2f, 0.0f);
	m_material.setShininess(128.0f);
}

void ObjModel::print() {
	cout << "(" << this->getObjectID() << " ObjModel " << m_id << ": " << this->getName() << ")";
}
