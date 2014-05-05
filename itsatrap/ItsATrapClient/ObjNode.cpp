#include "ObjNode.h"

namespace sg {
	
	ObjNode::ObjNode() {
		m_color = glm::vec3(1,0,0);
		m_turtleScale = 0.01f;
		m_cityScale = 0.1f;
		m_canScale = 5.0f;
		m_defaultScale = 1.0f;

		m_physics = Physics();
		
	}

	ObjNode::ObjNode(int id) {
		m_id = id;
		m_color = glm::vec3(1,0,0);
		m_turtleScale = 0.01f;
		m_cityScale = 0.1f;
		m_canScale = 5.0f;
		m_defaultScale = 1.0f;

		m_physics = Physics();
	}

	ObjNode::~ObjNode() {
		
		/*for (int i = 0; i < m_maxNumArrays; i++) {
			delete[] m_vertices[i];
			m_vertices[i] = nullptr;

			delete[] m_normals[i];
			m_normals[i] = nullptr;

			delete[] m_texcoords[i];
			m_texcoords = nullptr;

			delete[] m_indices[i];
			m_indices[i] = nullptr;
		}

		delete[] m_vertices;
		m_vertices = nullptr;
		delete[] m_nVertices;
		m_nVertices = nullptr;
		delete[] m_normals;
		m_normals = nullptr;
		delete[] m_texcoords;
		m_texcoords = nullptr;
		delete[] m_indices;
		m_indices = nullptr;
		delete[] m_nIndices;
		m_nIndices = nullptr;*/
	}

	void ObjNode::setVertices(vector<float> arr) {
		m_vertices.push_back(arr);
		setNVertices(arr.size());
	}
	
	void ObjNode::setNormals(vector<float> arr) {
		m_normals.push_back(arr);
	}
	
	void ObjNode::setTexcoords(vector<float> arr) {
		m_texcoords.push_back(arr);
	}
	
	void ObjNode::setIndices(vector<int> arr) {
		m_indices.push_back(arr);
		setNIndices(arr.size());
	}
	
	void ObjNode::setNVertices(int ele) {
		m_nVertices.push_back(ele);
	}
	
	void ObjNode::setNIndices(int ele) {
		m_nIndices.push_back(ele);
		cout << glm::to_string(this->getMatrix()) << endl;
	}


	void ObjNode::draw(glm::mat4 parent, glm::mat4 cam) {
		//cout << "Drawing: " << m_id << endl;
		this->setMatrix(glm::translate(this->getPosition()) * glm::scale(m_scaleVec));
		glm::mat4 new_model = parent * this->getMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));
			//glColor3f(color.x, color.y, color.z);
			drawShape();
		glPopMatrix();

	}

	void ObjNode::drawShape() {
		//cout << "HIHI\n";
		glEnable(GL_TEXTURE_2D);

		//cout << "nverts is : " << m_nIndices[0] << endl;
		int p = 0;
		int k = 0;
		int l = 0;
		int t = 0;
		//cout << m_nm_indices << endl;
		int max_ele = 10000;
			
		// bind texture here
		glBindTexture(GL_TEXTURE_2D, texturePPM);

		// if city and want colorful ObjNodes!
		if (m_id % 6 == 0) {
			glColor3f(0, 0, 1);
		}
		if (m_id % 6 == 1) {
			glColor3f(0, 1, 0);
		}
		if (m_id % 6 == 2) {
			glColor3f(1, 0, 0);
		}
		if (m_id % 6 == 3) {
			glColor3f(0, 1, 1);
		}
		if (m_id % 6 == 4) {
			glColor3f(1, 0, 1);
		}
		if (m_id % 6 == 5) {
			glColor3f(1, 1, 0);
		}

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
	}

	void ObjNode::loadData() {

		vector<tinyobj::shape_t> shapes;
  
		string err = tinyobj::LoadObj(shapes, m_objFilename.c_str());

		int indicesCount = 0;
		int verticesCount = 0;
		int texturesCount = 0;
		int normalsCount = 0;
		int max_ele = -1;
		int added = 0;
		//int added = 0;
		//std::string tmp;
		//stringstream ss;
		
		for (int j = 0; j < shapes.size(); j++) {
			/*
			vector<float> tmpArr;

			if (m_objFilename == "city.obj") {
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_cityScale, 0.0f, -1.0f, 0.0f);
			}
			else if (m_objFilename == "Can.obj") {
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_canScale, 0.0f, 0.0f, 0.0f);
			}
			else if (m_objFilename == "turtle_obj.obj") {
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

		cout << "DONE!\n";
		
	}


	void ObjNode::calculateBoundingBox() {
		
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
				
		m_boundingBox.setAABB(minx, miny, minz, maxx, maxy, maxz);
		
	}

	bool ObjNode::isInside(glm::vec3 point) {		
		return (m_boundingBox.inside(point));
	}

	bool ObjNode::collidesWith(ObjNode* o) {
		return (m_boundingBox.collidesWith(o->m_boundingBox));
	}
	
	void ObjNode::setMaterial() {
		m_material.setAmbient(0.7f, 0.7f, 0.7f, 1.0f); 
		m_material.setDiffuse(0.1f, 0.5f, 0.8f, 1.0f);
		m_material.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		m_material.setEmission(0.3f, 0.2f, 0.2f, 0.0f);
		m_material.setShininess(128.0f);
	}

	void ObjNode::print() {
		cout << "(" << this->getObjectID() << " ObjNode " << m_id << ": " << this->getName() << ")";
	}

}
