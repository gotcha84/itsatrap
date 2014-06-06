#pragma once
#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "Utilities.h"
#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "HUD.h"
#include "Scoreboard.h"
#include "ObjModel.h"
#include "TrapMenu.h"
#include "GameOver.h"

namespace sg {
	class Player : public Group {
		private:
			float getFadeForFlashOrBlood(int x, int fadeOut);

		public:
			int m_playerID;
			MyPlayer *m_player;
			HUD *m_hud;
			Scoreboard *board;
			bool m_drawBB;
			bool checkMouse;
			TrapMenu *trapMenu;
			GameOver *gameOver;
			Stopwatch *timer;
			int m_elapsedGameTime;
			bool m_gameOver;

			glm::vec4 m_color;
			glm::vec3 m_translate;
			glm::quat m_rotate;
			glm::vec3 m_scale;

			float m_xAngleChange, m_yAngleChange;
			float m_xAngleChangeFactor, m_yAngleChangeFactor;

			ObjModel *m_thisPlayer;
			ObjModel *m_otherPlayer;

			Player();
			Player(glm::vec3 pos);
			Player(MyPlayer *p);
			~Player();

			void initModels();

			void setColor(glm::vec4 color);
			glm::vec4 getColor();

			MyPlayer *getPlayer();
			Camera *getCamera();
			glm::vec3 getPosition();
			glm::mat4 getModelMatrix();

			glm::mat4 rotate(glm::vec3 axis, float theta);
			
			int getPlayerID();
			void setPlayerID(int id);

			void draw(); // draw as current player
			void drawAsCurrentPlayer(glm::mat4 mv);

			void draw(glm::mat4 parent, glm::mat4 cam); // draw as another player
			void drawAsOtherPlayer(glm::mat4 mv);

			void handleXRotation(float magnitude);
			void handleYRotation(float magnitude);
			void handleMovement(unsigned char key);
			
			void move(glm::vec3 delta);
			void moveTo(glm::vec3 pos);

			void lookIn(glm::vec3 direction);
			void lookAt(glm::vec3 point);

			void setUp(glm::vec3 direction);

			void enableDrawBB();
			void disableDrawBB();

			void print();

			bool collidesWith(sg::Player *other);
			//bool knifeHitWith(sg::Player *other);

			struct playerObject getPlayerObjectForNetworking();
			struct cameraObject getCameraObjectForNetworking();

			int getHealth();
			void setHealth(int health);
			void damage(int dmg);
			bool isDead();
			bool isAlive();
	};
}

#endif