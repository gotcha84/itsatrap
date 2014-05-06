#pragma once
#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>

//#include "Texture.h"

class HUD {
	public:
		HUD();
		~HUD();

		void draw(int health);
		
		void drawCrossHair(); 
		void drawHealthBar(int health);

		// GLuint loadAndBufferImage( const char *filename);
};

#endif