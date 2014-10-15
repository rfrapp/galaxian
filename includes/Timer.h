#include <iostream>
#include <string>
#include "SDL/SDL.h"

#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
	int startTicks;
	int pausedTicks;
	int countdown_from;
	int seconds;
	bool paused;
	bool started;

public:
	Timer(int s) : countdown_from(s), seconds(0)
	{
		startTicks = 0;
		pausedTicks = 0;
		paused = false;
		started = false;
	}
	
	void reset() { startTicks = SDL_GetTicks(); }
	void start();
	void stop();
	void pause();
	void unpause();
	
	int getTicks();
	int getHours();
	int getMinutes();
	int getSeconds();
	std::string getFormattedTime();
	bool isStarted();
	bool isPaused();
};

#endif
