#pragma once
#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Scoreboard.h"
#include <FTGL/ftgl.h>

#include <string>

//#include "Texture.h"

class HUD {
	public:
		HUD();
		~HUD();

		FTGLPixmapFont *font;
		Scoreboard *board;

		void draw(int health, int resources, int spawnTime, int progressTime, int flashTime);
		
		void drawCrossHair(); 
		void drawHealthBar(int health);
		void drawResource(int resource);
		void drawDeathTimer(int respawnTime);
		void drawProgressBar(int time);
		void drawFlashbag(int time); // max 10 to 0
		// GLuint loadAndBufferImage( const char *filename);
};

#endif