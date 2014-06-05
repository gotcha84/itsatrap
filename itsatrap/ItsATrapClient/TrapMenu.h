#pragma once
#ifndef TRAPMENU_H
#define TRAPMENU_H

//Scoreboard.h
#include <string>
#include <utility>
#include <vector>
#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"
#include <FTGL/ftgl.h>
#include "Stopwatch.h"

class TrapMenu {
public:

	int infoState;
	FTGLPixmapFont *font;

	TrapMenu();
	~TrapMenu();
	void draw();
	void drawCube(float x, float y, float z);
	void trapInfo(int state, float y, float r, float g, float b);

	void setInfoState(int state);
	int getInfoState();

private:
	
};

#endif