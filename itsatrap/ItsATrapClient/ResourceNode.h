#pragma once
#ifndef RESOURCENODE_H
#define RESOURCENODE_H

#include "ObjNode.h"
#include "ParticleSystem.h"

namespace sg {
	class ResourceNode : public ObjNode {
		private:
			int owner;
			bool isActive, isChanneling;
			AABB bouddndingBox;

		public:
			ParticleSystem *m_particles;
			ParticleSystem *m_particles2;

			ResourceNode(int numParticles);
			~ResourceNode();

			ParticleSystem *getParticleSystem();
			void setParticleSystem(ParticleSystem *p);
		
			//void getOwner();
			//void setOwner();
			//void addResourcesToOwner();
			//bool startChanneling(int player);

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
		};
}

#endif