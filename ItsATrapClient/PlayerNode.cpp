#define PLAYER_RAD 5.0f

#include "PlayerNode.h"

#include "Texture.h"
extern Texture *textures;

#include "ClientInstance.h"
extern ClientInstance* client;

namespace sg {

	Player::Player() {
		this->setPlayerID(0);
		m_player = new MyPlayer();
		m_hud = new HUD();
		board = new Scoreboard();
		
		m_translate = glm::vec3();
		m_rotate = glm::quat();
		m_scale = glm::vec3(1,1,1);

		m_xAngleChange = 0.0f;
		m_yAngleChange = 0.0f;

		m_color = glm::vec4(10,10,10,1);
		m_xAngleChangeFactor = 20.0f;
		m_yAngleChangeFactor = 20.0f;

		trapMenu = new TrapMenu();
		gameOver = new GameOver();

		timer = new Stopwatch();
		checkMouse = false;

		m_elapsedGameTime = 0;
		m_gameOver = false;

		int numBloodParticles = 0;
		ConfigSettings::getConfig()->getValue("NumBloodParticles", numBloodParticles);
		bloodEffect = new ParticleSystem3(numBloodParticles, getPlayer()->getPosition());

		initModels();
	}

	Player::Player(glm::vec3 pos) {
		this->setPlayerID(0);
		m_player = new MyPlayer(pos);
		m_hud = new HUD();
		board = new Scoreboard();

		m_translate = glm::vec3();
		m_rotate = glm::quat();
		m_scale = glm::vec3(1,1,1);

		m_xAngleChange = 0.0f;
		m_yAngleChange = 0.0f;

		m_color = glm::vec4(10,10,10,1);
		m_xAngleChangeFactor = 20.0f;
		m_yAngleChangeFactor = 20.0f;

		trapMenu = new TrapMenu();
		gameOver = new GameOver();

		timer = new Stopwatch();
		checkMouse = false;

		m_elapsedGameTime = 0;
		m_gameOver = false;

		int numBloodParticles = 0;
		ConfigSettings::getConfig()->getValue("NumBloodParticles", numBloodParticles);
		bloodEffect = new ParticleSystem3(numBloodParticles, getPlayer()->getPosition());


		initModels();
	}

	Player::Player(MyPlayer *p) {
		m_player = p;

		m_elapsedGameTime = 0;
		m_gameOver = false;

		int numBloodParticles = 0;
		ConfigSettings::getConfig()->getValue("NumBloodParticles", numBloodParticles);
		bloodEffect = new ParticleSystem3(numBloodParticles, getPlayer()->getPosition());
	}

	Player::~Player() {
		delete m_player;
		m_player = nullptr;

		delete m_hud;
		m_hud = nullptr;

		delete m_thisPlayer;
		m_thisPlayer = nullptr;

		delete m_otherPlayer;
		m_otherPlayer = nullptr;

		delete board;
		board = nullptr;

		delete trapMenu;
		trapMenu = nullptr;

		delete gameOver;
		gameOver = nullptr;

		delete timer;
		timer = nullptr;

		delete bloodEffect;
		bloodEffect = nullptr;

		delete m_thisPlayerAttack;
		m_thisPlayerAttack = nullptr;

		delete m_otherPlayerAttack;
		m_otherPlayerAttack = nullptr;
	}

	void Player::initModels() {

		// Determine color
		glm::vec4 color;
		color = glm::vec4(1, 1, 1, 1);
		//if (this->getPlayerID() % 2 == 0)
			//color = glm::vec4(0.75, 0, 0, 1);
		//else
			//color = glm::vec4(0, 0, 0.75, 1);

		m_otherPlayer = new ObjModel();
		m_otherPlayer->loadModel("../Models/Avatar/Polynoid.obj", "../Models/Avatar/");
		m_otherPlayer->flipTexture();
		if (getPlayerID() % 2 == 0) {
			m_otherPlayer->setTexture(textures->m_texID[Textures::RedPolynoid]);
		}
		else {
			m_otherPlayer->setTexture(textures->m_texID[Textures::BluePolynoid]);
		}
		m_otherPlayer->disableDrawBB();
		m_otherPlayer->setColor(color);


		m_thisPlayer = new ObjModel();
		m_thisPlayer->loadModel("../Models/Avatar/Polynoid_Headless.obj", "../Models/Avatar/");
		m_thisPlayer->flipTexture();
		if (getPlayerID() % 2 == 0) {
			m_thisPlayer->setTexture(textures->m_texID[Textures::RedPolynoid]);
		}
		else {
			m_thisPlayer->setTexture(textures->m_texID[Textures::BluePolynoid]);
		}
		m_thisPlayer->disableDrawBB();
		m_thisPlayer->setColor(color);

		m_thisPlayerAttack = new ObjModel();
		m_thisPlayerAttack->loadModel("../Models/Avatar/Headless_Attack_01.obj", "../Models/Avatar/");
		m_thisPlayerAttack->flipTexture();
		if (getPlayerID() % 2 == 0) {
			m_thisPlayerAttack->setTexture(textures->m_texID[Textures::RedPolynoid]);
		}
		else {
			m_thisPlayerAttack->setTexture(textures->m_texID[Textures::BluePolynoid]);
		}
		m_thisPlayerAttack->disableDrawBB();
		m_thisPlayerAttack->setColor(color);

		m_otherPlayerAttack = new ObjModel();
		m_otherPlayerAttack->loadModel("../Models/Avatar/Avatar_Attack_01.obj", "../Models/Avatar/");
		m_otherPlayerAttack->flipTexture();
		if (getPlayerID() % 2 == 0) {
			m_otherPlayerAttack->setTexture(textures->m_texID[Textures::RedPolynoid]);
		}
		else {
			m_otherPlayerAttack->setTexture(textures->m_texID[Textures::BluePolynoid]);
		}
		m_otherPlayerAttack->disableDrawBB();
		m_otherPlayerAttack->setColor(color);

	}

	void Player::setColor(glm::vec4 color) {
		m_color = color;
	}

	glm::vec4 Player::getColor() {
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
		// ANDRE
		
		int PlayerHeight = 0;
		ConfigSettings::getConfig()->getValue("PlayerHeight", PlayerHeight);
		//cout << "camxrot: " << this->getCamera()->getXRotated() << endl;
		glm::vec3 camcam = this->getCamera()->getCameraCenter();
		//if (true) {
		//	camcam.y += -10.0f;
		//}
		glm::mat4 translationMatrix = glm::translate(glm::vec3(m_translate.x, /*m_translate.y - 10.0f*/ camcam.y - PlayerHeight, m_translate.z));

		this->getCamera()->calculateAxis();
		glm::mat4 rotatedX = Utilities::rotateY(this->getCamera()->m_xRotated);
		//glm::mat4 rotatedY = this->rotate(this->getCamera()->m_camX, this->getCamera()->m_yRotated);
		glm::mat4 rotationMatrix = rotatedX;
		
		glm::mat4 scaleMatrix = glm::scale(m_scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
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

	float Player::getFadeForFlashOrBlood(int x, int fadeOut)
	{
		float fade = 0;
		
		if (x > fadeOut)
			fade = 1;
		else
			fade = (float)x / fadeOut;
		if (fade < 0)
			fade = 0;

		return fade;
	}

	// moves camera to player's view
	void Player::draw() {
		//cout << "velocity: " << glm::to_string(this->getPlayer()->getPhysics()->m_velocity) << endl;
		//cout << "velocity diff: " << glm::to_string(this->getPlayer()->getPhysics()->m_velocityDiff) << endl << endl;
		
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(this->getPlayer()->getProjectionMatrix()));

		GameStates curr = client->gameState.getState();
		if (curr == GAMEOVER) {
			cout << "GAME OVER" << endl;
			int teamOneScore = 0;
			int teamTwoScore = 0;
			for (unordered_map<int, sg::Player*>::iterator it = client->players.begin(); it != client->players.end(); it++) {
				sg::Player *player = it->second;
				if (player->getPlayerID() % 2) {
					teamOneScore = teamOneScore + player->getPlayer()->m_numKills - player->getPlayer()->m_numDeaths;
				}else{
					teamTwoScore = teamTwoScore + player->getPlayer()->m_numKills - player->getPlayer()->m_numDeaths;
				}
			}
			gameOver->setTeamScore(teamOneScore, teamTwoScore);
			glDisable(GL_LIGHTING);
			gameOver->draw();		// TODO: Get Screen to actually show?
			//glEnable(GL_LIGHTING);
		}
		else {
			if (!m_player->m_deathState)
			{
				for (int i=0; i<m_nChild; i++) {
					m_child[i]->draw(glm::mat4(), this->getPlayer()->getCameraMatrix());
				}
			}
			/*else
			{
				cout << "DEAD! Time till respawn " << m_player->m_timeUntilRespawn <<endl;
			}*/

			// draw player avatar
			glm::mat4 mv = glm::inverse(this->getCamera()->getCameraMatrix()) * this->getModelMatrix();
			//this->drawAsCurrentPlayer(mv);

			// draw player hud
			//TODO: last parameter is the respawn input time. from server to client
			for (unordered_map<int, sg::Player*>::iterator it = client->players.begin(); it != client->players.end(); it++) {
				sg::Player *player = it->second;
				//cout << "player " << player->getPlayerID() << " : " << player->getPlayer()->m_numKills << " / " << player->getPlayer()->m_numDeaths << endl;
				//cout << client->tabPressed << endl;

			}
		
			if (client->tabPressed) {
				for (unordered_map<int, sg::Player*>::iterator it = client->players.begin(); it != client->players.end(); it++) {
					sg::Player *player = it->second;
					Scoreboard::Entry entry;
					entry.setName( player->getPlayerID() );
					entry.setKill(player->getPlayer()->m_numKills);
					entry.setDeath(player->getPlayer()->m_numDeaths);
					board->addEntry(entry);
					//cout << "player " << player->getPlayerID() << " : " << player->getPlayer()->m_numKills << " / " << player->getPlayer()->m_numDeaths << endl;
					//cout << client->tabPressed << endl;
				}
				glDisable(GL_LIGHTING);
				board->setCurrentPlayer(client->root->getPlayerID());
				board->draw();
				//glEnable(GL_LIGHTING);
			}
			
			if (client->scrollDown || client->scrollUp) {
				timer->start();
				checkMouse = true;
				if (client->scrollUp) {
					if ((trapMenu->getInfoState() + 1) == 7) trapMenu->setInfoState(0);
					else trapMenu->setInfoState(trapMenu->getInfoState() + 1);
					client->scrollUp = false;
				}
				if (client->scrollDown) {
					if ((trapMenu->getInfoState() - 1) < 0) trapMenu->setInfoState(6);
					else trapMenu->setInfoState(trapMenu->getInfoState() - 1);
					client->scrollDown = false;
				}
			}

			if (timer->getElapsedMilliseconds() < 5000 && checkMouse == true) {
				if (client->root->getPlayer()->getHealth() > 0) {
					glDisable(GL_LIGHTING);
					trapMenu->draw();
					//glEnable(GL_LIGHTING);
				}
			}else{
				timer->reset();
				checkMouse = false;
			}
			
			//if (client->scrollUp) cout << "playerNode: scrollUp" << endl;
			//if (client->scrollDown) cout << "playerNode: scrollDown" << endl;

			int flashFadeOut = 0, bloodFadeOut = 0;
			ConfigSettings::getConfig()->getValue("FlashFadeOut", flashFadeOut);
			ConfigSettings::getConfig()->getValue("BloodFadeOut", bloodFadeOut);
			float flash = getFadeForFlashOrBlood(getPlayer()->m_flashDuration, flashFadeOut);
			float blood = getFadeForFlashOrBlood(getPlayer()->m_bloodDuration, bloodFadeOut);

			glDisable(GL_LIGHTING);

			m_hud->draw(this->getHealth(), this->getPlayer()->m_resources, m_player->m_timeUntilRespawn, flash, blood, getPlayer()->m_hitCrosshairDuration, getPlayer()->m_recallElapsed, m_player->m_infoMsg.getMessage(), m_elapsedGameTime, getPlayer()->m_slowDuration, getPlayer()->m_stunDuration);

			//glEnable(GL_LIGHTING);
		}

	}

	void Player::drawAsCurrentPlayer(glm::mat4 mv) {
		// load updated mv matrix and draw shape for player

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);
			
			if (getPlayer()->m_knifeDelay > 800)
			{
				//m_thisPlayerAttack->setColor(this->getPlayerID() % 2 == 0 ? glm::vec4(0.75, 0, 0, 1) : glm::vec4(0, 0, 0.75, 1));
				if (getPlayerID() % 2 == 0) {
					m_thisPlayerAttack->setTexture(textures->m_texID[Textures::RedPolynoid]);
				}
				else {
					m_thisPlayerAttack->setTexture(textures->m_texID[Textures::BluePolynoid]);
				}
				m_thisPlayerAttack->drawModel();
			}
			else
			{
				//m_thisPlayer->setColor(this->getPlayerID() % 2 == 0 ? glm::vec4(0.75, 0, 0, 1) : glm::vec4(0, 0, 0.75, 1));
				if (getPlayerID() % 2 == 0) {
					m_thisPlayer->setTexture(textures->m_texID[Textures::RedPolynoid]);
				}
				else {
					m_thisPlayer->setTexture(textures->m_texID[Textures::BluePolynoid]);
				}
				m_thisPlayer->drawModel();
			}

		glPopMatrix();
	}

	// draws player avatar at player location
	void Player::draw(glm::mat4 parent, glm::mat4 cam) {
		glm::mat4 new_model = parent * this->getModelMatrix();
		glm::mat4 mv = glm::inverse(cam) * new_model;
		
		this->drawAsOtherPlayer(mv);

		if (getPlayer()->m_startBlood)
		{
			getPlayer()->m_startBlood = false;
			bloodEffect->particlesReset();
		}

		bloodEffect->m_origin = getPlayer()->getPosition();
		bloodEffect->draw(parent, cam);

		if (m_drawBB) {
			m_otherPlayer->getBoundingBox().draw();
		}
	}

	void Player::drawAsOtherPlayer(glm::mat4 mv) {

		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(mv));

			glColor4f(this->getColor().r, this->getColor().g, this->getColor().b, this->getColor().a);

			if (getPlayer()->m_knifeDelay > 800)
			{
				//m_otherPlayerAttack->setColor(this->getPlayerID() % 2 == 0 ? glm::vec4(0.75, 0, 0, 1) : glm::vec4(0, 0, 0.75, 1));
				if (getPlayerID() % 2 == 0) {
					m_otherPlayerAttack->setTexture(textures->m_texID[Textures::RedPolynoid]);
				}
				else {
					m_otherPlayerAttack->setTexture(textures->m_texID[Textures::BluePolynoid]);
				}
				m_otherPlayerAttack->drawModel();
			}
			else
			{
				//m_otherPlayer->setColor(this->getPlayerID() % 2 == 0 ? glm::vec4(0.75, 0, 0, 1) : glm::vec4(0, 0, 0.75, 1));
				if (getPlayerID() % 2 == 0) {
					m_otherPlayer->setTexture(textures->m_texID[Textures::RedPolynoid]);
				}
				else {
					m_otherPlayer->setTexture(textures->m_texID[Textures::BluePolynoid]);
				}
				m_otherPlayer->drawModel();
			}

			
		glPopMatrix();
	}

	void Player::handleXRotation(float magnitude) {
		this->getPlayer()->handleXRotation(magnitude);
	}

	void Player::handleYRotation(float magnitude) {
		this->getPlayer()->handleYRotation(magnitude);
	}

	void Player::handleMovement(unsigned char key) {
		//this->getPlayer()->handleMovement(key);
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

	void Player::enableDrawBB() {
		m_drawBB = true;
	}

	void Player::disableDrawBB() {
		m_drawBB = false;
	}

	void Player::print() {
		cout << "(" << this->getObjectID() << " Player p" << this->getPlayerID() << ": " << this->getName() << ")";
	}

	bool Player::collidesWith(sg::Player *other) {
		return this->getPlayer()->collidesWith(other->getPlayer());
	}

	//bool Player::knifeHitWith(sg::Player *other) {
	//	return this->getPlayer()->knifeHitWith(other->getPlayer());
	//}

	struct playerObject Player::getPlayerObjectForNetworking()
	{
		struct playerObject p = {};
		
		p.id = this->getPlayerID();
		p.health = this->getHealth();
		p.position = glm::vec3(this->getPosition());
		p.aabb.minX = this->getPlayer()->getAABB()->minX;
		p.aabb.minY = this->getPlayer()->getAABB()->minY;
		p.aabb.minZ = this->getPlayer()->getAABB()->minZ;
		p.aabb.maxX = this->getPlayer()->getAABB()->maxX;
		p.aabb.maxY = this->getPlayer()->getAABB()->maxY;
		p.aabb.maxZ = this->getPlayer()->getAABB()->maxZ;
		p.cameraObject.cameraLookAt = this->getCamera()->getCameraLookAt();
		p.cameraObject.cameraCenter = this->getCamera()->getCameraCenter();
		p.cameraObject.cameraUp = this->getCamera()->getCameraUp();
		p.cameraObject.xRotated = this->getCamera()->getXRotated();
		p.cameraObject.yRotated = this->getCamera()->getYRotated();
		p.stunDuration = m_player->m_stunDuration;
		p.slowDuration = m_player->m_slowDuration;
		p.timeUntilRespawn = m_player->m_timeUntilRespawn;
		p.onTopOfBuildingId = m_player->m_onTopOfBuildingId;
		p.deathState = m_player->m_deathState;
		p.cameraObject.camZ = m_player->getCamera()->m_camZ;
		p.cameraObject.camX = m_player->getCamera()->m_camX;
		p.cameraObject.xAngle = m_player->getCamera()->m_xRotationAngle;
		p.cameraObject.yAngle = m_player->getCamera()->m_yRotationAngle;
		return p;
	}

	struct cameraObject Player::getCameraObjectForNetworking()
	{
		struct cameraObject cam = {};
		cam.cameraCenter = this->getCamera()->getCameraCenter();
		cam.cameraLookAt = this->getCamera()->getCameraLookAt();
		cam.cameraUp = this->getCamera()->getCameraUp();
		cam.camX = this->getCamera()->m_camX;
		cam.camZ = this->getCamera()->m_camZ;
		cam.xRotated = this->getCamera()->getXRotated();
		cam.yRotated = this->getCamera()->getYRotated();
		cam.xAngle = this->getCamera()->m_xRotationAngle;
		cam.yAngle = this->getCamera()->m_yRotationAngle;

		return cam;
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