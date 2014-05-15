#pragma once
#ifndef HUD_H
#define HUD_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>

//#include "Texture.h"

class HUD {
	public:
		HUD();
		~HUD();

		void draw(int health, int resources);
		
		void drawCrossHair(); 
		void drawHealthBar(int health);
		void drawResource(int resource);
		void drawDeathTimer(float respawnTime);

		// GLuint loadAndBufferImage( const char *filename);
};

#endif