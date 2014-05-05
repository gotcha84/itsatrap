#include "Trap.h"

namespace sg {

	Trap::Trap(glm::vec3 currPos) {
		m_position = currPos;
		m_objFilename = "Can.obj";
		m_model = glm::translate(currPos);
		m_color = glm::vec3(1,0,0);
		m_turtleScale = 0.01f;
		m_cityScale = 0.1f;
		m_canScale = 5.0f;
		m_defaultScale = 1.0f;

		if (m_objFilename == "Can.obj") {
			m_scaleVec = glm::vec3(m_canScale, m_canScale, m_canScale);
		}
		else {
			m_scaleVec = glm::vec3(m_defaultScale, m_defaultScale, m_defaultScale);
		}

		m_physics = Physics(currPos, FLT_MAX);
		
		loadData();
	}

	Trap::Trap(glm::vec3 currPos, int type) {
	}

	Trap::~Trap() {
		//delete m_boundingBox;
		//m_boundingBox = nullptr;
	}
	
	void Trap::draw(glm::mat4 parent, glm::mat4 cam) {
		//cout << "Drawing: " << m_id << endl;
		this->setMatrix(glm::translate(this->getPosition()) * glm::scale(m_scaleVec));
		glm::mat4 new_model = parent * this->getMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));
			
			glColor3f(1,0,0);
			glutSolidCube(1.0f);
		glPopMatrix();
	}

	void Trap::print() {
		cout << "(" << this->getObjectID() << " Trap: " << this->getName() << ")";
	}

}