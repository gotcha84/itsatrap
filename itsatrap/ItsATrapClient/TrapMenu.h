#pragma once
#ifndef SSCOREBOARD_H
#define SCOREBOARD_H

//Scoreboard.h
#include <string>
#include <utility>
#include <vector>
#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"
#include <FTGL/ftgl.h>

class TrapMenu {
public:


	TrapMenu();
	~TrapMenu();
	void draw();
	void drawCube(float x, float y, float z);
	void trapInfo(int state);

	void setInfoState(int state);
	int getInfoState();

private:
	int infoState;
};

#endif