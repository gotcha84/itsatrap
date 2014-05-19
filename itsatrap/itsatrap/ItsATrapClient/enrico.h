#ifndef ENRICO_H
#define ENRICO_H

#include <stdio.h>

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// graphics
#include "ClientInstance.h"
#include "Window.h"
#include "SceneGraph.h"

// networking
#include "Client.h"
#include "WorldState.h"

void testAddCube(int id, float x, float y, float z);
int testUpdate(int id, float x, float y, float z);
void testUpdateWorld(WorldState *world);

#endif