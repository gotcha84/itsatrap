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
		void start();
		void stop();
		void reset();
		unsigned long getElapsedMilliseconds();
};

#endif