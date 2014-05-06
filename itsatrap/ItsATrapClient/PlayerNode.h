#pragma once
#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "HUD.h"

namespace sg {
	class Player : public MatrixTransform {
		public:
			int m_playerID;		
			MyPlayer *m_player;
			HUD *m_hud;
			glm::vec3 m_color;

			Player();
			Player(glm::vec3 pos);
			Player(MyPlayer *p);
			~Player();

			void setColor(glm::vec3 color);
			glm::vec3 getColor();

			MyPlayer *getPlayer();
			Camera *getCamera();
			glm::vec3 getPosition();
			
			int getPlayerID();
			void setPlayerID(int id);

			void draw();
			void draw(glm::mat4 parent, glm::mat4 cam);

			void move(glm::vec3 delta);
			void moveTo(glm::vec3 pos);

			void lookIn(glm::vec3 direction);
			void lookAt(glm::vec3 point);

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