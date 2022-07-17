#pragma once

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

///
/// Snazzy interface for SDL_GetTicks().
/// Keeps track of the time passed since the timer was started.
/// Can be paused, unpaused and stopped.
///

#include "UtilDefines.hpp"
#include <cstddef>

class Timer
{
	public:
		Timer();
		COPY_CTOR(Timer) = default;
		MOVE_CTOR(Timer) = default;
		COPY_ASSIGN(Timer) = default;
		MOVE_ASSIGN(Timer) = default;
		~Timer();
		
		bool is_paused()  const;
		bool is_started() const;
		
		/// return : time since started in ms (1/1000th of a second)
		size_t get_ticks() const;
		
		void pause();
		void unpause();
		void start();
		void stop();
		
	private:
		bool paused;
		bool started;
		
		size_t startTicks;
		size_t pausedTicks;
};
