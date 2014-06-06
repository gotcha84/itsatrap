#pragma once
#ifndef TRAPMENU_H
#define TRAPMENU_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>
#include <FTGL/ftgl.h>

#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"
#include "Stopwatch.h"

using namespace std;

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