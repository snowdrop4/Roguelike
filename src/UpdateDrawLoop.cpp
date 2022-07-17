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
