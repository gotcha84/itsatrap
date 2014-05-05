#pragma once
#ifndef TRAP_H
#define TRAP_H


#include <iostream>
#include <fstream>

#include "ObjNode.h"
#include "tiny_obj_loader.h"

//enum TrapType {
//	Shrink = 0,
//	Immobilize = 1,
//	Intoxicate = 2
//};

namespace sg {
	class Trap : public ObjNode {
		public:

			//TrapType m_type;
			Trap(glm::vec3);
			Trap(glm::vec3 currPos, int type);
			~Trap();

			//void loadData();

			//void setBoundingBox(AABB box);
			//AABB getBoundingBox();
			//void updateBoundingBox();
			//void calculateBoundingBox();
			
			void print();
	};
}

#endif