/*
 * Copyright 2012 Alyxr 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

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
