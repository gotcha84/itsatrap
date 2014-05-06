#define PLAYER_RAD 5.0f

#include "PlayerNode.h"

namespace sg {

	Player::Player() {
		this->setPlayerID(0);
		m_player = new MyPlayer();
		m_hud = new HUD();
		
		m_translate = glm::vec3();
		m_rotate = glm::quat();
		m_scale = glm::vec3(1,1,1);

		m_xAngleChange = 0.0f;
		m_yAngleChange = 0.0f;

		m_color = glm::vec3(10,10,10);
		m_xAngleChangeFactor = 20.0f;
		m_yAngleChangeFactor = 20.0f;
	}

	Player::Player(glm::vec3 pos) {
		this->setPlayerID(0);
		m_player = new MyPlayer(pos);
		m_hud = new HUD();
		
		m_translate = glm::vec3();
		m_rotate = glm::quat();
		m_scale = glm::vec3(1,1,1);

		m_xAngleChange = 0.0f;
		m_yAngleChange = 0.0f;

		m_color = glm::vec3(10,10,10);
		m_xAngleChangeFactor = 20.0f;
		m_yAngleChangeFactor = 20.0f;
	}

	Player::Player(MyPlayer *p) {
		m_player = p;
	}

	Player::~Player() {
		delete m_player;
		m_player = nullptr;

		delete m_hud;
		m_hud = nullptr;
	}

	void Player::setColor(glm::vec3 color) {
		m_color = color;
	}

	glm::vec3 Player::getColor() {
		return m_color;
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

	glm::mat4 Player::getModelMatrix() {
		m_translate = this->getPosition();
		glm::mat4 translationMatrix = glm::translate(m_translate);

		this->getCamera()->calculateAxis();
		glm::mat4 rotatedX = this->rotateY(this->getCamera()->m_xRotated);
		//glm::mat4 rotatedY = this->rotate(this->getCamera()->m_camX, this->getCamera()->m_yRotated);
		glm::mat4 rotationMatrix = rotatedX;
		
		glm::mat4 scaleMatrix = glm::scale(m_scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
	
	glm::mat4 Player::rotateY(float theta) { // theta = angle in degrees
		theta = glm::radians(theta);
		
		return glm::transpose(glm::mat4(
			glm::cos(theta), 0, glm::sin(theta), 0,
			0, 1, 0, 0,
			-glm::sin(theta), 0, glm::cos(theta), 0, 
			0, 0, 0, 1
			));
	}

	// TODO - fix implementation
	glm::mat4 Player::rotate(glm::vec3 axis, float theta) {
		axis = glm::normalize(axis);
		theta = glm::radians(theta);

		float c = glm::cos(theta);
		float s = glm::sin(theta);
		float ax = axis.x;
		float ay = axis.y;
		float az = axis.z;

		return glm::transpose(glm::mat4(
			ax*ax+c*(1.0f-ax*ax), ax*ay*(1.0f-c)-az*s, ax*az*(1.0f-c)+ay*s, 0, 
			ax*ay*(1.0f-c)+az*s, ay*ay+c*(1.0f-ay*ay), ay*az*(1.0f-c)-ax*s, 0, 
			ax*az*(1.0f-c)-ay*s, ay*az*(1.0f-c)+ax*s, az*az+c*(1.0f-az*az), 0, 
			0, 0, 0, 1.0f
			));
		
		//glm::transpose(glm::mat4(
		//	1+(1-glm::cos(theta))*(axis.x*axis.x - 1), -axis.z*glm::sin(theta)+(1-glm::cos(theta))*axis.x*axis.y, axis.y*glm::sin(theta)+(1-glm::cos(theta))*axis.x*axis.z, 0, 
		//	axis.z*glm::sin(theta)+(1-glm::cos(theta))*axis.y*axis.x, 1+(1-glm::cos(theta))*(axis.y*axis.y-1), -axis.x*glm::sin(theta)+(1-glm::cos(theta))*axis.y*axis.z, 0, 
		//	-axis.y*glm::sin(theta)+(1-glm::cos(theta))*axis.z*axis.x, axis.x*glm::sin(theta)+(1-glm::cos(theta))*axis.z*axis.y, 1+(1-glm::cos(theta))*(axis.z*axis.z-1), 0, 
		//	0, 0, 0, 1
		//	));
	}

	int Player::getPlayerID() {
		return m_playerID;
	}

	void Player::setPlayerID(int id) {
		m_playerID = id;
	}

	// moves camera to player's view
	void Player::draw() {
		//cout << "velocity: " << glm::to_string(this->getPlayer()->getPhysics()->m_velocity) << endl;
		//cout << "velocity diff: " << glm::to_string(this->getPlayer()->getPhysics()->m_velocityDiff) << endl << endl;
		

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(this->getPlayer()->getProjectionMatrix()));

		for (int i=0; i<m_nChild; i++) {
			m_child[i]->draw(glm::mat4(), this->getPlayer()->getCameraMatrix());
		}

		// draw player avatar
		glm::mat4 mv = glm::inverse(this->getCamera()->getCameraMatrix()) * this->getModelMatrix();
		this->drawAsCurrentPlayer(mv);

		// draw player hud
		m_hud->draw(this->getHealth());
	}

	void Player::drawAsCurrentPlayer(glm::mat4 mv) {
		// load updated mv matrix and draw shape for player
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor3f(this->getColor().x, this->getColor().y, this->getColor().z);
			glutWireCube(PLAYER_RAD*2);
		glPopMatrix();
	}

	// draws player avatar at player location
	void Player::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 new_model = parent * this->getModelMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;
		
		this->drawAsOtherPlayer(mv);
	}

	void Player::drawAsOtherPlayer(glm::mat4 mv) {
		// load updated mv matrix and draw shape for player
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glPushMatrix();
				glTranslatef(0, 0, -PLAYER_RAD);
				glScalef(1, 1, 0.1f);
				glColor3f(10,0,0);
				glutSolidCube(PLAYER_RAD*2);
			glPopMatrix();

			glColor3f(this->getColor().x, this->getColor().y, this->getColor().z);
			glutSolidCube(PLAYER_RAD*2);
		glPopMatrix();
	}

	void Player::handleXRotation(float magnitude) {
		this->getPlayer()->handleXRotation(magnitude);
	}

	void Player::handleYRotation(float magnitude) {
		this->getPlayer()->handleYRotation(magnitude);
	}

	void Player::handleMovement(unsigned char key) {
		this->getPlayer()->handleMovement(key);
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

	void Player::setUp(glm::vec3 direction) {
		this->getCamera()->m_cameraUp = direction;
	}

	void Player::print() {
		cout << "(" << this->getObjectID() << " Player p" << this->getPlayerID() << ": " << this->getName() << ")";
	}

	bool Player::collidesWith(sg::Player *other) {
		return this->getPlayer()->collidesWith(other->getPlayer());
	}

	bool Player::knifeHitWith(sg::Player *other) {
		return this->getPlayer()->knifeHitWith(other->getPlayer());
	}

	void Player::updateHealthHud() {
		this->m_hud->drawHealthBar(this->getHealth());
	}

	struct playerObject Player::getPlayerObjectForNetworking()
	{
		struct playerObject p = {};
		
		p.id = this->getPlayerID();
		p.health = this->getHealth();
		p.x = this->getPosition().x;
		p.y = this->getPosition().y;
		p.z = this->getPosition().z;
		p.aabb.minX = this->getPlayer()->getAABB()->m_minX;
		p.aabb.minY = this->getPlayer()->getAABB()->m_minY;
		p.aabb.minZ = this->getPlayer()->getAABB()->m_minZ;
		p.aabb.maxX = this->getPlayer()->getAABB()->m_maxX;
		p.aabb.maxY = this->getPlayer()->getAABB()->m_maxY;
		p.aabb.maxZ = this->getPlayer()->getAABB()->m_maxZ;
		p.lookX = this->getCamera()->getCameraLookAt().x;
		p.lookY = this->getCamera()->getCameraLookAt().y;
		p.lookZ = this->getCamera()->getCameraLookAt().z;
		p.upX = this->getCamera()->getCameraUp().x;
		p.upY = this->getCamera()->getCameraUp().y;
		p.upZ = this->getCamera()->getCameraUp().z;
		p.xRotated = this->getCamera()->getXRotated();
		p.yRotated = this->getCamera()->getYRotated();
		
		return p;
	}

	int Player::getHealth() {
		return this->getPlayer()->getHealth();
	}

	void Player::setHealth(int health) {
		this->getPlayer()->setHealth(health);
	}

	void Player::damage(int dmg) {
		this->getPlayer()->damage(dmg);
	}

	bool Player::isDead() {
		return this->getPlayer()->isDead();
	}

	bool Player::isAlive() {
		return this->getPlayer()->isAlive();
	}
}