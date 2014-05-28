#pragma once
#ifndef RESOURCENODE_H
#define RESOURCENODE_H

#include "ObjNode.h"
#include "ParticleSystem.h"

namespace sg {
	class ResourceNode : public ObjNode {
		private:
			int resourceId;
			int owner;
			bool isActive, isChanneling;

			HWND channelingHandle;

			void channelingCompleted();

		public:
			ParticleSystem *m_particles;
			ParticleSystem *m_particles2;

			ResourceNode(int id, int numParticles);
			~ResourceNode();

			ParticleSystem *getParticleSystem();
			void setParticleSystem(ParticleSystem *p);
			void enableParticles();
			void disableParticles();
		
			int getResourceId();
			//void getOwner();
			//void setOwner();
			//void addResourcesToOwner();
			bool startChanneling(int player);
			void cancelChanneling();

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
		};
}

#endif