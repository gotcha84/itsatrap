#pragma once
#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <string>
#include <utility>
#include <vector>
#include <FTGL/ftgl.h>
#include <fstream>
#include <algorithm>

#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"

using namespace std;

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