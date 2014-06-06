#pragma once
#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <FTGL/ftgl.h>
#include <string>

#include "Scoreboard.h"
#include "Sound.h"
//#include "SceneGraph.h"
#include "Cube.cpp"

//#include "Texture.h"

class HUD {
	public:
		HUD();
		~HUD();

		FTGLPixmapFont *font;
		Scoreboard *board;
		int m_progressTime;
		ISoundEngine *ouchSound;
		ISoundEngine *deathSound;

		sg::Cube *test;

		void draw(int health, int resources, int spawnTime, float flashFade, float bloodFade, int hitCrosshairDuration, int recallElapsed, string msg, int gameTime);
		
		void drawCrossHair(); 
		void drawHealthBar(int health);
		void drawResource(int resource);
		void drawInfoMessage(string msg);
		void drawDeathTimer(int respawnTime);
		void drawProgressBar(int time);
		void drawFlashbag(float fade); // between 0 and 1
		void drawBlood(float fade); // between 0 and 1
		void drawKillSymbol(bool hit);
		void drawClock(int time);
		// GLuint loadAndBufferImage( const char *filename);
};

#endif