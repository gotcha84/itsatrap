#include <ctime>

#include "Stopwatch.h"

Stopwatch()
{
	timer.start = 0;
	timer.stop = 0;
}

void startTimer()
{
	timer.start = clock();
}

void stopTimer()
{
	timer.stop = clock();
}

unsigned long getElapsedMilliseconds()
{
	return clock() - timer.start;
}