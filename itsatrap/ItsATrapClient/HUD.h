#pragma once
#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Scoreboard.h"
#include <FTGL/ftgl.h>

#include <string>
#include "Sound.h"


//#include "Texture.h"

class HUD {
	public:
		HUD();
		~HUD();

		FTGLPixmapFont *font;
		Scoreboard *board;
		int m_progressTime;
		ISoundEngine *engine;
		ISound *deathSound;

		void draw(int health, int resources, int spawnTime, float flashFade, int hitCrosshairDuration);
		
		void drawCrossHair(); 
		void drawHealthBar(int health);
		void drawResource(int resource);
		void drawDeathTimer(int respawnTime);
		void drawProgressBar(int time);
		void drawFlashbag(float fade); // between 0 and 1
		void drawKillSymbol(bool hit);
		// GLuint loadAndBufferImage( const char *filename);
};

#endif