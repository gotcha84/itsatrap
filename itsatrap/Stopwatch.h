#ifndef STOPWATCH_H
#define STOPWATCH_H

struct struct_stopwatch
{
	unsigned long start;
	unsigned long stop;
};

class Stopwatch
{
	private:
		struct_stopwatch timer;

	public:
		Stopwatch();
		void startTimer();
		void stopTimer();
		unsigned long getElapsedMilliseconds();
};

#endif