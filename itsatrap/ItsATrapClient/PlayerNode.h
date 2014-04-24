#pragma once
#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "MatrixTransform.h"
#include "MyPlayer.h"

namespace sg {
	class Player : public MatrixTransform {
		public:
			int m_playerID;
			MyPlayer *m_player;

			Player();
			Player(MyPlayer *p);
			~Player();

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
	};
}

#endif