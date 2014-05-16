#define ENABLE_SHADER 0

#include "Building.h"

namespace sg {
	

	Building::Building(int id) {
		m_id = id;
		
		if (ENABLE_SHADER) {
			shader = new Shader();
			light = shader->lightShader("phongandtexture.frag", "phongandtexture.vert");
		}

		//this let the shaders on texture, so when you try to use shader on texture you add this on eline of a code
		texture = new Texture();
		if (ENABLE_SHADER) {
			glUniform1i(glGetUniformLocation(light, "building1.ppm"), 0);
		}
		texturePPM = texture->loadTexture("building1.ppm");
	}

	/*Building::Building(string filename) {
		World::m_heightMapXShift = 278;
		World::m_heightMapZShift = 463;
		initArrays();
		loadData(filename);
	}*/

	Building::~Building() {
		delete texture;
		texture = nullptr;
		
		delete shader;
		shader = nullptr;
	}

	void Building::draw(glm::mat4 parent, glm::mat4 cam) {
		//cout << "nverts is : " << m_nIndices[0] << endl;
		//cout << "Drawing: " << m_id << endl;
		glm::mat4 mv = glm::inverse(cam) * parent;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			//glColor4f(color.r, color.g, color.b, color.a);
			if (shouldDraw()) {
				drawShape();
			}
			/*else {
				cout << "didnt render: " << m_id << endl;
			}*/

			//drawShape();
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

		// These two lines of codes apply shader and texture
		if (ENABLE_SHADER) {
			glUseProgram(light);
		}
		glBindTexture(GL_TEXTURE_2D, texturePPM);

		sg::City* myParent = (sg::City*)getParent();

		// if city and want colorful buildings!
		if (!ENABLE_SHADER) {
			glColor4f(1,0,0,1);

			if (m_id % 6 == 0) {
				glColor4f(0,0,1,1);
			}
			if (m_id % 6 == 1) {
				glColor4f(0,1,0,1);
			}
			if (m_id % 6 == 2) {
				glColor4f(1,0,0,1);
			}
			if (m_id % 6 == 3) {
				glColor4f(0,1,1,1);
			}
			if (m_id % 6 == 4) {
				glColor4f(1,0,1,1);
			}
			if (m_id % 6 == 5) {
				glColor4f(1,1,0,1);
			}
		}

		//cout << "m_id: " << m_id << endl;

		glMaterialfv( GL_FRONT, GL_AMBIENT, m_material.m_ambient);
		glMaterialfv( GL_FRONT, GL_DIFFUSE, m_material.m_diffuse);
		glMaterialfv( GL_FRONT, GL_SPECULAR, m_material.m_specular);
		glMaterialfv( GL_FRONT, GL_EMISSION, m_material.m_emission);
		glMaterialf( GL_FRONT, GL_SHININESS, m_material.m_shininess);

		//glColor4f(((m_id%8)%4)%2, (m_id%4)%2, m_id%2, 1);

		for (int i = 0; i < myParent->m_nIndices[m_id]/3; i++) {
			//cout << m_indices[i] << endl;
			//cout << ((m_id%2)%2)%2 << endl;
			
			//if (!ENABLE_SHADER) {
			//	if (m_id == 2) {
			//		glColor4f(0,1,0,1);
			//	}
			//	else if (m_id == 7) {
			//		glColor4f(0,0,1,1);
			//	}
			//	else if (m_id == 25) {
			//		glColor4f(1,0,1,1);
			//	}

			//	else if (m_id == 31) {
			//		glColor4f(1,1,0,1);
			//	}
			//	else {
			//		glColor4f(1,0,0,1);
			//	}
			//}

			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				glNormal3f(myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]], myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]+1], myParent->m_normals[m_id][3*myParent->m_indices[m_id][3*i+j]+2]);
				glTexCoord2f(myParent->m_texcoords[m_id][2*myParent->m_indices[m_id][t]], myParent->m_texcoords[m_id][2*myParent->m_indices[m_id][t]+1]);
				glVertex3f(myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]], myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]+1], myParent->m_vertices[m_id][3*myParent->m_indices[m_id][3*i+j]+2]);
				t++;
			}
			glEnd();
		}
		

		//glUseProgram(0);
		//cout << max_ele << endl;
		glDisable(GL_TEXTURE_2D);
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
			if (myParent->m_vertices[m_id][i+2] > maxz) {
				maxz = myParent->m_vertices[m_id][i+2];
			}
		}
		
		m_boundingBox.setAABB(minx, miny, minz, maxx, maxy, maxz);

		glm::vec3 tmpPos = glm::vec3((maxx-minx)/2, (maxy-miny)/2, (maxz-minz)/2);

		m_physics = Physics(tmpPos, FLT_MAX);
	}

	bool Building::isInside(glm::vec3 point) {		
		return (m_boundingBox.inside(point));
	}

	bool Building::nearTop(glm::vec3 point) {		
		return (m_boundingBox.nearTop(point));
	}

	bool Building::clearedTop(AABB* other) {
		return (m_boundingBox.clearedTop(other));
	}

	glm::vec3 Building::intersects(glm::vec3 from, glm::vec3 goTo) {
		return (m_boundingBox.intersects(from, goTo));
	}

	float Building::angleIntersection(glm::vec3 from, glm::vec3 goTo) {
		return (m_boundingBox.angleIntersection(from, goTo));
	}

	int Building::reflectionIntersection(glm::vec3 from, glm::vec3 goTo) {
		return (m_boundingBox.reflectionIntersection(from, goTo));
	}

	bool Building::onTopOf(AABB* other) {
		return (m_boundingBox.onTopOf(other));
	}

	int Building::collidesWith(AABB* other) {
		if (m_id == 40 || !m_boundingBox.collidesWithPointer(other)) {
			return -1;
		}
		return m_id;
	}
	
	void Building::setMaterial() {
		m_material.setAmbient(0.7f, 0.7f, 0.7f, 1.0f); 
		m_material.setDiffuse(0.1f, 0.5f, 0.8f, 1.0f);
		m_material.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
		m_material.setEmission(0.3f, 0.2f, 0.2f, 0.0f);
		m_material.setShininess(128.0f);
	}
}
