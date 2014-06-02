#include "Sound.h"

// default constructor
Sound::Sound() {
	engine = createIrrKlangDevice();
	if (!engine)
		perror("error starting up the engine");

	path = "../Sound/background.wav";
	radius = 5;
}

Sound::Sound(char* s) {
	engine = createIrrKlangDevice();
	if (!engine)
		perror("error starting up the engine");

	path = s;
	radius = 5;
}

// destructor
Sound::~Sound() {
	delete engine;
	engine = nullptr;
}

void Sound::playMusic() {
	music = engine->play3D(path,
		vec3df(0, 0, 0), true, false, true);
}

void Sound::playMusic(bool loop, bool notSure, bool rturnPtr) {
	music = engine->play3D(path,
		vec3df(0, 0, 0), loop, notSure, rturnPtr);
}

void Sound::stopMusic() {
	if (music)
		music->drop(); // release music stream.

	engine->drop(); // delete engine
}

bool Sound::setDistance(float f) {
	if (music) {
		music->setMinDistance(f);
		return true;
	}
	return false;
}

void Sound::setCenterPosition() {
	engine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));
}

void Sound::changePosition(float f) {
	posOnCircle = f;
	vec3df pos3d(radius * cosf(posOnCircle), 0,
		radius * sinf(posOnCircle * 0.5f));
	if (music)
		music->setPosition(pos3d);
}