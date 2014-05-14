#pragma once
#ifndef SOUND_H
#define SOUND_H

#if defined(WIN32)
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#else
#include "../common/conio.h"
#endif

#include <stdio.h>
#include <irrKlang.h>
#include <string>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class Sound {
	public:
		ISoundEngine* engine;
		ISound* music;
		float radius;
		Sound();
		Sound(char* s);
		~Sound();
		void playMusic();
		void playMusic(bool loop, bool notSure, bool rturnPtr);
		void stopMusic();
		bool setDistance(float f);
		void setCenterPosition();
		void changePosition(float f);

	private:
		const char *path;
		float posOnCircle;
		vec3df pos3d;

};

#endif