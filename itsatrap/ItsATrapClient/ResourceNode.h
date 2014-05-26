#pragma once
#ifndef RESOURCENODE_H
#define RESOURCENODE_H

#include "ObjNode.h"
#include "ParticleSystem.h"

namespace sg {
	class ResourceNode : public ObjNode {
		public:
			ParticleSystem *m_particles;
			ParticleSystem *m_particles2;

			ResourceNode(int numParticles);
			~ResourceNode();

			ParticleSystem *getParticleSystem();
			void setParticleSystem(ParticleSystem *p);

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
		};

}

#endif