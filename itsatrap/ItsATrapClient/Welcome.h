#pragma once
#ifndef WELCOME_H
#define WELCOME_H

//Scoreboard.h
#include <string>
#include <utility>
#include <vector>
#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"
#include <FTGL/ftgl.h>

class Welcome {
public:

	FTGLPixmapFont *font;

	Welcome();
	~Welcome();
	void draw();
	void drawCube(float x, float y, float z);
private:

};

#endif