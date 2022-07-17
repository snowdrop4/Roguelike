#pragma once

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
