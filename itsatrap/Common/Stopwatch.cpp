#include <ctime>

#include "Stopwatch.h"

Stopwatch::Stopwatch()
{
	timer.start = 0;
	timer.stop = 0;
}

void Stopwatch::start()
{
	timer.start = clock();
}

void Stopwatch::stop()
{
	timer.stop = clock();
}

void Stopwatch::reset()
{
	timer.start = clock();
	timer.stop = 0;
}

unsigned long Stopwatch::getElapsedMilliseconds()
{
	return clock() - timer.start;
}