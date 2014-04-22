#include "PlayerNode.h"

namespace sg {
	Player::Player() {
		m_player = new MyPlayer();
	}

	Player::Player(MyPlayer *p) {
		m_player = p;
	}

	Player::~Player() {
		delete m_player;
		m_player = nullptr;
	}

	MyPlayer *Player::getPlayer() {
		return m_player;
	}

	Camera *Player::getCamera() {
		return m_player->getCamera();
	}

	// moves camera to player's view
	void Player::draw() {
		m_player->updateModelViewMatrix();

		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixf(glm::value_ptr(m_player->m_modelviewMatrix));
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(m_player->m_projectionMatrix));

		for (int i=0; i<m_nChild; i++) {
			try {
				m_child[i]->draw(m_player->m_modelMatrix, m_player->getCameraMatrix());
			}
			catch (...)
			{
			}
		}
	}

	// draws player avatar at player location
	void Player::draw(glm::mat4 parent, glm::mat4 camera) {
		m_player->updateModelViewMatrix(); // update MyPlayer mv matrix
		glm::mat4 new_model = parent * m_player->m_modelviewMatrix; // create new mv from scenegraph + MyPlayer
		
		glm::mat4 cam_inverse = glm::inverse(camera);
		glm::mat4 mv = cam_inverse * new_model;

		// load updated mv matrix and draw shape for player
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor3f(1,1,1);
			glutSolidSphere(10, 10, 10);
		glPopMatrix();
	}

	void Player::move(glm::vec3 delta) {
		m_player->move(delta);
	}

	void Player::moveTo(glm::vec3 pos) {
		m_player->moveTo(pos);
	}

	void Player::lookIn(glm::vec3 direction) {
		m_player->lookIn(direction);
	}

	void Player::lookAt(glm::vec3 point) {
		m_player->lookAt(point);
	}
}