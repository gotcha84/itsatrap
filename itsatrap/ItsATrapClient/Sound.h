#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

using namespace std;

class Sound {
	public:
		Sound();
		~Sound();

		int endWithError(char* msg, int error);
		int wav_demo(string filename);
};