#include "Geode.h"

namespace sg {

	Geode::Geode() {
		m_color = glm::vec4(1,0,0,1);
	}

	Geode::~Geode() {

	}

	void Geode::setPosition(glm::vec3 pos) {
		m_position = pos;
		//this->updateBoundingBox();
	}
	
	glm::vec3 Geode::getPosition() {
		return m_position;
	}

	void Geode::setMatrix(glm::mat4 model) {
		m_model = model;
	}

	glm::mat4 Geode::getMatrix() {
		return m_model;
	}

	void Geode::setColor(glm::vec4 color) {
		m_color = color;
	}

	glm::vec4 Geode::getColor() {
		return m_color;
	}

	AABB Geode::getBoundingBox() {
		return m_boundingBox;
	}

	void Geode::setBoundingBox(glm::vec3 pos, float rad) {
		m_boundingBox.setAABB(pos, rad);
	}

	void Geode::print() {
		cout << "(" << this->getObjectID() << " Geode: " << this->getName() << ")";
	}

	// TODO : implement base class
	void Geode::calculateBoundingBox() {

	}

	// TODO : implement base class
	bool Geode::isInside(glm::vec3 goTo) {
		cout << "geode isInside" << endl;
		return false;
	}

	// void Geode::collidesWith(Geode b) {

	// }
	
	void Geode::setMaterial() {

	}

	bool Geode::shouldDraw() {
		float o[16],p[16];

		glGetFloatv(GL_PROJECTION_MATRIX,p);
		glGetFloatv(GL_MODELVIEW_MATRIX,o);

		glm::mat4 a = glm::mat4(o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8], o[9], o[10], o[11], o[12], o[13], o[14], o[15]);
		glm::mat4 b = glm::mat4(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
	
		glm::mat4 c = b*a;
		//c = glm::transpose(c);

		//cout << "c: " << glm::to_string(c) << endl;

		// left right bottom top near far
		

		//float left = -100.0f;
		//float right = 100.0f;
		//float bottom = -100.0f;
		//float top = 100.0f;
		//float anear = 100.0f;
		//float afar = 10000.0f;

		////cout << glm::to_string(a) << endl;

		//glm::vec3 currPos = glm::vec3(a[3][0], a[3][1], a[3][2]);

		//AABB frustrum = AABB(currPos.x+left, currPos.y+bottom, currPos.z+anear, currPos.x+right, currPos.y+top, currPos.z+afar);

		//return(m_boundingBox.inside(frustrum) || m_boundingBox.collidesWith(frustrum));

		/*glm::vec4 anear = glm::vec4(c[2][0] + c[3][0], c[2][1] + c[3][1], c[2][2] + c[3][2], c[2][3] + c[3][3]);
		glm::vec4 afar = glm::vec4(-1*c[2][0] + c[3][0], -1*c[2][1] + c[3][1], -1*c[2][2] + c[3][2], -1*c[2][3] + c[3][3]);
		glm::vec4 bottom = glm::vec4(c[1][0] + c[3][0], c[1][1] + c[3][1], c[1][2] + c[3][2], c[1][3] + c[3][3]);
		glm::vec4 top = glm::vec4(-1*c[1][0] + c[3][0], -1*c[1][1] + c[3][1], -1*c[1][2] + c[3][2], -1*c[1][3] + c[3][3]);
		glm::vec4 left = glm::vec4(c[0][0] + c[3][0], c[0][1] + c[3][1], c[0][2] + c[3][2], c[0][3] + c[3][3]);
		glm::vec4 right = glm::vec4(-1*c[0][0] + c[3][0], -1*c[0][1] + c[3][1], -1*c[0][2] + c[3][2], -1*c[0][3] + c[3][3]);*/


		glm::vec4 anear = glm::vec4(c[2][0], c[2][1], c[2][2], c[2][3]);
		glm::vec4 afar = glm::vec4(-1*c[2][0], -1*c[2][1], -1*c[2][2], -1*c[2][3]);
		glm::vec4 bottom = glm::vec4(c[1][0], c[1][1], c[1][2], c[1][3]);
		glm::vec4 top = glm::vec4(-1*c[1][0], -1*c[1][1], -1*c[1][2], -1*c[1][3]);
		glm::vec4 left = glm::vec4(c[0][0], c[0][1], c[0][2], c[0][3]);
		glm::vec4 right = glm::vec4(-1*c[0][0], -1*c[0][1], -1*c[0][2], -1*c[0][3]);

		anear = glm::normalize(anear);
		afar = glm::normalize(afar);
		bottom = glm::normalize(bottom);
		top = glm::normalize(top);
		left = glm::normalize(left);
		right = glm::normalize(right);

		/*cout << "anear: "<< glm::to_string(anear) << endl;
		cout << "afar: "<< glm::to_string(afar) << endl;	
		cout << "bottom: "<< glm::to_string(bottom) << endl;
		cout << "top: "<< glm::to_string(top) << endl;
		cout << "left: "<< glm::to_string(left) << endl;
		cout << "right: "<< glm::to_string(right) << endl << endl;*/

		vector<glm::vec4> planes;
		planes.push_back(anear);
		planes.push_back(afar);
		planes.push_back(top);
		planes.push_back(bottom);
		planes.push_back(left);
		planes.push_back(right);
		
		float midX = (m_boundingBox.maxX+m_boundingBox.minX)/2;
		float midY = (m_boundingBox.maxY+m_boundingBox.minY)/2;
		float midZ = (m_boundingBox.maxZ+m_boundingBox.minZ)/2;

		float xLength = 2.0f*(m_boundingBox.maxX-m_boundingBox.minX);
		float yLength = 2.0f*(m_boundingBox.maxY-m_boundingBox.minY);
		float zLength = 2.0f*(m_boundingBox.maxZ-m_boundingBox.minZ);

		//float maxLength = max(max(midX, midY), midZ);
		//float maxLength = max(max(xLength, yLength), zLength);
		float maxLength = sqrt((xLength*xLength)+(yLength*yLength)+(zLength*zLength));
		
		glm::vec4 mids = glm::vec4(midX, midY, midZ, 1);
		glm::vec4 transformed = c*mids;
		//
		float distance;
		int count = 0;

		for (int i = 0; i < 6; i++) {
			distance = planes[i][0]*transformed[0] + planes[i][1]*transformed[1] + planes[i][2]*transformed[2] /*+ planes[i][3]*transformed[3] */;
			if (distance > maxLength) {
				return false;
			}
		}
		return true;
	}
}

