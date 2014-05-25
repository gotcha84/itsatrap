#ifndef STOPWATCH_H
#define STOPWATCH_H

struct struct_stopwatch
{
	unsigned long start;
	unsigned long stop;
};

class Stopwatch
{
	public:
		struct_stopwatch timer;
		__declspec(dllexport) Stopwatch();
		__declspec(dllexport) void start();
		__declspec(dllexport) void stop();
		__declspec(dllexport) void reset();
		__declspec(dllexport) unsigned long getElapsedMilliseconds();
};

#endif