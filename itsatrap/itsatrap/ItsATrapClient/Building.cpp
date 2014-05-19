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

	void Building::draw(glm::mat4 parent, glm::mat4 camera) {
		glm::mat4 cam_inverse = glm::inverse(camera);
		glm::mat4 mv = cam_inverse * parent;
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor3f(color.x, color.y, color.z);
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

		// bind texture here
		glBindTexture(GL_TEXTURE_2D, textuPpm);

		sg::City* myParent = (sg::City*)getParent();
		// TODO: change 46
		for (int i = 0; i < myParent->m_nIndices[m_id]/3; i++) {
			//cout << m_indices[i] << endl;
			//glColor3f(1, 1, 0);
		
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				glNormal3f(myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]], myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]+1], myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]+2]);
				glTexCoord2f(myParent->m_texcoords[m_id][2*myParent->	m_indices[m_id][t]], myParent->m_texcoords[m_id][2*myParent->m_indices[m_id][t]+1]);
				glVertex3f(myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]], myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]+1], myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]+2]);
				t++;
			}
			glEnd();
		}
		
		//cout << max_ele << endl;	
	}

	void Building::calculateBoundingBox() {

		sg::City* myParent = (sg::City*)getParent();

		float minx = FLT_MAX;
		float miny = FLT_MAX;
		float minz = FLT_MAX;
		float maxx = FLT_MIN;
		float maxy = FLT_MIN;
		float maxz = FLT_MIN;

		for (int i = 0; i < myParent->m_nVertices[m_id]; i+=3) {
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
			if (myParent->m_vertices[m_id][i+2] > maxz) {
				maxz = myParent->m_vertices[m_id][i+2];
			}
		}
		if (m_id == 2) {
			cout << "minx: " << minx << endl;
			cout << "miny: " << miny << endl;
			cout << "minz: " << minz << endl;
			cout << "maxx: " << maxx << endl;
			cout << "maxy: " << maxy << endl;
			cout << "maxz: " << maxz << endl;
		}
		m_boundingBox.setAABB(minx, miny, minz, maxx, maxy, maxz);
	}

	// TODO: find better fix for road
	bool Building::isInside(glm::vec3 goTo) {
		if (m_id == 40) {
			return false;
		}
		else {
			return (m_boundingBox.inside(goTo));
		}
	}
}

