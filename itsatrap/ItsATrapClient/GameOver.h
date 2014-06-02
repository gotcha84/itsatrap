#pragma once
#ifndef GAMEOVER_H
#define GAMEOVER_H

//Scoreboard.h
#include <string>
#include <utility>
#include <vector>
#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"
#include <FTGL/ftgl.h>

class GameOver {
public:

	FTGLPixmapFont *font;

	GameOver();
	~GameOver();
	void draw();
	void drawCube(float x, float y, float z);
	void trapInfo(int state);
	void setTeamScore(int one, int two);
	void displayWin();
	void displayCredits();
private:
	int team1;
	int team2;
};

#endif