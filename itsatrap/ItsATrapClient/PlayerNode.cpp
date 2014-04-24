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
		return getPlayer()->getCamera();
	}

	glm::vec3 Player::getPosition() {
		return getPlayer()->getPosition();
	}

	int Player::getPlayerID() {
		return m_playerID;
	}

	void Player::setPlayerID(int id) {
		m_playerID = id;
	}

	// moves camera to player's view
	void Player::draw() {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(this->getPlayer()->getProjectionMatrix()));

		for (int i=0; i<m_nChild; i++) {
			m_child[i]->draw(glm::mat4(), this->getPlayer()->getCameraMatrix());
		}
	}

	// draws player avatar at player location
	void Player::draw(glm::mat4 parent, glm::mat4 cam) {		
		glm::mat4 new_model = parent * this->getPlayer()->getModelMatrix(); // create new mv from scenegraph + MyPlayer
		glm::mat4 mv = glm::inverse(cam) * new_model;

		// load updated mv matrix and draw shape for player
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor3f(1,1,1);
			glutSolidSphere(10, 10, 10);
		glPopMatrix();
	}

	void Player::move(glm::vec3 delta) {
		this->getPlayer()->move(delta);
	}

	void Player::moveTo(glm::vec3 pos) {
		this->getPlayer()->moveTo(pos);
	}

	void Player::lookIn(glm::vec3 direction) {
		this->getPlayer()->lookIn(direction);
	}

	void Player::lookAt(glm::vec3 point) {
		this->getPlayer()->lookAt(point);
	}

	void Player::print() {
		cout << "(" << this->getObjectID() << " Player p" << this->getPlayerID() << ": " << this->getName() << ")";
	}
}