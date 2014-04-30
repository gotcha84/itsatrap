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
#include "DynamicWorld.h"

void handleNewObjectFromServer(int id, float x, float y, float z, int type);
void handleUpdateObjectFromServer(int id, float x, float y, float z, int type);
void handleUpdateWorldFromServer(DynamicWorld *world);

#endif