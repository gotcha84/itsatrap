#ifndef PARTICLESYSTEMNODE_H
#define PARTICLESYSTEMNODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Node.h"

namespace sg {
	class ParticleSystemNode : public Node {
		public:

			ParticleSystemNode(int numParticles);
			~ParticleSystemNode();

			void draw(glm::mat4 parent, glm::mat4 cam);
			void print();
	};
}

#endif