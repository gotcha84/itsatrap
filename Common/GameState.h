#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

#include "Exports.h"

using namespace std;

enum GameStates {
	WELCOME,
	GAMEREADY,
	GAMEPLAY,
	GAMEOVER
};

class COMMON_API GameState {
	public:
		static GameStates m_gameState;

		static void setWelcome();
		static void setGameReady();
		static void setGameplay();
		static void setGameover();
		static GameStates getState();

		GameState();
		~GameState();
};

#endif