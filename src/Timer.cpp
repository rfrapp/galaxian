#include "Timer.h"
#include "SDL/SDL.h"
#include <iomanip>
#include <sstream>
	
void Timer::start()
{
	// start the timer
	started = true;
	
	// unpause the timer
	paused = false;
	
	// get the current clock time
	startTicks = SDL_GetTicks();
	//std::cout << "starting..." << std::endl;
}

void Timer::stop()
{
	// stop the timer
	started = false;
	
	// unpause the timer
	paused = false;
}

void Timer::pause()
{
	if (started && !paused)
	{
		paused = true;
		
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()
{
	if (paused)
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}
	
int Timer::getTicks()
{
	if (started)
	{
		if (paused)
			return pausedTicks;
		else 
			return SDL_GetTicks() - startTicks;
	}
	
	return 0;
}

int Timer::getHours()
{
	if (!paused)
		return ((getTicks() / 1000) / 60 % 60) / 60 % 24;
	else 
		return ((pausedTicks / 1000) / 60 % 60) / 60 % 24;
}

int Timer::getMinutes()
{
	if (!paused)
		return (getTicks() / 1000) / 60 % 60;
	else 
		return (pausedTicks / 1000) / 60 % 60;
}

int Timer::getSeconds()
{
	if (!paused)
	{
		seconds = countdown_from - ((SDL_GetTicks() - startTicks) / 1000);
		return seconds;
	}
	else 
		return seconds;
}
	
bool Timer::isStarted()
{
	return started;
}

std::string Timer::getFormattedTime()
{
	if (!paused && started)
	{
		std::stringstream time;
		time << std::setfill('0') << std::setw(2) << getHours();
		time << ":";
		time << std::setfill('0') << std::setw(2) << getMinutes();
		time << ":";
		time << std::setfill('0') << std::setw(2) << getSeconds();
				
		return time.str();
	}
	else if (paused && started)
	{
		std::stringstream time;
		int seconds = (pausedTicks / 1000) % 60;
		int minutes = (seconds / 60) % 60;
		int hours = (minutes / 60) % 24;
		
		
		time << std::setfill('0') << std::setw(2) << hours;
		time << ":";
		time << std::setfill('0') << std::setw(2) << minutes;
		time << ":";
		time << std::setfill('0') << std::setw(2) << seconds;
				
		return time.str();
	}
	
	
	return "00:00:00";
}

bool Timer::isPaused()
{
	return paused;
}
