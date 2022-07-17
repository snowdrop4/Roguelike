#include "Timer.hpp"

#include <SDL/SDL.h>

Timer::Timer() :
	paused(false),
	started(false),
	startTicks(0),
	pausedTicks(0)
{
	
}

Timer::~Timer() { }

bool Timer::is_paused() const
{ return paused; }

bool Timer::is_started() const
{ return started; }

size_t Timer::get_ticks() const
{
	if      (paused  == true) return pausedTicks;
	else if (started == true) return SDL_GetTicks() - startTicks;
	else return 0;
}

void Timer::start()
{
	paused = false;
	started = true;

	startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	paused = false;
	started = false;
}

void Timer::pause()
{
	paused = true;	
	pausedTicks = SDL_GetTicks() - startTicks;
}

void Timer::unpause()
{
	paused = false;
	startTicks = SDL_GetTicks() - pausedTicks;
	pausedTicks = 0;
}
