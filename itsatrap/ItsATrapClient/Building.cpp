#include "Building.h"

namespace sg {
	

	Building::Building(int id) {
		m_id = id;
	}

	/*Building::Building(string filename) {
		World::m_heightMapXShift = 278;
		World::m_heightMapZShift = 463;
		initArrays();
		loadData(filename);
	}*/

	Building::~Building() {
		
	
	}

	void Building::draw(glm::mat4 parent, glm::mat4 cam) {
		//cout << "nverts is : " << m_nIndices[0] << endl;
		//cout << "Drawing: " << m_id << endl;
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			//glColor3f(color.x, color.y, color.z);
			drawShape();
		glPopMatrix();

	}

	void Building::drawShape() {
		
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

		sg::City* myParent = (sg::City*)getParent();

		// if city and want colorful buildings!
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

		//glColor3f(((m_id%8)%4)%2, (m_id%4)%2, m_id%2);
		for (int i = 0; i < myParent->m_nIndices[m_id]/3; i++) {
			//cout << m_indices[i] << endl;
			//cout << ((m_id%2)%2)%2 << endl;
			/*
			if (m_id == 2) {
				glColor3f(0, 1, 0);
			}
			else if (m_id == 7) {
				glColor3f(0, 0, 1);
			}
			else if (m_id == 25) {
				glColor3f(1, 0, 1);
			}

			else if (m_id == 31) {
				glColor3f(1, 1, 0);
			}
			else {
				glColor3f(1, 0, 0);
			}*/
		
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				glNormal3f(myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]], myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]+1], myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]+2]);
				glTexCoord2f(myParent->m_texcoords[m_id][2*myParent->m_indices[m_id][t]], myParent->m_texcoords[m_id][2*myParent->m_indices[m_id][t]+1]);
				glVertex3f(myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]], myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]+1], myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]+2]);
				t++;
			}
			glEnd();
		}
		
		//cout << max_ele << endl;	
	}

	void Building::print() {
		cout << "(" << this->getObjectID() << " Building " << m_id << ": " << this->getName() << ")";
	}

	void Building::calculateBoundingBox() {
		sg::City* myParent = (sg::City*)getParent();

		float minx = FLT_MAX;
		float miny = FLT_MAX;
		float minz = FLT_MAX;
		float maxx = -1.0f*FLT_MAX;
		float maxy = -1.0f*FLT_MAX;
		float maxz = -1.0f*FLT_MAX;
		
		for (int i = 0; i < myParent->m_nVertices[m_id]/*-myParent->m_nVertices[m_id]%12*/; i+=3) {
			if (myParent->m_vertices[m_id][i] < minx) {
				minx = myParent->m_vertices[m_id][i];
			}
			if (myParent->m_vertices[m_id][i+1] < miny) {
				miny = myParent->m_vertices[m_id][i+1];
			}
			if (myParent->m_vertices[m_id][i+2] < minz) {
				minz = myParent->m_vertices[m_id][i+2];
			}
			if (myParent->m_vertices[m_id][i] > maxx) {
				maxx = myParent->m_vertices[m_id][i];
			}
			if (myParent->m_vertices[m_id][i+1] > maxy) {
				maxy = myParent->m_vertices[m_id][i+1];
			}
			//if (m_id == 7) {
			//	cout << myParent->m_vertices[m_id][i+2] << endl;
			//}
			if (myParent->m_vertices[m_id][i+2] > maxz) {
				maxz = myParent->m_vertices[m_id][i+2];
				/*if (m_id == 7) {
					cout << "i'm working" << endl;
				}*/
			}
		}
		/*
		if (m_id == 7) {
			cout << "minx: " << minx << endl;
			cout << "miny: " << miny << endl;
			cout << "minz: " << minz << endl;
			cout << "maxx: " << maxx << endl;
			cout << "maxy: " << maxy << endl;
			cout << "maxz: " << maxz << endl;
		}*/
		
		m_boundingBox.setAABB(minx, miny, minz, maxx, maxy, maxz);
	}

	bool Building::isInside(glm::vec3 point) {		
		if (m_id == 43) {
			return false;
		}
		else {
			//cout << "m_id: " << m_id << endl;
			//cout << "mins: " << m_boundingBox.m_minX << ", " << m_boundingBox.m_minY << ", " << m_boundingBox.m_minZ << endl;
			//cout << "maxs: " << m_boundingBox.m_maxX << ", " << m_boundingBox.m_maxY << ", " << m_boundingBox.m_maxZ << endl;
			//cout << "goto: " << glm::to_string(point) << endl;
			return (m_boundingBox.inside(point));
		}
	}

	bool Building::collidesWith(Building* b) {
		return (m_boundingBox.collidesWith(b->m_boundingBox));
	}
	
	void Building::setMaterial() {
		m_material.setAmbient(0.7f, 0.7f, 0.7f, 1.0f); 
		m_material.setDiffuse(0.1f, 0.5f, 0.8f, 1.0f);
		m_material.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		m_material.setEmission(0.3f, 0.2f, 0.2f, 0.0f);
		m_material.setShininess(128.0f);
	}
}
