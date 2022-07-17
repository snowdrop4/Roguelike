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

#include "UpdateDrawLoop.hpp"

#include <iostream>

void run_loop(int updatesPerSec,
              const std::function<bool(int)>    &update,
              const std::function<void(double)> &draw)
{
	const int SKIP_TICKS = 1000 / updatesPerSec;
	const int MAX_FRAMESKIP = 5;
	
	Timer timer;
	timer.start();
	
	Timer fpsTimer;
	fpsTimer.start();
	int framesDrawn = 0;
	int fps = 0;
	
	double nextGameTick = timer.get_ticks();
	
	while (1)
	{
		int loops = 0;
		while (timer.get_ticks() > nextGameTick && loops < MAX_FRAMESKIP)
		{
			if (update(fps)) return;
			nextGameTick += SKIP_TICKS;
			++loops;
		}
		
		double interpolation = double(timer.get_ticks() + SKIP_TICKS - nextGameTick) / double(SKIP_TICKS);
		draw(interpolation);
		
		++framesDrawn;
		if (fpsTimer.get_ticks() > 1000)
		{
			fps = framesDrawn;
			framesDrawn = 0;
			fpsTimer.start();
		}
	}
}
