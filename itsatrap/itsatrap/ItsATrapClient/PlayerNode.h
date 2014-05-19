#pragma once
#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "Group.h"
#include "MyPlayer.h"

namespace sg {
	class Player : public Group {
		public:
			MyPlayer *m_player;

			Player();
			Player(MyPlayer *p);
			~Player();

			MyPlayer *getPlayer();
			Camera *getCamera();

			void draw();
			void draw(glm::mat4 parent, glm::mat4 camera);

			void move(glm::vec3 delta);
			void moveTo(glm::vec3 pos);

			void lookIn(glm::vec3 direction);
			void lookAt(glm::vec3 point);
	};
}

#endif