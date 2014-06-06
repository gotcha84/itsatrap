#pragma once
#ifndef RESOURCENODE_H
#define RESOURCENODE_H

#include "ConfigSettings.h"
#include "ObjNode.h"
#include "ParticleSystem.h"
#include "Texture.h"

namespace sg {
	class ResourceNode : public ObjNode {
		private:
			int resourceId;
			int owner;

		public:
			ParticleSystem *m_particles;
			ParticleSystem *m_particles2;

			ResourceNode(int id, int numParticles);
			~ResourceNode();

			ParticleSystem *getParticleSystem();
			void setParticleSystem(ParticleSystem *p);
			void setParticleSystemOrigin(glm::vec3 origin);
			void enableParticles();
			void disableParticles();
		
			int getResourceId();
			void setOwnerColor(int playerId);
			void setInactiveColor();
			void setActiveColor();

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
		};
}

#endif