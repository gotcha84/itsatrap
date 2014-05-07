#pragma once
#ifndef TRAP_H
#define TRAP_H

#include <iostream>
#include <fstream>

#include "Geode.h"
#include "ObjModel.h"
#include "NetworkObjects.h"

// andre
//enum TrapType {
//	Shrink = 0,
//	Immobilize = 1,
//	Intoxicate = 2
//};

namespace sg {
	class Trap : public Geode {
		public:
			int m_ownerId;
			glm::vec3 m_position;
			glm::mat4 m_model;
			//TrapType m_type;

			ObjModel *m_model1;

			Trap();
			Trap(int, glm::vec3 pos);
			Trap(glm::vec3);
			Trap(glm::vec3 currPos, int type);

			~Trap();

			void initModel(ObjModel *model);
			//void loadData();
			//void setBoundingBox(AABB box);
			//AABB getBoundingBox();
			void updateBoundingBox();
			//void calculateBoundingBox();
			
			void setPosition(glm::vec3 pos);
			glm::vec3 getPosition();

			void setMatrix(glm::mat4 model);
			glm::mat4 getMatrix();

			void draw(glm::mat4 parent, glm::mat4 cam);

			void print();

			struct trapObject getTrapObjectForNetworking();
	};
}

#endif