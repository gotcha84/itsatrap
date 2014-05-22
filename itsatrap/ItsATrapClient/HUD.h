#pragma once
#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "FTGL/ftgl.h"

#include <string>

//#include "Texture.h"

class HUD {
	public:
		HUD();
		~HUD();

		//FTGLFont *font;

		void draw(int health, int resources);
		
		void drawCrossHair(); 
		void drawHealthBar(int health);
		void drawResource(int resource);
		void drawDeathTimer(float respawnTime);

		// GLuint loadAndBufferImage( const char *filename);
};

#endif