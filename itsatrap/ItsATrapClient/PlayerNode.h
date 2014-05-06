#pragma once
#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "HUD.h"

namespace sg {
	class Player : public Group {
		public:
			int m_playerID;		
			MyPlayer *m_player;
			HUD *m_hud;
			
			glm::vec3 m_color;
			glm::vec3 m_translate;
			glm::quat m_rotate;
			glm::vec3 m_scale;

			float m_xAngleChange, m_yAngleChange;
			float m_xAngleChangeFactor, m_yAngleChangeFactor;

			Player();
			Player(glm::vec3 pos);
			Player(MyPlayer *p);
			~Player();

			void setColor(glm::vec3 color);
			glm::vec3 getColor();

			MyPlayer *getPlayer();
			Camera *getCamera();
			glm::vec3 getPosition();
			glm::mat4 getModelMatrix();

			glm::mat4 rotateY(float theta);
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

			void print();

			bool collidesWith(sg::Player *other);
			bool knifeHitWith(sg::Player *other);

			struct playerObject getPlayerObjectForNetworking();

			int getHealth();
			void setHealth(int health);
			void damage(int dmg);
			bool isDead();
			bool isAlive();
	};
}

#endif